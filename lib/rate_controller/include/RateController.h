#ifndef RATE_CONTROLLER_H_
#define RATE_CONTROLLER_H_

#include <Arduino.h>

#include <Mpu6050.h>
#include <MotorManager.h>
#include <PidController.h>

class RateController {
public:
	RateController(const float dt);

	void Init();
	void Process(Mpu6050 &mpu6050, MotorManager &motorManager,
				 float throttleReq, const float rollReq, const float pitchReq,
				 const float yawReq, const bool emergencyActive);
	void ProcessDirectRollPitch(Mpu6050 &mpu6050, MotorManager &motorManager,
								float throttleReq, const float rollRate,
								const float pitchRate, const float yawReq,
								const bool emergencyActive);
	float ReqInputToRotationRate(const float input);

public:
	static constexpr auto LOWER_USER_INPUT = 1000.F;
	static constexpr auto UPPER_USER_INPUT = 2000.F;

	static constexpr auto USER_INPUT_MID =
		0.5F * (LOWER_USER_INPUT + UPPER_USER_INPUT);
	static constexpr auto USER_THROTTLE_IDLE =
		LOWER_USER_INPUT + .18F * (UPPER_USER_INPUT - LOWER_USER_INPUT);

private:
	static constexpr auto MAX_ROT_RATE_DEG_PER_SEC = 75.F;
	static constexpr auto MULTIPLIER =
		2.F * MAX_ROT_RATE_DEG_PER_SEC / (UPPER_USER_INPUT - LOWER_USER_INPUT);

private:
	PidController pidRoll;
	PidController pidPitch;
	PidController pidYaw;

	bool throttleCutOffLogged = false;
};

#endif /* RATE_CONTROLLER_H_ */