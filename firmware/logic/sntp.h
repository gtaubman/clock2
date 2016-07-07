#ifndef _SNTPCLIENT_H_
#define _SNTPCLIENT_H_

#include <IPAddress.h>

class SNTPClient {
 public:
  explicit SNTPClient(const char* ntp_server);
 
  bool GetTime();

 private:
  bool ResolveIPAddress();
 
  const char* ntp_server_;
  IPAddress ntp_server_ip_;
};

#endif
