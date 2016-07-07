#ifndef _NTP_UTILS_H_
#define _NTP_UTILS_H_

class NTPUtils {
 public:
  static void ExtractTimestamp(unsigned char* timestamp,
                               unsigned long* seconds,
                               unsigned long* fraction);

  static int TimestampFractionToMillis(unsigned long fraction);
  static long TimestampFractionToMicros(unsigned long fraction);

  static void ExtractTimestampWithMillis(unsigned char* timestamp,
                                         unsigned long* seconds,
                                         int* ms);

                                         
  static void ExtractTimestampWithMicros(unsigned char* timestamp,
                                         unsigned long* seconds,
                                         long* us);

  static double ExtractTimestampAsDouble(unsigned char* timestamp);
                                         
};

#endif
