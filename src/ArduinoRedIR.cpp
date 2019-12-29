#include <ArduinoRedIR.h>

//external functions
extern void Debug(String DebugLine, boolean addTime = true, boolean newLine = true, boolean sendToMqtt = true);

//external variables
extern uint16_t irTransmiterPin;
extern uint16_t irReceiverPin;

//own variables
IRsend irTransmiter(irTransmiterPin);
IRrecv irRecviver(irReceiverPin);

//methods
ArduinoRedIR::ArduinoRedIR() {}

void ArduinoRedIR::setup() const
{
    Debug("starting IR reciver");
    irRecviver.enableIRIn();
    Debug("starting IR transmiter");
    irTransmiter.begin();

    topicRemoteRecivedCode = getClientConfigurationDocCallback("client", "name") + "/remote/recivedCode";
    topicRemote = getClientConfigurationDocCallback("client", "name") + "/remote";

    remoteLearningMode = false;
    mqttPublishCallback(topicRemote.c_str(), ArduinoRedUtils::SimpleJsonGenerator("mode", "normal").c_str());
}

void ArduinoRedIR::loop() const
{
    if (irRecviver.decode(&irRecvResults))
    {
        irCodeRecived(&irRecvResults);

        if (remoteLearningMode)
            mqttPublishCallback(topicRemoteRecivedCode.c_str(), uInt64toStrHex(irRecvResults.value).c_str());

        if (runIrDump)
        {
            String irDumpStr = irDump(&irRecvResults);
            runIrDump = false;

            mqttPublishCallback(topicRemoteRecivedCode.c_str(), irDumpStr.c_str());
            ////mqttPublishCallback(topicRemote.c_str(), ArduinoRedUtils::SimpleJsonGenerator("mode", "normal").c_str());
            ////remoteLearningMode = false;
        }
    }
}

void ArduinoRedIR::transmitIRCode(String prontoCodeMsg) const
{
    prontoCode += prontoCodeMsg;
    if (prontoCode.length() > 1000)
    {
        Debug("prontoCode message merger error, size>1000, deleteing code");
        prontoCode = "";
    }
    else if (String(prontoCodeMsg[prontoCodeMsg.length() - 1]) == "!")
    {
        prontoCode = prontoCode.substring(0, prontoCode.length() - 1);

        int count = (prontoCode.length() + 1) / 5;
        uint16_t prontoCodeArray[count];

        for (int i = 0; i < count; i++)
        {
            String hexString = "0x" + prontoCode.substring(0, 4);
            prontoCodeArray[i] = uint16_t(strtoul(hexString.c_str(), 0, 16));
            ////Debug(String(i) + ": 0x" + prontoCode.substring(0, 4) + " " + String(prontoCodeArray[i]), false, true, false);
            prontoCode.remove(0, 5);
        }

        Debug("transmiting IR pronto code");
        irTransmiter.sendPronto(prontoCodeArray, count);
        prontoCode = "";
    }
}

String ArduinoRedIR::uInt64toStrHex(uint64_t num) const
{
    char buffer[sizeof(num) * 8 + 1];
    itoa(num, buffer, HEX);
    return String(buffer);
}

void ArduinoRedIR::irCodeRecived(decode_results *irRecvResults) const
{
    uint64_t receivedCode = irRecvResults->value;

    serialPrintUint64(receivedCode, HEX);
    Serial.print(" (");
    Serial.print(irRecvResults->bits, DEC);
    Serial.println(" bits)");

    if (receivedCode >= (uint64_t)65536)
    {
        if (lastIrCodeRecived == receivedCode)
        {
            irCodeCounter += 1;
            if (irCodeCounter == 2)
            {
                runIrDump = true;
                irCodeCounter = 0;
            }
        }
        else
        {
            lastIrCodeRecived = receivedCode;
            irCodeCounter = 0;
        }
    }
    irRecviver.resume();
    delay(100);
}

String ArduinoRedIR::irDump(decode_results *results) const
{
    Serial.println("----------------------------------------");
    uint16_t count = results->rawlen;
    String remoteType;
    if (results->decode_type == UNKNOWN)
    {
        remoteType = "Unknown encoding: ";
    }
    else if (results->decode_type == NEC)
    {
        remoteType = "Decoded NEC: ";
    }
    else if (results->decode_type == SONY)
    {
        remoteType = "Decoded SONY: ";
    }
    else if (results->decode_type == RC5)
    {
        remoteType = "Decoded RC5: ";
    }
    else if (results->decode_type == RC5X)
    {
        remoteType = "Decoded RC5X: ";
    }
    else if (results->decode_type == RC6)
    {
        remoteType = "Decoded RC6: ";
    }
    else if (results->decode_type == RCMM)
    {
        remoteType = "Decoded RCMM: ";
    }
    else if (results->decode_type == PANASONIC)
    {
        remoteType = "Decoded PANASONIC - Address: " + uInt64toStrHex(results->address) + " Value: ";
    }
    else if (results->decode_type == LG)
    {
        remoteType = "Decoded LG: ";
    }
    else if (results->decode_type == JVC)
    {
        remoteType = "Decoded JVC: ";
    }
    else if (results->decode_type == AIWA_RC_T501)
    {
        remoteType = "Decoded AIWA RC T501: ";
    }
    else if (results->decode_type == WHYNTER)
    {
        remoteType = "Decoded Whynter: ";
    }
    else if (results->decode_type == NIKAI)
    {
        remoteType = "Decoded Nikai: ";
    }
    Serial.print(remoteType);
    serialPrintUint64(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.println(" bits)");
    Serial.print("Raw (");
    Serial.print(count, DEC);
    Serial.print("): {");

    for (uint16_t i = 1; i < count; i++)
    {
        if (i % 100 == 0)
            yield(); // Preemptive yield every 100th entry to feed the WDT.
        if (i & 1)
        {
            Serial.print(results->rawbuf[i] * kRawTick, DEC);
        }
        else
        {
            Serial.print(", ");
            Serial.print((uint32_t)results->rawbuf[i] * kRawTick, DEC);
        }
    }
    Serial.println("};");
    Serial.println("----------------------------------------");

    String remoteCodeType = remoteType + "0x" + uInt64toStrHex(results->value);
    return remoteCodeType;
}
