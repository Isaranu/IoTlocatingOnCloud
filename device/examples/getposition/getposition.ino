#include "IoTlocatingOnCloud.h"

const char *ssid = "{ssid}";
const char *passw = "{password}";

IoTlocatingOnCloud IoT;

void setup() {
  Serial.begin(115200);
  IoT.connect(ssid, passw);
  IoT.sendAroundMacaddress();
}

void loop() {
  //..
}
