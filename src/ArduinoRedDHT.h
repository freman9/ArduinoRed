#ifndef ArduinoRedDHT_h
#define ArduinoRedDHT_h

#include <ArduinoJson.h>
#include <functional>
#include <DHTesp.h>

class ArduinoRedDHT
{
private:
#define DHTTYPE DHT22
    mutable DHTesp dht;

    mutable long lastDhtRefresh;
    mutable float oldTemperature, oldHumidity;

    mutable float diffTemperature;
    mutable float diffHumidity;
    mutable int DHTRefreshLag_sec;

    mutable String topicThermostat;

public:
    std::function<void(const char *, const char *)> mqttPublishCallback;

protected:
    ArduinoRedDHT();

    void refreshDHT(boolean forceDHTUpdate) const;

    void setup() const;

    void loop() const;

private:
};

#endif