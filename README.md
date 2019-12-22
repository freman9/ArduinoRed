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


Node-Red
--------
The ESP devices needs to get the mqttbroker paramters from node-red,

(you have to set node-red before turning on the ESP)

instructions:

create "http in" node with url "/getArduinoRedConfiguration"

and respond with json object:

"{

  "server":
  
    {
    
      "url":"mqtt broker url",
      
     "port":mqtt port,
     
     "user":"mqtt user",
     
     "password":"mqtt password"
     
     },
     
  "dht":
  
    {
    
      "refreshLag":5,         //dht refresh time in seconds
      
      "diffTemperature":0.2,  //min delta temperature to update thermostat data
      
      "diffHumidity":0.5      //min delta humidity to update thermostat data
      
    },
    
   "topics":                  //optional - you can define multiple topics for th ESP to subscribe
   
      [{"topic":"test","qos":1,"subscribe":true}],
      
   "client":
   
   {
   
    "name":"ESP32D1"
    
   }
   
 }"
 
the device name should match the one typed in the main.cpp.

send the json object to "http response" node.

Device status
-------------

recive device status on topic ["device name"/status]

send device status quary to topic ["device name"/status] payload ["sync"]

Device debug
-------------

control debug messages broadcasting by sending ["startDebug"] or ["stopDebug"] to topic ["device name"/status]

recive debug messages on ["device name"/debug]

DHT
---

recive dht data as json object on topic ["device name"/thermostat/status]

IR
--

recive IR reciver data on topic ["device name"/remote/recivedCode]

send IR command (prontocode style as string) to topic ["device name"/remote/transmitCode]

change mode (learn/normal) by sending ["mode:learn"] or ["mode:normal"] to topic ["device name"/remote]

Control the board
-----------------

send json object to control the board to topic ["device name"/board]

i.e. msg.payload={"pin":12,"state":"LOW","mode":"OUTPUT"}

//state: "LOW"/"HIGH"

//mode: "INPUT"/"OUTPUT"/"INPUT_PULLUP"/"INPUT_PULLUP_16"

