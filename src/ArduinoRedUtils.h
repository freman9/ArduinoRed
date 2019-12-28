#ifndef ArduinoRedUtils_h
#define ArduinoRedUtils_h

#include <ArduinoJson.h>

class ArduinoRedUtils
{
private:
public:
protected:
    ArduinoRedUtils();

    String SimpleJsonGenerator(String element, String value); //todo: convert to template?

    String SimpleJsonGenerator(String element, int value);

private:
};

#endif