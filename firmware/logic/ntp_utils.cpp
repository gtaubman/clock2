#include "ntp_utils.h"

void NTPUtils::ExtractTimestamp(unsigned char* timestamp,
                                unsigned long* seconds,
                                unsigned long* fraction) {

  *seconds = ((unsigned long) timestamp[0]) << 24 |
             ((unsigned long) timestamp[1]) << 16 |
             ((unsigned long) timestamp[2]) << 8  |
             timestamp[3];
  *fraction = ((unsigned long) timestamp[4]) << 24 |
              ((unsigned long) timestamp[5]) << 16 |
              ((unsigned long) timestamp[6]) << 8  |
              timestamp[7];
}

int NTPUtils::TimestampFractionToMillis(unsigned long fraction) {
  return int(float(fraction) / (1LL << 32) * 1000);
}

long NTPUtils::TimestampFractionToMicros(unsigned long fraction) {
  return int(float(fraction) / (1LL << 32) * 1000000);
}

void NTPUtils::ExtractTimestampWithMillis(unsigned char* timestamp,
                                          unsigned long* seconds,
                                          int* ms) {
  unsigned long frac;
  ExtractTimestamp(timestamp, seconds, &frac);
  *ms = TimestampFractionToMillis(frac);
}

void NTPUtils::ExtractTimestampWithMicros(unsigned char* timestamp,
                                         unsigned long* seconds,
                                         long* us) {
  unsigned long frac;
  ExtractTimestamp(timestamp, seconds, &frac);
  *us = TimestampFractionToMicros(frac);
}

double NTPUtils::ExtractTimestampAsDouble(unsigned char* timestamp) {
  unsigned long seconds;
  unsigned long frac;
  unsigned long frac_us;
  ExtractTimestamp(timestamp, &seconds, &frac);
  frac_us = TimestampFractionToMicros(frac);
  double out = seconds;
  out += frac_us / 1000000.0;
  return out;
}

