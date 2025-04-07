#include <Arduino.h>
#include <Wire.h>

#include "Gyroscope.h"

Gyroscope::Gyroscope(const uint8_t busId)
    : busId(busId)
{
}

void Gyroscope::Init()
{
    // power up Gyro
    Wire.beginTransmission(busId);
    Wire.write(0x6BU);
    Wire.write(0x00U);
    Wire.endTransmission();

    // switch low pass filter to 10 Hz bandwith
    Wire.beginTransmission(busId);
    Wire.write(0x1AU);
    Wire.write(0x05AU);
    Wire.endTransmission();

    // configure the Gyro full range to 500 °/s with LSB scale 65.5 LSB / °/s
    Wire.beginTransmission(busId);
    Wire.write(0x1BU);
    Wire.write(0x08U);
    Wire.endTransmission();
}

void Gyroscope::Process()
{
    // read 6 byte starting from roll rate register
    Wire.beginTransmission(busId);
    Wire.write(0x43);
    Wire.endTransmission();

    Wire.requestFrom(busId, 6U);

    int16_t gyroX = (Wire.read() << 8U) | Wire.read();
    int16_t gyroY = (Wire.read() << 8U) | Wire.read();
    int16_t gyroZ = (Wire.read() << 8U) | Wire.read();

    rateRoll = (float)gyroX / SCALE;
    ratePitch = (float)gyroY / SCALE;
    rateYaw = (float)gyroZ / SCALE;
}
