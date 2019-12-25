#ifndef ArduinoRedBoard_h
#define ArduinoRedBoard_h

#include <ArduinoJson.h>
#include <functional>

class ArduinoRedBoard
{
private:
    mutable long lastBoardStateRefresh;
    mutable int boardStateRefreshRate_sec;

    String topicBoardPinValues;

public:
    void boardCallback(String payload) const;

protected:
    ArduinoRedBoard();

    std::function<String(String, String)> getClientConfigurationDocCallback;

    std::function<void(const char *, const char *)> mqttPublishCallback;

    void setup();

    void loop() const;

private:
    uint8_t str2Enum(String str) const;
};

#endif