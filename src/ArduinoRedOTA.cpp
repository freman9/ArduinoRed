#include <ArduinoRedOTA.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//external variables
extern const char *deviceName;
extern const char *arduinoRedCode;

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
    ArduinoOTA.setPassword(arduinoRedCode);
    //upload_flags =
    //    --auth=pass
    ArduinoOTA.begin();

    Debug("OTA Ready");
}
