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