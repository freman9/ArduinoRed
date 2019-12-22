#ifndef ArduinoRedBoard_h
#define ArduinoRedBoard_h

#include <ArduinoJson.h>

class ArduinoRedBoard
{
private:
public:
    void boardCallback(String payload) const;

protected:
    ArduinoRedBoard();

private:
    uint8_t str2Enum(String str) const;
};

#endif
