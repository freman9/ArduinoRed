#ifndef ArduinoRedWifi_h
#define ArduinoRedWifi_h

#include <ArduinoJson.h>

#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>

#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#endif

class ArduinoRedWifi
{
private:
public:
protected:
    ArduinoRedWifi();

    void setup() const;

    void loop() const;

private:
    void WifiConnect() const;
};

#endif