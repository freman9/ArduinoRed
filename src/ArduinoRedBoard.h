#ifndef ArduinoRedBoard_h
#define ArduinoRedBoard_h

#include <ArduinoJson.h>
#include <functional>

#define numberOfPins 40

class ArduinoRedBoard
{
private:
    mutable String boardState[numberOfPins][2];

    mutable long lastBoardStateRefresh;
    mutable int boardStateRefreshRate_sec;

    mutable String topicBoardPinValues;

public:
    void boardCallback(String payload) const;

protected:
    ArduinoRedBoard();

    std::function<String(String, String)> getClientConfigurationDocCallback;

    std::function<void(const char *, const char *)> mqttPublishCallback;

    void setup() const;

    void loop() const;

private:
    uint8_t str2Enum(String str) const;
};

#endif