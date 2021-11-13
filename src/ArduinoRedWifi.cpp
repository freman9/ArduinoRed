#include <ArduinoRedWifi.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//external variables
extern StaticJsonDocument<1024> configurationDoc;

//own variables

//methods

ArduinoRedWifi::ArduinoRedWifi() {}

void ArduinoRedWifi::setup() const
{
    WifiConnect();
}

void ArduinoRedWifi::loop() const
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Debug("Wifi disconnected,trying to reconnect...");
        WifiConnect();
    }
}

void ArduinoRedWifi::WifiConnect() const
{
    ////WiFi.mode(WIFI_STA);
    WiFi.begin(configurationDoc["wifi"]["wifiSSID"].as<String>().c_str(), configurationDoc["wifi"]["wifiPassword"].as<String>().c_str());
    Debug("connecting to: " + configurationDoc["wifi"]["wifiSSID"].as<String>(), true, false);

    int Wifiattempts = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Debug(".", false, false);
        ++Wifiattempts;
        if (Wifiattempts > 120) //60 sec
        {
            Debug(" can't connect to WiFi, restarting");
            ESP.restart();
        }
    }
    Debug("connected", false);
    Debug("local IP address: " + WiFi.localIP().toString() + ", host name: " + configurationDoc["device"]["deviceName"].as<String>());

    Debug("RSSI: " + String(WiFi.RSSI()) + "dBm");

    if (!MDNS.begin(configurationDoc["device"]["deviceName"].as<String>().c_str()))
        Debug("Error setting up MDNS responder!");
}
