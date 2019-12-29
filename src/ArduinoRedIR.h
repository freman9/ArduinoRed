#ifndef ArduinoRedIR_h
#define ArduinoRedIR_h

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
#include <functional>
#include <ArduinoRedUtils.h>

class ArduinoRedIR : private ArduinoRedUtils
{
private:
    mutable String topicRemote;
    mutable String topicRemoteRecivedCode;

    mutable decode_results irRecvResults;
    mutable uint64_t lastIrCodeRecived = 0;
    mutable int irCodeCounter = 0;
    mutable boolean runIrDump = false;

    mutable String prontoCode = "";

public:
    mutable boolean remoteLearningMode;

    void transmitIRCode(String prontoCodeMsg) const;

protected:
    ArduinoRedIR();

    std::function<String(String, String)> getClientConfigurationDocCallback;

    std::function<void(const char *, const char *)> mqttPublishCallback;

    void setup() const;

    void loop() const;

private:
    String uInt64toStrHex(uint64_t num) const;

    void irCodeRecived(decode_results *irRecvResults) const;

    String irDump(decode_results *results) const;
};

#endif