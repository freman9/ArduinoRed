#include <ArduinoRedNtp.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//own variables
const char *NTPServer = "europe.pool.ntp.org";
const int UTCOffset = 7200;
const int NTPUpdateInterval = 60000;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTPServer, UTCOffset, NTPUpdateInterval);

//methods
ArduinoRedNtp::ArduinoRedNtp() {}

void ArduinoRedNtp::setup() const
{
    timeClient.begin();
    timeClient.update();
    Debug("NTP connected");
}

void ArduinoRedNtp::loop() const
{
    timeClient.update();
}

String ArduinoRedNtp::currentTime() const
{
    return String(timeClient.getFormattedTime());
}
