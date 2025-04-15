#include <MotorManager.h>

MotorManager::MotorManager(const uint16_t escFreqHz, const uint8_t outputPinFR,
						   const uint8_t outputPinRR, const uint8_t outputPinRL,
						   const uint8_t outputPinFL)
	: escFreqHz(escFreqHz)
{
	outputPins[MOT_FR] = outputPinFR;
	outputPins[MOT_RR] = outputPinRR;
	outputPins[MOT_RL] = outputPinRL;
	outputPins[MOT_RR] = outputPinRR;
}

void MotorManager::Init()
{
	analogWriteResolution(12U);
	for (auto i = 0U; i < MOT_NoOf; ++i) {
		analogWriteFrequency(outputPins[i], escFreqHz);
		analogWrite(i, 1.024F * 1000);
	}
}

void MotorManager::Process()
{
	for (auto i = 0U; i < MOT_NoOf; ++i)
		analogWrite(outputPins[i], (int)(1.024F * throttle[i]));
}

void MotorManager::SetThrottle(Motor motor, float throttle)
{
	this->throttle[motor] = max(throttle, 1000.F);
}

void MotorManager::SetThrottle(const float throttle[MOT_NoOf])
{
	for (auto i = 0U; i < MOT_NoOf; ++i)
		SetThrottle(i, throttle[i]);
}