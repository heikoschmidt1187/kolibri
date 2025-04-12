#include "Receiver.h"

Receiver::Receiver(const uint8_t ppmPin)
    : ppmPin(ppmPin), input(RISING)
{
    memset(channelValues, 0, sizeof(channelValues));
}

void Receiver::Init()
{
    input.begin(ppmPin);
}

void Receiver::Process()
{
    actualChannelCount = input.available();

    for (auto i = 0U; (i < actualChannelCount) && (i < MAX_CHANNELS); ++i)
        channelValues[i] = input.read(i + 1); // 1-based
}

uint8_t Receiver::GetActualChannelCount()
{
    return actualChannelCount;
}

float Receiver::GetChannelValue(const uint8_t channel)
{
    if (channel >= actualChannelCount || channel >= MAX_CHANNELS)
        return nan("0");

    return channelValues[channel];
}
