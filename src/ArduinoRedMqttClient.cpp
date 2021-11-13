#include <ArduinoRedMqttClient.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//external variables
extern StaticJsonDocument<1024> configurationDoc;

//own variables
#ifdef ESP32
WiFiClientSecure pubSubWiFiClient;
#endif
#ifdef ESP8266
BearSSL::WiFiClientSecure pubSubWiFiClient;
#endif

PubSubClient pubSubClient(pubSubWiFiClient);
int noMqttConnectionCounter = 0;

//methods
ArduinoRedMqttClient::ArduinoRedMqttClient() {}

void ArduinoRedMqttClient::setup() const
{
#ifdef ESP32
    pubSubWiFiClient.setInsecure(); ////
#endif

#ifdef ESP8266
    pubSubWiFiClient.setInsecure(); ////
#endif

    const char *foli = configurationDoc["mqtt"]["mqttServer"]; ////
    //Debug(foli);                                               ////

    pubSubClient.setServer(foli, configurationDoc["mqtt"]["mqttPort"].as<uint16_t>());

    topicStatus = configurationDoc["device"]["deviceName"].as<String>() + "/status";
    topicDebug = configurationDoc["device"]["deviceName"].as<String>() + "/debug";
    topicBoard = configurationDoc["device"]["deviceName"].as<String>() + "/board";

    if (IRfunctionalityState)
    {
        topicRemote = configurationDoc["device"]["deviceName"].as<String>() + "/remote";
        topicRemoteTransmitCode = configurationDoc["device"]["deviceName"].as<String>() + "/remote/transmitCode";
    }

    pubSubClient.setCallback([this](char *topic, uint8_t *payload, unsigned int length)
                             { pubSubClientCallback(topic, payload, length); });

    mqttServerConnect();

    mqttPublish("ESP32D1/status/setup", "{\"key\":\"2009\"}"); ////
}

void ArduinoRedMqttClient::loop() const
{
    if (!pubSubClient.connected())
        mqttServerConnect();

    pubSubClient.loop();

    flushDebugToPubSubClient();
}

void ArduinoRedMqttClient::mqttPublish(const char *topic, const char *payload) const
{
    size_t totalMqttMessageLength = strlen(topic) + strlen(payload);

    if (totalMqttMessageLength > 122)
        Debug("mqttpublish error, topic: " + String(topic) + ", total topic+msg length > 122: " + String(totalMqttMessageLength));

    pubSubClient.publish(topic, payload);
}

void ArduinoRedMqttClient::addtoDebugBuff(String debugString) const
{
    pubSubClientDebugBuffer += debugString;

    if (pubSubClientDebugBuffer.length() > pubSubClientDebugBufferLimit)
        pubSubClientDebugBuffer = "... " + pubSubClientDebugBuffer.substring(pubSubClientDebugBuffer.length() - 1000, pubSubClientDebugBuffer.length());
}

void ArduinoRedMqttClient::pubSubClientSubscribe() const
{
    pubSubClient.subscribe(topicStatus.c_str(), 1);
    pubSubClient.subscribe(topicBoard.c_str(), 1);

    if (IRfunctionalityState)
    {
        pubSubClient.subscribe(topicRemote.c_str(), 1);
        pubSubClient.subscribe(topicRemoteTransmitCode.c_str(), 1);
    }

    for (uint16_t i = 0; i < configurationDoc["topics"].size(); i++)
    {
        String topicTemp = configurationDoc["device"]["deviceName"].as<String>() + "/" + configurationDoc["topics"][i]["topic"].as<String>();
        int qos = configurationDoc["topics"][i]["qos"].as<int>();
        boolean subscribe = (configurationDoc["topics"][i]["subscribe"].as<boolean>());

        Debug("---topic: " + topicTemp + " qos: " + String(qos) + " subscribe: " + String(subscribe));

        if (subscribe)
            pubSubClient.subscribe(topicTemp.c_str(), qos);
    }
}

void ArduinoRedMqttClient::pubSubClientCallback(char *topic, uint8_t *payload, unsigned int length) const
{
    String payloadStr;
    for (unsigned int i = 0; i < length; i++)
        payloadStr += (char)payload[i];

    boolean filteredTopic = false;
    if (strcmp(topic, topicRemoteTransmitCode.c_str()) == 0) //filter debug of topicRemoteTransmitCode messages
        filteredTopic = true;
    /*if (strcmp(topic, topicBoard.c_str()) == 0) //filter debug of topicBoard messages
        filteredTopic = true;*/

    if (!filteredTopic)
        Debug("[mqtt] topic: " + String(topic) + ", payload: " + payloadStr);

    //status, debug & reset
    if (strcmp(topic, topicStatus.c_str()) == 0)
    {
        if (strcmp(payloadStr.c_str(), "sync") == 0)
        {
            Debug(configurationDoc["device"]["deviceName"].as<String>() + ": online");
            mqttPublish(topicStatus.c_str(), (configurationDoc["device"]["deviceName"].as<String>() + ": online").c_str());
        }

        if (strcmp(payloadStr.c_str(), "getEspMemStatus") == 0)
            getEspMemStatusCallback();

        if (strcmp(payloadStr.c_str(), ArduinoRedUtils::SimpleJsonGenerator("debug", "start").c_str()) == 0)
            pubSubClientDebugState = true;

        if (strcmp(payloadStr.c_str(), ArduinoRedUtils::SimpleJsonGenerator("debug", "stop").c_str()) == 0)
            pubSubClientDebugState = false;

        if (strcmp(payloadStr.c_str(), "reset") == 0)
        {
            Debug("reseting...");
            ESP.restart();
        }
    }

    //board
    if (strcmp(topic, topicBoard.c_str()) == 0)
        boardCommandCallback(payloadStr);

    //IR
    if (IRfunctionalityState)
    {
        if (strcmp(topic, topicRemote.c_str()) == 0)
        {
            if (strcmp(payloadStr.c_str(), ArduinoRedUtils::SimpleJsonGenerator("mode", "learn").c_str()) == 0)
                setRemoteModeCallback(true);

            if (strcmp(payloadStr.c_str(), ArduinoRedUtils::SimpleJsonGenerator("mode", "normal").c_str()) == 0)
                setRemoteModeCallback(false);
        }

        if (strcmp(topic, topicRemoteTransmitCode.c_str()) == 0)
        {
            transmitIRCodeCallback(payloadStr);
        }
    }

    //Other Topics
    for (uint16_t i = 0; i < configurationDoc["topics"].size(); i++)
    {
        String topicTemp = configurationDoc["device"]["deviceName"].as<String>() + "/" + configurationDoc["topics"][i]["topic"].as<String>();
        if (strcmp(topic, topicTemp.c_str()) == 0)
        {
            Debug("topic match topics!!!!!");
        }
    }
}

void ArduinoRedMqttClient::mqttServerConnect() const
{
    if (!pubSubClient.connected()) ////was while not good for ded wifi
    {
        Debug("connecting to MQTT broker: " + configurationDoc["mqtt"]["mqttServer"].as<String>() + "...", true, false);
        String clientId = configurationDoc["device"]["deviceName"].as<String>() + "-" + String(WiFi.macAddress());

        if (pubSubClient.connect(clientId.c_str(),
                                 configurationDoc["mqtt"]["mqttUser"].as<String>().c_str(),
                                 configurationDoc["mqtt"]["mqttPassword"].as<String>().c_str(),
                                 topicStatus.c_str(),
                                 willQoS,
                                 willRetain,
                                 (configurationDoc["device"]["deviceName"].as<String>() + ": unexpected disconnection").c_str()))
        {
            Debug("connected", false);

            noMqttConnectionCounter = 0;

            pubSubClientSubscribe();

            mqttPublish(topicStatus.c_str(), (configurationDoc["device"]["deviceName"].as<String>() + ": online").c_str());
            mqttPublish(topicStatus.c_str(), String("send debug state").c_str());
        }
        else
        {
            Debug("failed!", false, true);
            Debug("state: " + String(pubSubClient.state()) + ", ", false, false);
            switch (pubSubClient.state())
            {
            case -4:
                Debug("MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time", false);
                break;
            case -3:
                Debug("MQTT_CONNECTION_LOST - the network connection was broken", false);
                break;
            case -2:
                Debug("MQTT_CONNECT_FAILED - the network connection failed", false);
                break;
            case -1:
                Debug("MQTT_DISCONNECTED - the client is disconnected cleanly", false);
                break;
            case 0:
                Debug("MQTT_CONNECTED - the client is connected", false);
                break;
            case 1:
                Debug("MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT", false);
                break;
            case 2:
                Debug("MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier", false);
                break;
            case 3:
                Debug("MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection", false);
                break;
            case 4:
                Debug("MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected", false);
                break;
            case 5:
                Debug("MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect", false);
                break;
            }

            noMqttConnectionCounter++;
            if (noMqttConnectionCounter < 3)
            {
                Debug(" " + String(noMqttConnectionCounter) + " try, waiting 5 sec and trying again", false);
                delay(5000);
            }
            else
            {
                Debug("failed MQTT server reconnection for " + String(noMqttConnectionCounter) + " times, restarting");
                delay(100);
                ESP.restart();
            }
        }
    }
}

void ArduinoRedMqttClient::flushDebugToPubSubClient() const
{
    if ((pubSubClientDebugBuffer != "") && (pubSubClientDebugState))
    {
        int lineBreakPos = pubSubClientDebugBuffer.indexOf('\n');

        if (lineBreakPos == -1)
            lineBreakPos = pubSubClientDebugBuffer.length(); //todo: not 100 but max mqtt topic+msg length
        if (lineBreakPos > 100)
            lineBreakPos = 100;

        String pubSubClientDebugBufferLine = pubSubClientDebugBuffer.substring(0, lineBreakPos + 1);

        pubSubClientDebugBuffer = pubSubClientDebugBuffer.substring(lineBreakPos + 1, pubSubClientDebugBuffer.length());

        mqttPublish(topicDebug.c_str(), (pubSubClientDebugBufferLine).c_str());
    }
}