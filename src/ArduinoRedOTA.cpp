#include <ArduinoRedOTA.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//external variables
extern const char *deviceName;

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
    ArduinoOTA.setHostname(deviceName);
    ArduinoOTA.setPassword("2009");

    ArduinoOTA.begin();

    Debug("OTA Ready");
}
