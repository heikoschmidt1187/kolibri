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

	//switch low pass filter to 10 Hz bandwith
	Wire.beginTransmission(busId);
	Wire.write(0x1AU);
	Wire.write(0x05AU);
	Wire.endTransmission();

	// configure the Gyro full range to 500 °/s with LSB scale 65.5 LSB / °/s
	Wire.beginTransmission(busId);
	Wire.write(0x1BU);
	Wire.write(0x08U);
	Wire.endTransmission();

	// calibrate the offsets by taking a large number of measurements and take
	// the average
	float offsetRoll = 0.F;
	float offsetPitch = 0.F;
	float offsetYaw = 0.F;

	for (auto i = 0U; i < CALIBRATION_MEASURMENTS; ++i) {
		Process();

		offsetRoll += rateRoll;
		offsetPitch += ratePitch;
		offsetYaw += rateYaw;

		delay(1U);
	}

	calibrationOffsetRoll = offsetRoll / CALIBRATION_MEASURMENTS;
	calibrationOffsetPitch = offsetPitch / CALIBRATION_MEASURMENTS;
	calibrationOffsetYaw = offsetYaw / CALIBRATION_MEASURMENTS;

	// correct last measurement
	rateRoll -= calibrationOffsetRoll;
	ratePitch -= calibrationOffsetPitch;
	rateYaw -= calibrationOffsetYaw;

	Serial.printf(
		"Gyro calibration offsets = Roll: %f °/s | Pitch: %f °/s | Yaw: %f °/s\n",
		calibrationOffsetRoll, calibrationOffsetPitch, calibrationOffsetYaw);
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

	rateRoll = ((float)gyroX / SCALE) - calibrationOffsetRoll;
	ratePitch = ((float)gyroY / SCALE) - calibrationOffsetPitch;
	rateYaw = ((float)gyroZ / SCALE) - calibrationOffsetYaw;

	Serial.printf("Gyro raw: R %f P %f Y %f\n", rateRoll, ratePitch, rateYaw);
}
