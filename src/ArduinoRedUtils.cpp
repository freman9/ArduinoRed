#include <ArduinoRedUtils.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//own variables

//methods
ArduinoRedUtils::ArduinoRedUtils() {}

String ArduinoRedUtils::SimpleJsonGenerator(String element, String value) const
{
    String JsonObj = "{\"" + element + "\":\"" + value + "\"}";
    return JsonObj;
}

String ArduinoRedUtils::SimpleJsonGenerator(String element, int value) const
{
    String JsonObj = "{\"" + element + "\":" + String(value) + "}";
    return JsonObj;
}

boolean ArduinoRedUtils::cmpStr(const char *str1, const char *str2) const
{
    if (strcmp(str1, str2) == 0)
        return true;
    else
        return false;
}