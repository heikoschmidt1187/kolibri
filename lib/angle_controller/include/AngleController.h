#ifndef ANGLE_CONTROLLER_H_
#define ANGLE_CONTROLLER_H_

#include <Arduino.h>

#include <AttitudeEstimator.h>
#include <PidController.h>

class AngleController {
public:
	AngleController(const float dt);

	void Init();
	void Process(const AttitudeEstimator &attitudeEstimator,
				 const float rollReq, const float pitchReq);
	float ReqInputToAngle(const float input);

	float GetRollRate() const
	{
		return desiredRollRate;
	}

	float GetPitchRate() const
	{
		return desiredPitchRate;
	}

public:
	static constexpr auto LOWER_USER_INPUT = 1000.F;
	static constexpr auto UPPER_USER_INPUT = 2000.F;

	static constexpr auto USER_INPUT_MID =
		0.5F * (LOWER_USER_INPUT + UPPER_USER_INPUT);

private:
	static constexpr auto MAX_ROT_ANGLE_DEG = 50.F;
	static constexpr auto MULTIPLIER =
		2.F * MAX_ROT_ANGLE_DEG / (UPPER_USER_INPUT - LOWER_USER_INPUT);

private:
	PidController pidRoll;
	PidController pidPitch;

	float desiredRollRate = 0.F;
	float desiredPitchRate = 0.F;
};

#endif /* ANGLE_CONTROLLER_H_ */