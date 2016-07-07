#include "sntp.h"

#include <WiFiUdp.h>

SNTPClient::SNTPClient(const char* ntp_server)
    : ntp_server_(ntp_server) { }

bool SNTPClient::GetTime() {
  // Resolve our IP address from the ntp server hostname.  We do this every
  // time GetTime() is called so that DNS-based loadbalancing can be used
  // by NTP servers.
  ResolveIPAddress();
  return true;
}

bool SNTPClient::ResolveIPAddress() {
  return true;
}

