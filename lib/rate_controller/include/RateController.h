#ifndef RATE_CONTROLLER_H_
#define RATE_CONTROLLER_H_

#include <Arduino.h>
#include <Gyroscope.h>
#include <Receiver.h>
#include <MotorManager.h>

class RateController {
public:
	RateController() = default;
	void Init();
	void Process(const float throttleReq, const float rollReq,
				 const float pitchReq, const float yawReq);
	float ReqInputToRotationRate(const float input);

private:
	static constexpr auto MAX_ROT_RATE_DEG_PER_SEC = 75.F;
	static constexpr auto LOWER_USER_INPUT = 1000.F;
	static constexpr auto UPPER_USER_INPUT = 2000.F;
	static constexpr auto USER_INPUT_MID =
		0.5F * (LOWER_USER_INPUT + UPPER_USER_INPUT);
	static constexpr auto MULTIPLIER =
		2.F * MAX_ROT_RATE_DEG_PER_SEC / (UPPER_USER_INPUT - LOWER_USER_INPUT);
};

#endif /* RATE_CONTROLLER_H_ */