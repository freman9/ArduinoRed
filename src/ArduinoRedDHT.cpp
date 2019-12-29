#include <ArduinoRedDHT.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//external variables
extern const uint8_t DHTPin;

//own variables

//methods
ArduinoRedDHT::ArduinoRedDHT()
{
    lastDhtRefresh = 0;
    oldTemperature = 0;
    oldHumidity = 0;
    diffTemperature = 0.2;
    diffHumidity = 0.5;
    DHTRefreshLag_sec = 5;
}

void ArduinoRedDHT::setup() const
{
    topicThermostat = getClientConfigurationDocCallback("client", "name") + "/thermostat/status";

    setDHTConfig();
    dht.setup(DHTPin, DHTesp::DHTTYPE);
    Debug("DHT config: DHTRefreshLag_sec: " + String(DHTRefreshLag_sec) +
          " diffTemperature: " + String(diffTemperature) +
          " diffHumidity: " + String(diffHumidity));
}

void ArduinoRedDHT::loop() const
{
    long now = millis();
    if (now - lastDhtRefresh > DHTRefreshLag_sec * 1000)
    {
        lastDhtRefresh = now;
        RefreshDHT();
    }
}

void ArduinoRedDHT::setDHTConfig() const
{
    DHTRefreshLag_sec = atoi(getClientConfigurationDocCallback("dht", "refreshLag_sec").c_str());
    diffTemperature = atof(getClientConfigurationDocCallback("dht", "diffTemperature").c_str());
    diffHumidity = atof(getClientConfigurationDocCallback("dht", "diffHumidity").c_str());
}

void ArduinoRedDHT::RefreshDHT() const
{
    StaticJsonDocument<100> thermostatDoc;
    String jsonDHTString;

    TempAndHumidity newValues = dht.getTempAndHumidity();
    if (dht.getStatus() != 0)
    {
        Debug("DHT22 error, status code: " + String(dht.getStatusString()));

        thermostatDoc["mode"] = "0";
        serializeJson(thermostatDoc, jsonDHTString);

        mqttPublishCallback(topicThermostat.c_str(), jsonDHTString.c_str());

        return;
    }

    float temperature = newValues.temperature;
    float humidity = newValues.humidity;

    if ((abs(temperature - oldTemperature) < diffTemperature) && (abs(humidity - oldHumidity) < diffHumidity))
        return;

    oldTemperature = temperature;
    oldHumidity = humidity;

    float heatIndex = dht.computeHeatIndex(temperature, humidity);
    float dewPoint = dht.computeDewPoint(temperature, humidity);

    thermostatDoc["mode"] = "1";
    thermostatDoc["temperature"] = String(temperature, 1);
    thermostatDoc["humidity"] = String(humidity, 1);
    thermostatDoc["heatIndex"] = String(heatIndex, 1);
    thermostatDoc["dewPoint"] = String(dewPoint, 1);
    serializeJson(thermostatDoc, jsonDHTString);

    mqttPublishCallback(topicThermostat.c_str(), jsonDHTString.c_str());
}
