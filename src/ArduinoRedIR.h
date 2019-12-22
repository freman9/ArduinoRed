#ifndef ArduinoRedIR_h
#define ArduinoRedIR_h

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
#include <functional>

class ArduinoRedIR
{
private:
    mutable String topicRemote;
    mutable String topicRemoteRecivedCode;

    decode_results irRecvResults;
    uint64_t lastIrCodeRecived = 0;
    int irCodeCounter = 0;
    boolean runIrDump = false;

    String prontoCode = "";

public:
    mutable boolean remoteLearningMode;

    void transmitIRCode(String prontoCodeMsg);

protected:
    ArduinoRedIR();

    std::function<String(String, String)> getClientConfigurationDocCallback;

    std::function<void(const char *, const char *)> mqttPublishCallback;

    void setup() const;

    void loop();

private:
    String uInt64toStrHex(uint64_t num) const;

    void irCodeRecived(decode_results *irRecvResults);

    String irDump(decode_results *results) const;
};

#endif