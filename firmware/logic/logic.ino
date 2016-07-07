#include <RtcDateTime.h>
#include <RtcDS3231.h>
#include <RtcTemperature.h>
#include <RtcUtility.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include "ntp_utils.h"

// The pin on which we receive interrupts for the RTC PPS signal.
#define RTC_INTERRUPT_PIN 14

// Our connection to the RTC.
RtcDS3231 rtc_;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp_;


// Time stuff
bool checkedNetworkTime = false;

// Function declarations.
void handleClockInterrupt();

void DateTimeToNTP(RtcDateTime* now, unsigned long last_second_tick_micros,
                   unsigned long* seconds, unsigned long* frac);

void PrintNTPTimestamp(unsigned char* buf) {
  unsigned long ts;
  long frac;
  NTPUtils::ExtractTimestampWithMicros(buf, &ts, &frac);
  Serial.print(ts);
  Serial.print(".");
  Serial.println(frac);
}

IPAddress timeServerIP; // time.nist.gov NTP server address
//const char* ntpServerName = "time.nist.gov";
const char* ntpServerName = "pool.ntp.org";
unsigned int localPort = 2390;      // local port to listen for UDP packets

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets


volatile unsigned long seconds_since_2000_;
volatile unsigned long last_second_tick_micros_;

unsigned long micros_to_set_time_at_;
int time_offset_seconds_;

#if defined(ESP8266)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif
#include <Wire.h>  // must be incuded here so that Arduino library object file references work
#include <RtcDS3231.h>



#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}

// Send an NTP request to the time server at the given address.
// Returns the time in micros at which the packet was sent.
unsigned long sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // Set the reference timestamp (the time we think it is) starting at byte 16.
  noInterrupts();
  RtcDateTime fetchedTime(seconds_since_2000_);
  unsigned long ntp_orig_sec, ntp_orig_frac;
  DateTimeToNTP(&fetchedTime, last_second_tick_micros_,
                &ntp_orig_sec, &ntp_orig_frac);
  interrupts();
  packetBuffer[40] = ntp_orig_sec >> 24;
  packetBuffer[41] = ntp_orig_sec >> 16;
  packetBuffer[42] = ntp_orig_sec >> 8;
  packetBuffer[43] = ntp_orig_sec;

  packetBuffer[44] = ntp_orig_frac >> 24;
  packetBuffer[45] = ntp_orig_frac >> 16;
  packetBuffer[46] = ntp_orig_frac >> 8;
  packetBuffer[47] = ntp_orig_frac;

  Serial.println("finishing setting reference ts");

  unsigned long origin_ts;
  int origin_frac_millis;
  NTPUtils::ExtractTimestampWithMillis(&packetBuffer[40], &origin_ts, &origin_frac_millis);
  Serial.print("Sending Origin TS: ");
  Serial.print(origin_ts);
  Serial.print(".");
  Serial.println(origin_frac_millis);
                
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp_.beginPacket(address, 123); //NTP requests are to port 123
  udp_.write(packetBuffer, NTP_PACKET_SIZE);
  udp_.endPacket();
  return micros();
}

void setup() {
  Serial.begin(115200);

  // Initialize our pins.
  pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);

  // Initialize the RTC.
  rtc_.Begin();

  if (!rtc_.IsDateTimeValid()) {
    // If the RTC doesn't have a valid time (usually it's the first power on or the
    // the battery has died) then we set the time to the compile time.
    Serial.print("RTC did not have valid time.  Setting to compile time... ");
    rtc_.SetDateTime(RtcDateTime(__DATE__, __TIME__));
    Serial.println("done.");
  } else {
    Serial.println("RTC has valid date and time.  No need to set it.");
  }

  if (!rtc_.GetIsRunning()) {
    Serial.print("RTC is not running.  Enabling now... ");
    rtc_.SetIsRunning(true);
    Serial.println("done.");
  } else {
    Serial.println("RTC is already running.  No need to enable it.");
  }

  // Disable the 32kHz pin, and enable the PPS pin.
  rtc_.Enable32kHzPin(false);
  rtc_.SetSquareWavePin(DS3231SquareWavePin_ModeClock);
  rtc_.SetSquareWavePinClockFrequency(DS3231SquareWaveClock_1Hz);

  // Enable RTC interrupts.  We do this before initializing WiFi because WiFi initialization
  // can take some time and we want the clock to display the time before WiFi initialization
  // happens.
  attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN),
                  handleClockInterrupt,
                  FALLING);

  // Wed do one of two things depending on whether or not we can connect to WiFi.
  // A) If the clock can not connect to WiFi (either it has no ssid/password pair, or
  //    it can't connect to one that it already knows about), it will enter AP mode and
  //    become a captive portal.  If this happens it will light the No-WiFi LED.  Once
  //    a valid ssid/pw pair has been entered and a WiFi connection has been established,
  //    the clock moves to (B).
  // B) If the clock can connect to WiFi, it will bring up an HTTP server allowing the
  //    user to control various aspects of the clock such as auto-brightness, NTP config,
  //    status indicator API, MDNS name, timezone selection, etc.
  // Because displaying the time is handled via interrupt from the DS3231, time display
  // does not wait for either of these cases.

  // Connect to WiFi.
  Serial.println("Connecting to WiFi...");
  WiFiManager wifiManager;
  wifiManager.autoConnect();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp_.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp_.localPort());
}

unsigned long last_checked_ntp = 0;

void loop () {
  // If we haven't checked the network time at all, or recently, check it.
  if (checkedNetworkTime == false || last_checked_ntp + 1000L * 3600 < millis()) {
    last_checked_ntp = millis();
    
    //get a random server from the pool
    WiFi.hostByName(ntpServerName, timeServerIP); 


    long sent_micros = sendNTPpacket(timeServerIP);
    int packetSize = 0;
    while ((packetSize = udp_.parsePacket()) == 0 && (micros() - sent_micros < 2000000)) {
      // Do nothing.
      delay(1);
    }
    long received_response_micros = micros();
    if (packetSize == 0) {
      return;
    }

    checkedNetworkTime = true;


  noInterrupts();
  RtcDateTime fetchedTime(seconds_since_2000_);
  unsigned long ntp_recv_sec, ntp_recv_frac;
  DateTimeToNTP(&fetchedTime, last_second_tick_micros_,
                &ntp_recv_sec, &ntp_recv_frac);
  interrupts();
  
  byte recv_encoded[8];
  recv_encoded[0] = ntp_recv_sec >> 24;
  recv_encoded[1] = ntp_recv_sec >> 16;
  recv_encoded[2] = ntp_recv_sec >> 8;
  recv_encoded[3] = ntp_recv_sec >> 0;
  recv_encoded[4] = ntp_recv_frac >> 24;
  recv_encoded[5] = ntp_recv_frac >> 16;
  recv_encoded[6] = ntp_recv_frac >> 8;
  recv_encoded[7] = ntp_recv_frac >> 0;
  unsigned long recv_ts;
  long recv_frac_micros;
  NTPUtils::ExtractTimestampWithMicros(&recv_encoded[0], &recv_ts, &recv_frac_micros);

    udp_.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer


    Serial.print("packet received, length=");
    Serial.println(packetSize);
    Serial.print("It took the following number of micros: ");
    Serial.println((received_response_micros - sent_micros));

    // Compute our clock offset:
    //
    //      offset = (t2 - t1) + (t3 - t4)
    //               ---------------------
    //                         2
    double origin_ts = NTPUtils::ExtractTimestampAsDouble(&packetBuffer[24]);
    double received_ts = NTPUtils::ExtractTimestampAsDouble(&packetBuffer[32]);
    double transmit_ts = NTPUtils::ExtractTimestampAsDouble(&packetBuffer[40]);
    double destination_ts = NTPUtils::ExtractTimestampAsDouble(&recv_encoded[0]);

    double clock_offset = ((received_ts - origin_ts) + (transmit_ts - destination_ts)) / 2.0;

    Serial.print("Received   Origin Timestamp: ");
    PrintNTPTimestamp(&packetBuffer[24]);

    Serial.print("Received  Receive Timestamp: ");
    PrintNTPTimestamp(&packetBuffer[32]);

    Serial.print("Received Transmit Timestamp: ");
    PrintNTPTimestamp(&packetBuffer[40]);

    Serial.print("Received Destinat Timestamp: ");
    Serial.print(recv_ts);
    Serial.print(".");
    Serial.println(recv_frac_micros);

    Serial.print("Clock offset: ");
    Serial.println(clock_offset);

    unsigned long micros_offset = (unsigned long) ((fabs(clock_offset) - abs((unsigned long) clock_offset)) * 1000000);
    Serial.print("Micros Offset: ");
    Serial.println(micros_offset);
    //micros_to_set_time_at_ = micros() + (1000000 - abs((clock_offset - (unsigned long) clock_offset)) * 1000000);

    if (abs(micros_offset) <= 5000) {
      Serial.println("Skipping synchronization b/c clock is < 5ms off.");
      return;
    }

    time_offset_seconds_ = (int) clock_offset;
    if (clock_offset > 0) {
      time_offset_seconds_ += 1;
      unsigned long n = micros();
      Serial.print("Last second was at: ");
      Serial.println(last_second_tick_micros_);
      Serial.print("Going to advance time by ");
      Serial.print(time_offset_seconds_);
      Serial.print("s after ");
      Serial.print(1000000 - micros_offset);
      Serial.println(" micros.");
      micros_to_set_time_at_ = last_second_tick_micros_ + 2000000 + (1000000 - micros_offset);
    } else {
      unsigned long n = micros();
      Serial.print("Last second was at: ");
      Serial.println(last_second_tick_micros_);
      Serial.print("Going to move time back by ");
      Serial.print(time_offset_seconds_);
      Serial.print("s after ");
      Serial.print(micros_offset);
      Serial.println(" micros.");
      micros_to_set_time_at_ = last_second_tick_micros_ + 2000000 + micros_offset;
    }
    Serial.print("That's ");
    Serial.println(micros_to_set_time_at_);

    //micros_to_set_time_at_ += 2700;  // Fudge factor for getting and setting the time.

    /*
    RtcDateTime ntpTime = RtcDateTime(
      fetchedTime.Year(),
      fetchedTime.Month(),
      fetchedTime.Day(),
      (epoch % 86400L) / 3600,
      (epoch % 3600) / 60,
      epoch % 60);*/


  

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

/*
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // Add 1 to our number of seconds to give us time to sync up correctly.
    secsSince1900 += 1;

    unsigned long fracHighWord = word(packetBuffer[44], packetBuffer[45]);
    unsigned long fracLowWord = word(packetBuffer[46], packetBuffer[47]);
    unsigned long fracSecs = fracHighWord << 16 | fracLowWord;
Serial.print("Frac seconds: ");
Serial.println(fracSecs);

Serial.print("As double: ");
Serial.println(double(fracSecs));

    double ffrac = (double(fracSecs) / 0x100000000);
    Serial.println(ffrac, 10);
    delay(1000 - int(1000 * ffrac));

  unsigned long a;
  int b;
  NTPUtils::ExtractTimestampWithMillis(&packetBuffer[40], &a, &b);
  Serial.print("Extracted seconds: ");
  Serial.print(a);
  Serial.print(" and millis: ");
  Serial.println(b);

  unsigned long origin_ts;
  int origin_frac_millis;
  NTPUtils::ExtractTimestampWithMillis(&packetBuffer[24], &origin_ts, &origin_frac_millis);
    Serial.print("Received Origin TS: ");
  Serial.print(origin_ts);
  Serial.print(".");
  Serial.println(origin_frac_millis);
  */

  /*
    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    
    */
    
    /*
    // print Unix time:
    Serial.println(epoch);

    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
*/
  // TODO(gtaubman): Disable the RTC interrupt before getting and setting the time.
  //                 Otherwise both the interrupt and this will attempt to use the
  //                 i2c bus at the same time.

  }

  if (micros_to_set_time_at_ != 0 && micros() >= micros_to_set_time_at_) {
    int n = micros();
    unsigned long miss = n - micros_to_set_time_at_;
    Serial.print("It's currently ");
    Serial.println(n);
    Serial.print("Adjusting time by seconds: ");
    Serial.println(time_offset_seconds_);
    Serial.print("Missed our desired setting time by: ");
    Serial.println(miss);
    
    micros_to_set_time_at_ = 0;

    
    int pre_t = micros();
    noInterrupts();
    RtcDateTime tt = rtc_.GetDateTime();
    tt += time_offset_seconds_;
    rtc_.SetDateTime(tt);
    interrupts();
    Serial.print("It takes the follow micros to get and set the time: ");
    Serial.println(micros() - pre_t);
  }

  //sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  /*delay(10000);

  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
  }
  else {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);


    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
  }
*/


}

// Given an RtcDateTime, converts it to an NTP timestamp.
void DateTimeToNTP(RtcDateTime* now, unsigned long last_second_tick_micros,
                   unsigned long* seconds, unsigned long* frac) {
                    Serial.print("sizeof(unsigned long) = ");
                    Serial.println(sizeof(unsigned long));
  unsigned long seconds_since_2000 = now->TotalSeconds64();
  unsigned long seconds_since_1970 = seconds_since_2000 + 946684800UL;
  unsigned long seconds_since_1900 = seconds_since_1970 + 2208988800UL;
  *seconds = seconds_since_1900;

  *frac = (micros() - double(last_second_tick_micros)) * (1LL << 32) * 1.0e-6; 
}

void handleClockInterrupt() {
  RtcDateTime now = rtc_.GetDateTime();
  unsigned long now_us = micros();
  last_second_tick_micros_ = micros();
  seconds_since_2000_ = now.TotalSeconds();

  printDateTime(now);
  Serial.println();

  //Serial.print("Micros: ");
  //Serial.println(now - last_second_tick_micros);
}


