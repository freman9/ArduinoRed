#ifndef ArduinoRedMqttClient_h
#define ArduinoRedMqttClient_h

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <functional>

#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif

class ArduinoRedMqttClient
{
private:
    String clientName;
    String mqttServer;
    uint16_t mqttPort;
    String mqttUser;
    String mqttPassword;

    const boolean retained = true;
    const boolean willRetain = false;
    const byte willQoS = 1;

    String topicStatus;
    String topicDebug;
    String topicBoard;

    //if (IRfunctionalityState){
    String topicRemote = "null";             ////
    String topicRemoteTransmitCode = "null"; ////
    //}

    //mqttDebug
    mutable boolean pubSubClientDebugState = false;
    mutable String pubSubClientDebugBuffer;
    const int pubSubClientDebugBufferLimit = 1005; ////

public:
    boolean IRfunctionalityState = false;

    void addtoDebugBuff(String debugString) const;

    void updateClientConfigurationDoc(String clientConfiguration) const;

    void mqttPublish(const char *topic, const char *payload);

    String getClientConfigurationDoc(String first, String second);

protected:
    ArduinoRedMqttClient();

    std::function<void(String)> boardCommandCallback;

    std::function<void()> getEspMemStatusCallback;

    std::function<void(boolean)> setRemoteModeCallback;

    std::function<void(String)> transmitIRCodeCallback;

    void setup();

    void loop();

private:
    void pubSubClientSubscribe() const;

    void pubSubClientCallback(char *topic, uint8_t *payload, unsigned int length);

    void mqttServerConnect();

    void flushDebugToPubSubClient();
};

#endif