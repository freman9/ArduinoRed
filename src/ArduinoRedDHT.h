#ifndef ArduinoRedDHT_h
#define ArduinoRedDHT_h

#include <DHTesp.h>
#include <ArduinoJson.h>
#include <functional>

class ArduinoRedDHT
{
private:
#define DHTTYPE DHT22
    mutable DHTesp dht;

    mutable long lastDhtRefresh;
    mutable float oldTemperature, oldHumidity;
    mutable float diffTemperature, diffHumidity;
    mutable int DHTRefreshLag_sec;

    mutable String topicThermostat;

public:
protected:
    ArduinoRedDHT();

    std::function<String(String, String)> getClientConfigurationDocCallback;

    std::function<void(const char *, const char *)> mqttPublishCallback;

    void setup() const;

    void loop() const;

private:
    void setDHTConfig() const;

    void RefreshDHT() const;
};

#endif