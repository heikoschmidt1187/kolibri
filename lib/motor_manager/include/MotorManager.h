#ifndef MOTOR_MANAGER_H_
#define MOTOR_MANAGER_H_

#include <Arduino.h>

class MotorManager {
public:
	enum Motor : uint8_t
	{
		MOT_FR = 0U,
		MOT_RR,
		MOT_RL,
		MOT_FL,
		MOT_NoOf
	};

public:
	MotorManager(const uint16_t escFreqHz, const uint8_t outputPinFR,
				 const uint8_t outputPinRR, const uint8_t outputPinRL,
				 const uint8_t outputPinFL);
	void Init();
	void Process();

	void SetThrottle(Motor motor, float throttle);

private:
	uint16_t escFreqHz;
	uint8_t outputPins[MOT_NoOf];
	float throttle[MOT_NoOf] = { 0.F };
};

#endif /* MOTOR_MANAGER_H_ */