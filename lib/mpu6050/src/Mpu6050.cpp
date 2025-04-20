#include <Arduino.h>
#include <Wire.h>

#include "Mpu6050.h"

Mpu6050::Mpu6050(const uint8_t busId, float accelOffsetX, float accelOffsetY,
				 float accelOffsetZ)
	: busId(busId)
	, calibrationOffsetAccelX(accelOffsetX)
	, calibrationOffsetAccelY(accelOffsetY)
	, calibrationOffsetAccelZ(accelOffsetZ)
{
}

void Mpu6050::Init()
{
	// power up chip
	Wire.beginTransmission(busId);
	Wire.write(0x6BU);
	Wire.write(0x00U);
	Wire.endTransmission();

	// switch low pass filter to 10 Hz bandwith
	Wire.beginTransmission(busId);
	Wire.write(0x1AU);
	Wire.write(0x05AU);
	Wire.endTransmission();

	// configure the gyro full range to 500 °/s with LSB scale 65.5 LSB / °/s
	Wire.beginTransmission(busId);
	Wire.write(0x1BU);
	Wire.write(0x08U);
	Wire.endTransmission();

	// configure the accel full range to +/- 8g with LSB scale 4096 LSB / g
	Wire.beginTransmission(busId);
	Wire.write(0x1CU);
	Wire.write(0x10U);
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

	calibrationOffsetGyroRoll = offsetRoll / CALIBRATION_MEASURMENTS;
	calibrationOffsetGyroPitch = offsetPitch / CALIBRATION_MEASURMENTS;
	calibrationOffsetGyroYaw = offsetYaw / CALIBRATION_MEASURMENTS;

	// correct last measurement
	rateRoll -= calibrationOffsetGyroRoll;
	ratePitch -= calibrationOffsetGyroPitch;
	rateYaw -= calibrationOffsetGyroYaw;

	Serial.printf(
		"Gyro calibration offsets = Roll: %f °/s | Pitch: %f °/s | Yaw: %f °/s\n",
		calibrationOffsetGyroRoll, calibrationOffsetGyroPitch,
		calibrationOffsetGyroYaw);
	Serial.printf("Accel calibration offsets = X: %f g | Y: %f g | Z: %f g\n",
				  calibrationOffsetAccelX, calibrationOffsetAccelY,
				  calibrationOffsetAccelZ);
}

void Mpu6050::Process()
{
	// read gyro 6 byte starting from roll rate register
	Wire.beginTransmission(busId);
	Wire.write(0x43);
	Wire.endTransmission();

	Wire.requestFrom(busId, 6U);

	int16_t gyroX = (Wire.read() << 8U) | Wire.read();
	int16_t gyroY = (Wire.read() << 8U) | Wire.read();
	int16_t gyroZ = (Wire.read() << 8U) | Wire.read();

	rateRoll = ((float)gyroX / SCALE_GYRO) - calibrationOffsetGyroRoll;
	ratePitch = ((float)gyroY / SCALE_GYRO) - calibrationOffsetGyroPitch;
	rateYaw = ((float)gyroZ / SCALE_GYRO) - calibrationOffsetGyroYaw;

	// read accel 6 byte starting from AccX
	Wire.beginTransmission(busId);
	Wire.write(0x3B);
	Wire.endTransmission();

	Wire.requestFrom(busId, 6U);

	float accX =
		((float)((int16_t)((Wire.read() << 8) | Wire.read())) / SCALE_ACCEL) -
		calibrationOffsetAccelX;
	float accY =
		((float)((int16_t)((Wire.read() << 8) | Wire.read())) / SCALE_ACCEL) -
		calibrationOffsetAccelY;
	float accZ =
		((float)((int16_t)((Wire.read() << 8) | Wire.read())) / SCALE_ACCEL) -
		calibrationOffsetAccelZ;

	float accZ2 = pow(accZ, 2.F);

	angleRoll = atan(accY / sqrt(pow(accX, 2.F) + accZ2)) * 180.F / M_PI;
	anglePitch = -atan(accX / sqrt(pow(accY, 2.F) + accZ2)) * 180.F / M_PI;
}
