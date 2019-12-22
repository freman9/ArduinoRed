# ArduinoRed
control arduino (ESP32/ESP8266) from Node-Red


main.cpp
---------
#include <Arduino.h>


//Configure ArduinoRed

const char *deviceName="ESP32D1";

const char *arduinoRedCode="passkey";

const char *nodeRedURL="...noderedurl..."; //(without http/s)


//WiFi

const char *wifiSSID = "ssid";

const char *wifiPassword = "pass";


//DHT

#define DHTfunctionality //comment if functionality is not needed

extern const uint8_t DHTPin = 17;


//IR

#define IRfunctionality //comment if functionality is not needed

extern const uint16_t irTransmiterPin = 4;

extern const uint16_t irReciverPin = 14;


#include <ArduinoRed.h>


void setup()

{

  Serial.begin(115200);
  
  arduinoRed.setup();
  
}

void loop()

{

  arduinoRed.loop();
  
}
