#ifndef ArduinoRedMqttClient_h
#define ArduinoRedMqttClient_h

#include <ArduinoJson.h>
#include <functional>
#include <PubSubClient.h>
#include <ArduinoRedUtils.h>

#ifdef ESP32
#include <WiFiClientSecure.h>
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

class ArduinoRedMqttClient : private ArduinoRedUtils
{
private:
    const boolean retained = true;
    const boolean willRetain = false;
    const byte willQoS = 1;

    mutable String topicStatus;
    mutable String topicDebug;
    mutable String topicBoard;

    //if (IRfunctionalityState){
    mutable String topicRemote = "null";             ////
    mutable String topicRemoteTransmitCode = "null"; ////
                                                     //}

    //mqttDebug
    const int pubSubClientDebugBufferLimit = 1005; ////
    mutable boolean pubSubClientDebugState = false;
    mutable String pubSubClientDebugBuffer;

public:
    boolean IRfunctionalityState = false;

    void addtoDebugBuff(String debugString) const;

    void mqttPublish(const char *topic, const char *payload) const;

protected:
    ArduinoRedMqttClient();

    std::function<void(String)> boardCommandCallback;

    std::function<void()> getEspMemStatusCallback;

    std::function<void(boolean)> setRemoteModeCallback;

    std::function<void(String)> transmitIRCodeCallback;

    void setup() const;

    void loop() const;

private:
    void pubSubClientSubscribe() const;

    void pubSubClientCallback(char *topic, uint8_t *payload, unsigned int length) const;

    void mqttServerConnect() const;

    void flushDebugToPubSubClient() const;
};

#endif