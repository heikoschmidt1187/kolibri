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
	void SetThrottle(const float throttle[MOT_NoOf]);

private:
	static constexpr auto THROTTLE_OFF = 1000.F;

private:
	uint16_t escFreqHz;
	uint8_t outputPins[MOT_NoOf];
	float throttle[MOT_NoOf] = { THROTTLE_OFF };
};

#endif /* MOTOR_MANAGER_H_ */