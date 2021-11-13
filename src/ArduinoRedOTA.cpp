#include <ArduinoRedOTA.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//external variables
extern StaticJsonDocument<1024> configurationDoc;

//own variables

//methods
ArduinoRedOTA::ArduinoRedOTA() {}

void ArduinoRedOTA::setup() const
{
    OTASupport();
}

void ArduinoRedOTA::loop() const
{
    ArduinoOTA.handle();
}

void ArduinoRedOTA::OTASupport() const
{
    ArduinoOTA.setHostname(configurationDoc["device"]["deviceName"].as<String>().c_str());
    ArduinoOTA.setPassword(configurationDoc["device"]["arduinoRedCode"].as<String>().c_str());

    ArduinoOTA.begin();

    Debug("OTA Ready");
}
