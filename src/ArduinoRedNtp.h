#ifndef ArduinoRedNtp_h
#define ArduinoRedNtp_h

#include <WiFiUdp.h>
#include <NTPClient.h>

class ArduinoRedNtp
{
private:
public:
    String currentTime() const;

protected:
    ArduinoRedNtp();

    void setup() const;

    void loop() const;

private:
};

#endif
