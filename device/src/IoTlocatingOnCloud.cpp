#include "IoTlocatingOnCloud.h"

#define HOST "YOUR-SERVER-IP"
#define POSTPATH "http://{YOUR-SERVER}/{YOUR-POST-PATH}"
#define PORT 4000
#define retry_wait 1000
#define LAGTIME 100

#define MAXWIFINETWORK 6
#define WAITTIMESCAN 300

String IoTlocatingOnCloud::connect(const char *ssid, const char *passw){

  _ssid = ssid;
  _passw = passw;

  WiFi.begin(_ssid,_passw);
  Serial.print("WiFi start connecting.");

  while (WiFi.status() != WL_CONNECTED){
    delay(retry_wait);
    Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Connected !");
    
    WiFiClient client;
    if(client.connect(HOST, PORT)){
      _path = "GET /";
      _path += " HTTP/1.1\r\n";
      _path += "Host: ";
      _path += HOST;
      _path += ":";
      _path += PORT;
      _path += "\r\n";
      _path += "Connection: keep-alive\r\n\r\n";

      client.print(_path);
      delay(LAGTIME);

      while(client.available()){
        _response = client.readStringUntil('\r');
      }

      return _response;

    }
  }

}

String IoTlocatingOnCloud::sendAroundMacaddress(){

  String macPayload = "{\"datasets\":[";

  /* Read Around WiFi Mac address */
  Serial.println("Listing WiFi network nearly area..");
  int numWiFiAP = WiFi.scanNetworks();
  delay(WAITTIMESCAN);
  if(numWiFiAP > MAXWIFINETWORK){
    numWiFiAP = MAXWIFINETWORK;
  }
  Serial.println("Found = " + String(numWiFiAP) + " networks");

  byte bssid[MAXWIFINETWORK];
  for(int i=0; i<numWiFiAP; i++){
    Serial.print(MACtoString(WiFi.BSSID(i)));
    Serial.print(",");
    Serial.print(WiFi.RSSI(i));
    Serial.print(",");
    Serial.print(WiFi.channel(i));
    Serial.println();

    macPayload += "{\"macAddress\":\"";
    macPayload += MACtoString(WiFi.BSSID(i));
    macPayload += "\",";
    macPayload += "\"rssi\":\"";
    macPayload += WiFi.RSSI(i);
    macPayload += "\",";
    macPayload += "\"channel\":\"";
    macPayload += WiFi.channel(i);
    macPayload += "\"}";
    if(i < numWiFiAP-1){
      macPayload += ",";
    }else{
      macPayload += "]}";
    }
  }

  Serial.println("macPayload = " + macPayload);

  HTTPClient http;

  if(WiFi.status() == WL_CONNECTED){
    http.begin(POSTPATH);
    //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(macPayload);
    _response = http.getString();

    Serial.println(httpCode);
    Serial.println(_response);

    http.end();

  }else{
    Serial.println("Cannot connected to server");
    _response = "Cannot connected to server";
  }

  return _response;

}

String IoTlocatingOnCloud::MACtoString(uint8_t* macAddress) {
    uint8_t mac[6];
    char macStr[18] = { 0 };
    #ifdef ARDUINO_ARCH_SAMD
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", macAddress[5], macAddress[4], macAddress[3], macAddress[2], macAddress[1], macAddress[0]);
    #elif defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
    #endif
    return  String(macStr);
}
