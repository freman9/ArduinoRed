# ArduinoRed
control arduino (ESP32/ESP8266) from Node-Red


main.cpp
---------
#include <Arduino.h>

//DHT
#define DHTfunctionality

//IR
//#define IRfunctionality

#include <ArduinoRed.h>

void setup()
{
  Serial.begin(115200);

  //Device
  configurationDoc["device"]["deviceName"] = "aaa";
  configurationDoc["device"]["arduinoRedCode"] = "aaa";

  //WiFi
  configurationDoc["wifi"]["wifiSSID"] = "aaa";
  configurationDoc["wifi"]["wifiPassword"] = "aaa";

  //MQTT
  configurationDoc["mqtt"]["mqttServer"] = "aaa";
  configurationDoc["mqtt"]["mqttPort"] = aaa;
  configurationDoc["mqtt"]["mqttUser"] = "aaa";
  configurationDoc["mqtt"]["mqttPassword"] = "aaa";

  //Debug
  configurationDoc["debug"]["serialDebug"] = true;
  configurationDoc["debug"]["telnetDebug"] = true;
  configurationDoc["debug"]["mqttDebug"] = true;

  //DHT
  configurationDoc["dht"]["diffTemperature"] = 0.2;
  configurationDoc["dht"]["diffHumidity"] = 0.5;
  configurationDoc["dht"]["DHTRefreshLag_sec"] = 5;
  configurationDoc["dht"]["DHTPin"] = 14;

  //OR
  configurationDoc["ir"]["irTransmiterPin"] = 4;
  configurationDoc["ir"]["irReceiverPin"] = 17;

  //Topics
  configurationDoc["topics"][0]["topic"] = "foli";
  configurationDoc["topics"][0]["qos"] = 0;
  configurationDoc["topics"][0]["subscribe"] = true;

  arduinoRed.setup();
}

void loop()
{
  arduinoRed.loop();
}



Node-Red
--------
The ESPxx communicates with node red via MQTT.


Device status
-------------

receive device status on topic ["device name"/status]

send device status quary to topic ["device name"/status] payload ["sync"]

Device debug
-------------

control debug messages broadcasting by sending {"debug":"start"} or {"debug":"stop"} to topic ["device name"/status]

receive debug messages on ["device name"/debug]

DHT
---

receive dht data as json object on topic ["device name"/thermostat/status]

IR
--

receive IR data on topic ["device name"/remote/recivedCode]

send IR command (prontocode style as string) to topic ["device name"/remote/transmitCode] //split the string by using split node at 70 characters

change mode (learn/normal) by sending {"mode":"learn"} or {"mode":"normal"} to topic ["device name"/remote]

Control the board
-----------------

send json object to control the board to topic ["device name"/board]

i.e. msg.payload={"pin":12,"state":"LOW","mode":"OUTPUT"}

//state: "LOW"/"HIGH"

//mode: "INPUT"/"OUTPUT"/"INPUT_PULLUP"/"INPUT_PULLUP_16"

subscribe to topic ["device name"/board/pinValues] to get pins values (which you previously set as input)

General notes
-------------
message sent to device should not be greater than 256 characters (topic + payload length) [pubsubclient library limitation]
the same for message sent from the device.
