#ifndef IoTlocatingOnCloud_h
#define IoTlocatingOnCloud_h

#include "Arduino.h"
#include "ESP8266HTTPClient.h"
#include "ESP8266WiFi.h"

class IoTlocatingOnCloud{
  public:
    String connect(const char *ssid, const char *passw);
    String sendAroundMacaddress();
    String MACtoString(uint8_t* macAddress);
    const char *_ssid, *_passw;

  private:
    String _path;
    String _response;
};

#endif
