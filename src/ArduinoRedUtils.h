#ifndef ArduinoRedUtils_h
#define ArduinoRedUtils_h

#include <Arduino.h>

class ArduinoRedUtils
{
private:
public:
protected:
    ArduinoRedUtils();

    String SimpleJsonGenerator(String element, String value) const;

    String SimpleJsonGenerator(String element, int value) const;

private:
};

#endif