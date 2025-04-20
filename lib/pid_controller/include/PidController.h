#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

#include <Arduino.h>

class PidController {
public:
	struct Config {
		float Kp;
		float Ki;
		float Kd;
	};

public:
	PidController(const Config cfg, const float dt);

	float Update(const float error);
	void Reset();

private:
	const Config config;
	const float dt;

	float prevError = 0.F;
	float integral = 0.F;
};

#endif /* PID_CONTROLLER_H_ */