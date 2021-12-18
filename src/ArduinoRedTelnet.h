#ifndef ArduinoRedTelnet_h
#define ArduinoRedTelnet_h

#include <TelnetStream.h>
#include <functional>

class ArduinoRedTelnet
{
private:
public:
protected:
    ArduinoRedTelnet();

    std::function<void(boolean)> refreshDHTCallback;

    void setup() const;

    void loop() const;

private:
};

#endif