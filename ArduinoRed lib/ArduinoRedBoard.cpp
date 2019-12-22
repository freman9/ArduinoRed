#include <ArduinoRedBoard.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//methods
ArduinoRedBoard::ArduinoRedBoard() {}

void ArduinoRedBoard::boardCallback(String payload) const
{
    const int boardNodeRedInteractionDocSize = 200;
    StaticJsonDocument<boardNodeRedInteractionDocSize> boardNodeRedInteractionDoc;

    int documentSize = payload.length();
    if (documentSize > boardNodeRedInteractionDocSize)
        Debug("error, json boardNodeRedInteractionDoc too big, > " + String(boardNodeRedInteractionDocSize));

    deserializeJson(boardNodeRedInteractionDoc, payload);

    Debug("board command recived from node-red:", true, true, false);
    serializeJsonPretty(boardNodeRedInteractionDoc, Serial);
    Debug("", false, true, false);

    int pin = boardNodeRedInteractionDoc["pin"].as<int>();
    String state = boardNodeRedInteractionDoc["state"].as<String>();
    String mode = boardNodeRedInteractionDoc["mode"].as<String>();

    pinMode(pin, str2Enum(mode));
    digitalWrite(pin, str2Enum(state));
}

uint8_t ArduinoRedBoard::str2Enum(String str) const
{
    if (str == "INPUT")
        return INPUT;
    if (str == "OUTPUT")
        return OUTPUT;
    if (str == "INPUT_PULLUP")
        return INPUT_PULLUP;
#ifdef ESP32
    if (str == "INPUT_PULLDOWN")
        return INPUT_PULLDOWN;
#endif
#ifdef ESP8266
    if (str == "INPUT_PULLDOWN_16")
        return INPUT_PULLDOWN_16;
#endif

    if (str == "LOW")
        return LOW;
    if (str == "HIGH")
        return HIGH;

    return 0;
}
