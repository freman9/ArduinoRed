#ifndef ArduinoRedDHT_h
#define ArduinoRedDHT_h

#include <DHTesp.h>
#include <ArduinoJson.h>
#include <functional>

class ArduinoRedDHT
{
private:
#define DHTTYPE DHT22
    DHTesp dht;

    long lastDhtRefresh;
    float oldTemperature, oldHumidity;
    float diffTemperature, diffHumidity;
    int DHTRefreshLag_sec;

    String topicThermostat;

public:
protected:
    ArduinoRedDHT();

    std::function<String(String, String)> getClientConfigurationDocCallback;

    std::function<void(const char *, const char *)> mqttPublishCallback;

    void setup();

    void loop();

private:
    void setDHTConfig();

    void RefreshDHT();
};

#endif