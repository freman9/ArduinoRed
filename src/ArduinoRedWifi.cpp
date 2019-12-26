#include <ArduinoRedWifi.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//external variables
extern const char *wifiSSID;
extern const char *wifiPassword;

extern const char *arduinoRedCode;
extern const char *deviceName;
extern const char *nodeRedURL;

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

void ArduinoRedWifi::getArduinoRedConfiguration() const
{
    HTTPClient https;

    String URI = String(nodeRedURL) + "/" + String(deviceName) + "?key=" +
                 String(arduinoRedCode) +
                 "&deviceName=" + String(deviceName);

#ifdef ESP32
    URI = "https://" + URI;
    WiFiClientSecure client;
    https.begin(client, URI);
#endif
#ifdef ESP8266
    URI = "http://" + URI;
    https.begin(URI);
#endif

    String clientConfiguration = "";

    if (https.GET() > 0)
    {
        clientConfiguration = https.getString();

        updateClientConfigurationDocCallback(clientConfiguration);
    }
    else
    {
        Debug("Error on HTTP request");
    }

    https.end();
}

void ArduinoRedWifi::WifiConnect() const
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSSID, wifiPassword);
    Debug("connecting to: " + String(wifiSSID), true, false);

    int Wifiattempts = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Debug(".", false, false);
        ++Wifiattempts;
        if (Wifiattempts > 120) //60 sec
        {
            Debug(" can't connect, reseting...", false);
            ESP.restart();
        }
    }
    Debug("connected", false);
    Debug("local IP address: " + WiFi.localIP().toString() + ", host name: ");

    Debug("RSSI: " + String(WiFi.RSSI()) + "dBm");

    if (!MDNS.begin(deviceName))
        Debug("Error setting up MDNS responder!");
}
