#ifndef ArduinoRedWifi_h
#define ArduinoRedWifi_h

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

#include <functional>

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
