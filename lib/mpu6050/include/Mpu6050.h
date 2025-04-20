#ifndef MPU6050_H_
#define MPU6050_H_

class Mpu6050 {
public:
	Mpu6050(const uint8_t busId);

	void Init();
	void Process();

	float GetRollRate()
	{
		return rateRoll;
	}

	float GetPitchRate()
	{
		return ratePitch;
	}

	float GetYawRate()
	{
		return rateYaw;
	}

	float GetRollAngle()
	{
		return angleRoll;
	}

	float GetPitchAngle()
	{
		return anglePitch;
	}

private:
	static constexpr float SCALE_GYRO = 65.5F;
	static constexpr float SCALE_ACCEL = 4096.F;
	static constexpr uint16_t CALIBRATION_MEASURMENTS = 2000U;

	uint8_t busId = 0U;

	float rateRoll = 0.F;
	float ratePitch = 0.F;
	float rateYaw = 0.F;

	float angleRoll = 0.F;
	float anglePitch = 0.F;

	float calibrationOffsetRoll = 0.F;
	float calibrationOffsetPitch = 0.F;
	float calibrationOffsetYaw = 0.F;
};

#endif /* MPU6050_H_ */