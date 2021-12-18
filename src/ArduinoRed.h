#ifndef ArduinoRed_h
#define ArduinoRed_h

#include <ArduinoJson.h>

//external variables

//own variables
StaticJsonDocument<1024> configurationDoc;

//methods
void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

#include <ArduinoRedWifi.h>
#include <ArduinoRedOTA.h>
#include <ArduinoRedNtp.h>
#include <ArduinoRedBoard.h>
#include <ArduinoRedMqttClient.h>
#include <ArduinoRedTelnet.h>

#ifdef DHTfunctionality
#include <ArduinoRedDHT.h>
#endif

#ifdef IRfunctionality
#include <ArduinoRedIR.h>
#endif

class ArduinoRed : public ArduinoRedNtp,
                   public ArduinoRedTelnet,
                   public ArduinoRedMqttClient,
                   private ArduinoRedWifi,
                   private ArduinoRedBoard,

#ifdef DHTfunctionality
                   private ArduinoRedDHT,
#endif

#ifdef IRfunctionality
                   private ArduinoRedIR,
#endif

                   private ArduinoRedOTA
{
private:
public:
        ArduinoRed()
        {
#ifdef IRfunctionality
                ArduinoRedMqttClient::IRfunctionalityState = true;
#endif
        }

        void setup()
        {
                Debug("*** statrtup script started ***");

                ArduinoRedWifi::setup();

                ArduinoRedTelnet::refreshDHTCallback = [this](boolean forceDHTUpdate)
                { ArduinoRedDHT::refreshDHT(forceDHTUpdate); };
                ArduinoRedTelnet::setup();

                ArduinoRedNtp::setup();

#ifdef ESP32
                ArduinoRedOTA::setup();
#endif

                ArduinoRedMqttClient::refreshDHTCallback = [this](boolean forceDHTUpdate)
                { ArduinoRedDHT::refreshDHT(forceDHTUpdate); };
                ArduinoRedMqttClient::boardCommandCallback = [this](String payloadStr)
                { ArduinoRedBoard::boardCallback(payloadStr); };
                ArduinoRedMqttClient::getEspMemStatusCallback = [this]()
                { ArduinoRed::getEspMemStatus(); };
                ArduinoRedBoard::mqttPublishCallback = [this](const char *topic, const char *payload)
                { ArduinoRedMqttClient::mqttPublish(topic, payload); };
                ArduinoRedMqttClient::setup();

                ArduinoRedBoard::setup();

#ifdef DHTfunctionality
                ArduinoRedDHT::mqttPublishCallback = [this](const char *topic, const char *payload)
                { ArduinoRedMqttClient::mqttPublish(topic, payload); };
                ArduinoRedDHT::setup();
#endif

#ifdef IRfunctionality
                ArduinoRedMqttClient::setRemoteModeCallback = [this](boolean state)
                { ArduinoRedIR::remoteLearningMode = state; };
                ArduinoRedMqttClient::transmitIRCodeCallback = [this](String code)
                { ArduinoRedIR::transmitIRCode(code); };
                ArduinoRedIR::mqttPublishCallback = [this](const char *topic, const char *payload)
                { ArduinoRedMqttClient::mqttPublish(topic, payload); };
                ArduinoRedIR::setup();
#endif

                ////ArduinoRed::getEspMemStatus();

                Debug("*** statrtup script finished ***");
        }

        void loop()
        {
                ArduinoRedWifi::loop();

#ifdef ESP32
                ArduinoRedOTA::loop();
#endif

                ArduinoRedNtp::loop();

                ArduinoRedMqttClient::loop();

                ArduinoRedBoard::loop();

#ifdef DHTfunctionality
                ArduinoRedDHT::loop();
#endif
#ifdef IRfunctionality
                ArduinoRedIR::loop();
#endif

                ArduinoRedTelnet::loop();
        }

private:
        void getEspMemStatus() const
        {
                uint32_t heapFree = ESP.getFreeHeap() / 1000;

#ifdef ESP32
                uint32_t heapSize = ESP.getHeapSize() / 1000;
                uint32_t minFreeHeap = ESP.getMinFreeHeap() / 1000;
                uint32_t maxAllocHeap = ESP.getMaxAllocHeap() / 1000;
                float heapPercentage = float(heapFree) / float(heapSize) * 100;
                float heapFragmentation = float(maxAllocHeap) / float(heapSize) * 100;
                Debug("lowest level of free heap since boot: " + String(minFreeHeap) + "Kb");
                Debug("free heap: " + String(heapFree) + "Kb/" + String(heapSize) + "Kb " + String(int(heapPercentage)) + " %");
                Debug("largest block of heap that can be allocated at once: " + String(maxAllocHeap) + "Kb");
                Debug("heap fragmentation: " + String(int(heapFragmentation)) + " %");

#endif

#ifdef ESP8266
                uint16_t maxFreeBlockSize = ESP.getMaxFreeBlockSize() / 1000;
                uint8_t heapFragmentation = ESP.getHeapFragmentation();
                Debug("free heap: " + String(heapFree) + "Kb");
                Debug("largest block of heap that can be allocated at once: " + String(maxFreeBlockSize) + "Kb");
                Debug("heap fragmentation: " + String(heapFragmentation) + " %");

#endif
        }
};

ArduinoRed arduinoRed;

//ArduinoRed::Debug
void Debug(String DebugLine, boolean addTime, boolean newLine, boolean sendToMqtt)
{
        String DebugString = "";

        if (addTime)
                DebugString += arduinoRed.ArduinoRedNtp::currentTime() + " ";

        DebugString += DebugLine;

        if (newLine)
                DebugString += '\n';

        //send to Serial
        if (configurationDoc["debug"]["serialDebug"].as<boolean>())
                Serial.print(DebugString);

        //send to Telnet
        if (configurationDoc["debug"]["telnetDebug"].as<boolean>())
                TelnetStream.print(DebugString);

        //send to MQTT
        if ((configurationDoc["debug"]["mqttDebug"].as<boolean>()) && (sendToMqtt))
                arduinoRed.ArduinoRedMqttClient::addtoDebugBuff(DebugString);
}

#endif