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

    boolean cmpStr(const char *str1, const char *str2) const;

private:
};

#endif