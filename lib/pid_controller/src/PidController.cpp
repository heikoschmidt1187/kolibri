#include <PidController.h>

PidController::PidController(const Config cfg, const float dt)
	: config(cfg)
	, dt(dt)
{
}

float PidController::Update(const float error)
{
	float pTerm = config.Kp * error;
	float iTerm = integral + config.Ki * 0.5F * (error + prevError) * dt;
	float dTerm = config.Kd * (error - prevError) / dt;

	// avoid unintended runaway of I and D components, e.g. when target cannot be reached because
	// drone is still on ground
	iTerm = min(max(iTerm, -400.F), 400.F);
	dTerm = min(max(dTerm, -400.F), 400.F);

	integral = iTerm;
	prevError = error;

	return pTerm + iTerm + dTerm;
}

void PidController::Reset()
{
	prevError = 0.F;
	integral = 0.F;
}