#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <String>

class Clock {
 public:
  void SetMDNSName();

 protected:
  String mdns_name_;
};

#endif
