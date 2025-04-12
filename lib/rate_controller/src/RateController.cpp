#include <RateController.h>

void RateController::Init()
{
}

void RateController::Process(const float throttleReq, const float rollReq,
							 const float pitchReq, const float yawReq)
{
	// transform the user input into rotation rates, limit them for easier control
	float rollRate = ReqInputToRotationRate(rollReq);
	float pitchRate = ReqInputToRotationRate(pitchReq);
	float yawRate = ReqInputToRotationRate(yawReq);

	Serial.printf("Control inputs: T %f | R %f °/s | P %f °/s | Y %f °/s\n",
				  throttleReq, rollRate, pitchRate, yawRate);
}

float RateController::ReqInputToRotationRate(const float input)
{
	auto rate = MULTIPLIER * (input - USER_INPUT_MID);

	// clamp the rate
	rate = max(rate, -MAX_ROT_RATE_DEG_PER_SEC);
	rate = min(rate, MAX_ROT_RATE_DEG_PER_SEC);

	return rate;
}