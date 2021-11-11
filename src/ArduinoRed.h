#ifndef ArduinoRed_h
#define ArduinoRed_h

//external variables
extern const bool serialDebug;
extern const bool telnetDebug;
extern const bool mqttDebug;

//own variables

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

                ArduinoRedWifi::updateClientConfigurationDocCallback = [this](String clientConfiguration)
                { ArduinoRedMqttClient::updateClientConfigurationDoc(clientConfiguration); };
                ArduinoRedWifi::setup();

                ArduinoRedTelnet::setup();

                ArduinoRedNtp::setup();

#ifdef ESP32
                ArduinoRedOTA::setup();
#endif

                ArduinoRedWifi::getArduinoRedConfiguration();

                ArduinoRedMqttClient::boardCommandCallback = [this](String payloadStr)
                { ArduinoRedBoard::boardCallback(payloadStr); };
                ArduinoRedMqttClient::getEspMemStatusCallback = [this]()
                { ArduinoRed::getEspMemStatus(); };
                ArduinoRedMqttClient::setup();

                ArduinoRedBoard::mqttPublishCallback = [this](const char *topic, const char *payload)
                { ArduinoRedMqttClient::mqttPublish(topic, payload); };
                ArduinoRedBoard::getClientConfigurationDocCallback = [this](String first, String second)
                { return ArduinoRedMqttClient::getClientConfigurationDoc(first, second); };
                ArduinoRedBoard::setup();

#ifdef DHTfunctionality
                ArduinoRedDHT::mqttPublishCallback = [this](const char *topic, const char *payload)
                { ArduinoRedMqttClient::mqttPublish(topic, payload); };
                ArduinoRedDHT::getClientConfigurationDocCallback = [this](String first, String second)
                { return ArduinoRedMqttClient::getClientConfigurationDoc(first, second); };
                ArduinoRedDHT::setup();
#endif

#ifdef IRfunctionality
                ArduinoRedMqttClient::setRemoteModeCallback = [this](boolean state)
                { ArduinoRedIR::remoteLearningMode = state; };
                ArduinoRedMqttClient::transmitIRCodeCallback = [this](String code)
                { ArduinoRedIR::transmitIRCode(code); };

                ArduinoRedIR::mqttPublishCallback = [this](const char *topic, const char *payload)
                { ArduinoRedMqttClient::mqttPublish(topic, payload); };
                ArduinoRedIR::getClientConfigurationDocCallback = [this](String first, String second)
                { return ArduinoRedMqttClient::getClientConfigurationDoc(first, second); };
                ArduinoRedIR::setup();
#endif

                ArduinoRed::getEspMemStatus();

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
        if (serialDebug)
                Serial.print(DebugString);

        //send to telnet
        if (telnetDebug)
                TelnetStream.print(DebugString);

        //send to MQTT
        if ((mqttDebug) && (sendToMqtt))
                arduinoRed.ArduinoRedMqttClient::addtoDebugBuff(DebugString);
}

#endif