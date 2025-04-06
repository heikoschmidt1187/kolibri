#ifndef BATTERY_MONITOR_H_
#define BATTERY_MONITOR_H_

#include <Arduino.h>

class BatteryMonitor
{
public:
    BatteryMonitor(uint8_t inputPin, uint8_t resolution_bits, uint8_t voltage_divider_ratio);
    float process();

private:
    uint8_t inputPin;
    float divider;
};

#endif /* BATTERY_MONITOR_H_ */
