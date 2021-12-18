#include <ArduinoRedTelnet.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//own variables

//methods
ArduinoRedTelnet::ArduinoRedTelnet() {}

void ArduinoRedTelnet::setup() const
{
    TelnetStream.begin();
    Debug("telnet ready, port 23");
}

void ArduinoRedTelnet::loop() const
{
    switch (TelnetStream.read())
    {
    case 'R':
        Debug("restarting from telnet");
        ////TelnetStream.stop();
        delay(100);
        ESP.restart();
        break;
    case 'P':
        Debug("telnet Ping");
        TelnetStream.println("telnet Pong");
        break;
    case 'T':
        Debug("force thermostat data update");
        refreshDHTCallback(true);
        break;
    case 'W':
        Debug("Todo bom");
        break;
    }
}
