#ifndef ArduinoRedOTA_h
#define ArduinoRedOTA_h

//OTA
#include <ArduinoOTA.h>

class ArduinoRedOTA
{
private:
protected:
    ArduinoRedOTA();

    void setup() const;

    void loop() const;

private:
    void OTASupport() const;
};

#endif