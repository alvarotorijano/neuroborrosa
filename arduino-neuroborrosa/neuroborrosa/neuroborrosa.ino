/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>
#include <OneWire.h> //by jim studt https://www.pjrc.com/teensy/td_libs_OneWire.html
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

OneWire  ds(D4);  // on pin 10 (a 4.7K resistor is necessary)

#define UPDATE_INTERVAL 1800

void setup() {
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  Serial.println("Comienza el datalogger");
  // Serial.setDebugOutput(true);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("HackFi", "fxq4xcj2pjfmyqt2cy8k9qwhp");

}

void loop() {
  // wait for WiFi connection
  int i;
  static  char url[150];
  memset(url, 0, 150);
  //Serial.println(readTemp());

     if ((WiFiMulti.run() == WL_CONNECTED)) {

      WiFiClient client;
  
      HTTPClient http;
  
      Serial.print("[HTTP] begin...\n");
  
      sprintf(url, "http://40.74.248.13/data?temp=%g", readTemp());
      Serial.println(url);
      if (http.begin(client, url)) {  // HTTP
  
  
        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
  
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            Serial.println(payload);
            for (i = 0; i < 40; i++){
              digitalWrite(LED_BUILTIN, LOW);
              delay(50);
              digitalWrite(LED_BUILTIN, HIGH);
              delay(50);
            }
            delay(UPDATE_INTERVAL * 1000);
            ESP.restart();
          }
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
  
        http.end();
      } else {
        Serial.printf("[HTTP} Unable to connect\n");
      }
      
    }
  delay(1000);
}


float readTemp() {
  byte i;
  static byte type_s = 0;
  byte data[12];
  byte addr[8];
  float celsius;
  
  while ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    //return -501;
  }
  


  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -500;
  }

 
  // the first ROM byte indicates which chip

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad


  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

OneWire::crc8(data, 8);


  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];

  byte cfg = (data[4] & 0x60);
  // at lower res, the low bits are undefined, so let's zero them
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  //// default is 12 bit resolution, 750 ms conversion time

  celsius = (float)raw / 16.0;

  return celsius;

}
