#ifndef RECEIVER_H_
#define RECEIVER_H_

#include <Arduino.h>
#include <PulsePosition.h>

class Receiver
{
public:
    Receiver(const uint8_t ppmPin);

    void Init();
    void Process();

    uint8_t GetActualChannelCount();
    float GetChannelValue(const uint8_t channel);

private:
    static constexpr auto MAX_CHANNELS = 10U;

    uint8_t ppmPin;

    PulsePositionInput input;
    uint8_t actualChannelCount = 0U;

    float channelValues[MAX_CHANNELS];
};

#endif /* RECEIVER_H_ */