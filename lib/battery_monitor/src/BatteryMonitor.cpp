#include "BatteryMonitor.h"

BatteryMonitor::BatteryMonitor(uint8_t inputPin, uint8_t resolution_bits, uint8_t voltage_divider_ratio)
    : inputPin(inputPin)
{
    divider = powf(2.F, float(resolution_bits - 1)) / (3.3F * voltage_divider_ratio);
}

float BatteryMonitor::process()
{
    return (float)analogRead(inputPin) / divider;
}
