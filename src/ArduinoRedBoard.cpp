#include <ArduinoRedBoard.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//external variables
extern StaticJsonDocument<1024> configurationDoc;

//own variables

//methods
ArduinoRedBoard::ArduinoRedBoard()
{
    lastBoardStateRefresh = 0;
    boardStateRefreshRate_sec = 5;

    for (uint16_t pin = 0; pin < numberOfPins; pin++) //initialize boardState array
    {
        boardState[pin][0] = "null";
        boardState[pin][1] = "null";
    }
}

void ArduinoRedBoard::setup() const
{
    topicBoardPinValues = configurationDoc["device"]["deviceName"].as<String>() + "/board/pinValues";
}

void ArduinoRedBoard::loop() const
{

    long now = millis();
    if (now - lastBoardStateRefresh > boardStateRefreshRate_sec * 1000)
    {
        lastBoardStateRefresh = now;

        for (uint16_t pin = 0; pin < numberOfPins; pin++)
        {
            if ((boardState[pin][0] == "INPUT") ||
                (boardState[pin][0] == "INPUT_PULLUP") ||
                (boardState[pin][0] == "INPUT_PULLDOWN") ||
                (boardState[pin][0] == "INPUT_PULLDOWN_16"))
            {
                int digitalPinValue = digitalRead(pin);
                uint16_t analogPinValue = analogRead(pin); ////todo: analogRead()? (0 to 4095)

                String jsonPinValueString;
                StaticJsonDocument<300> pinValueDoc;

                pinValueDoc["pin"] = pin;
                pinValueDoc["mode"] = boardState[pin][0];
                pinValueDoc["state"] = boardState[pin][1];
                pinValueDoc["digital"] = digitalPinValue;
                pinValueDoc["analog"] = analogPinValue;

                serializeJson(pinValueDoc, jsonPinValueString);

                mqttPublishCallback(topicBoardPinValues.c_str(), jsonPinValueString.c_str());
            }
        }
    }
}

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

    if (boardNodeRedInteractionDoc["boardStateRefreshRate"].as<int>())
    {
        boardStateRefreshRate_sec = boardNodeRedInteractionDoc["boardStateRefreshRate"].as<int>();
        Debug("board state refrsh rate changed to: " + String(boardStateRefreshRate_sec));
    }

    int pin = boardNodeRedInteractionDoc["pin"].as<int>();
    String state = boardNodeRedInteractionDoc["state"].as<String>();
    String mode = boardNodeRedInteractionDoc["mode"].as<String>();

    if (str2Enum(mode) != 99) //check for MODE validity
    {
        pinMode(pin, str2Enum(mode));

        if (str2Enum(mode) == OUTPUT)
            digitalWrite(pin, str2Enum(state));

        //save pin mode & state
        boardState[pin][0] = mode;
        boardState[pin][1] = state;
    }
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

    return 99;
}
