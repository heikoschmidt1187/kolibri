#include <RateController.h>

RateController::RateController(const float dt)
	: pidRoll(PidController::Config{ .Kp = 0.6F, .Ki = 3.5F, .Kd = 0.03F }, dt)
	, pidPitch(PidController::Config{ .Kp = 0.6F, .Ki = 3.5F, .Kd = 0.03F }, dt)
	, pidYaw(PidController::Config{ .Kp = 2.F, .Ki = 12.F, .Kd = 0.F }, dt)
{
}

void RateController::Init()
{
}

void RateController::Process(Mpu6050 &mpu6050, MotorManager &motorManager,
							 float throttleReq, const float rollReq,
							 const float pitchReq, const float yawReq)
{
	// transform the user input into rotation rates, limit them for easier control
	float rollRate = ReqInputToRotationRate(rollReq);
	float pitchRate = ReqInputToRotationRate(pitchReq);
	float yawRate = ReqInputToRotationRate(yawReq);

	// determine error according to gyro measurements
	float errorRollRate = rollRate - mpu6050.GetRollRate();
	float errorPitchRate = pitchRate - mpu6050.GetPitchRate();
	float errorYawRate = yawRate - mpu6050.GetYawRate();

	// limit the throttle to 80 % in order to have room for controller
	float throttle =
		min(throttleReq,
			LOWER_USER_INPUT + .8F * (UPPER_USER_INPUT - LOWER_USER_INPUT));

	// get the controlled input for each angle
	float inputRoll = pidRoll.Update(errorRollRate);
	float inputPitch = pidPitch.Update(errorPitchRate);
	float inputYaw = pidYaw.Update(errorYawRate);

	// set motor requests
	float throttles[MotorManager::MOT_NoOf];

	/* clang-format off */
	throttles[MotorManager::MOT_FR] = throttle - inputRoll - inputPitch - inputYaw;
	throttles[MotorManager::MOT_RR] = throttle - inputRoll + inputPitch + inputYaw;
	throttles[MotorManager::MOT_RL] = throttle + inputRoll + inputPitch + inputYaw;
	throttles[MotorManager::MOT_FL] = throttle + inputRoll - inputPitch + inputYaw;
	/* clang-format on */

	for (auto i = 0U; i < MotorManager::MOT_NoOf; ++i) {
		// clamp at 2000 µs to avoid overload
		throttles[i] = min(throttles[i], 2000.F);

		// clamp at idle throttle (18 % throttle) during flight to avoid crash
		throttles[i] = max(throttles[i], USER_THROTTLE_IDLE);
	}

	// ensure motors can be switched off when moving throttle stick to lowest position
	if (throttleReq < 1050.F) {
		memset(&throttles[0], 0x0, sizeof(throttles));

		pidRoll.Reset();
		pidPitch.Reset();
		pidYaw.Reset();

		if (!throttleCutOffLogged) {
			Serial.printf("Cut off throttle request: %f\n", throttleReq);
			throttleCutOffLogged = true;
		}
	} else {
		throttleCutOffLogged = false;
	}

	// populate new control values
	motorManager.SetThrottle(throttles);
}

float RateController::ReqInputToRotationRate(const float input)
{
	auto rate = MULTIPLIER * (input - USER_INPUT_MID);

	// clamp the rate
	rate = min(max(rate, -MAX_ROT_RATE_DEG_PER_SEC), MAX_ROT_RATE_DEG_PER_SEC);

	return rate;
}