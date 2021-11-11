#ifndef ArduinoRedWifi_h
#define ArduinoRedWifi_h

#include <functional>

#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>

#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#endif

class ArduinoRedWifi
{
private:
public:
protected:
    ArduinoRedWifi();

    std::function<void(String)> updateClientConfigurationDocCallback;

    void setup() const;

    void loop() const;

    void getArduinoRedConfiguration() const;

private:
    void WifiConnect() const;
};

#endif