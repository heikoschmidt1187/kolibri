#include <AngleController.h>
#include <DroneConfig.h>

AngleController::AngleController(const float dt)
	: pidRoll(PidController::Config{ .Kp = 2.F, .Ki = 0.F, .Kd = 0.F }, dt)
	, pidPitch(PidController::Config{ .Kp = 2.F, .Ki = 0.F, .Kd = 0.F }, dt)
{
}

void AngleController::Init()
{
}

void AngleController::Process(const AttitudeEstimator &attitudeEstimator,
							  const float rollReq, const float pitchReq)
{
	// transform the user input into rotation angles, limit them for easier control
	float rollAngle = ReqInputToAngle(rollReq);
	float pitchAngle = ReqInputToAngle(pitchReq);

#ifdef MEASUREMENT_MODE
	Serial.printf("Angle Controller: ReqRoll %f | ReqPitch %f\n", rollAngle,
				  pitchAngle);
#endif

	// determine error according to input measurements
	float errorRollAngle = rollAngle - attitudeEstimator.GetRoll();
	float errorPitchAngle = pitchAngle - attitudeEstimator.GetPitch();

	desiredRollRate = pidRoll.Update(errorRollAngle);
	desiredPitchRate = pidPitch.Update(errorPitchAngle);

#ifdef MEASUREMENT_MODE
	Serial.printf(
		"Angle Controller: DesiredRollRate %f | DesiredPitchRate %f\n",
		desiredRollRate, desiredPitchRate);
#endif
}

float AngleController::ReqInputToAngle(const float input)
{
	auto angle = MULTIPLIER * (input - USER_INPUT_MID);

	// clamp the rate
	angle = min(max(angle, -MAX_ROT_ANGLE_DEG), MAX_ROT_ANGLE_DEG);

	return angle;
}