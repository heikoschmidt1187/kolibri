#include <MotorManager.h>

#include <DroneConfig.h>

MotorManager::MotorManager(const uint16_t escFreqHz, const uint8_t outputPinFR,
						   const uint8_t outputPinRR, const uint8_t outputPinRL,
						   const uint8_t outputPinFL)
	: escFreqHz(escFreqHz)
{
	outputPins[MOT_FR] = outputPinFR;
	outputPins[MOT_RR] = outputPinRR;
	outputPins[MOT_RL] = outputPinRL;
	outputPins[MOT_FL] = outputPinFL;
}

void MotorManager::Init()
{
	analogWriteResolution(12U);
	for (auto i = 0U; i < MOT_NoOf; ++i) {
		analogWriteFrequency(outputPins[i], escFreqHz);
		analogWrite(i, 1.024F * THROTTLE_OFF);
	}
}

void MotorManager::Process()
{
#ifdef MEASUREMENT_MODE
	Serial.printf("Throttle: %f\n ", throttle[0]);
#endif

	for (auto i = 0U; i < MOT_NoOf; ++i)
		analogWrite(outputPins[i], (int)(1.024F * throttle[i]));
}

void MotorManager::SetThrottle(Motor motor, float throttle)
{
	this->throttle[motor] = max(throttle, THROTTLE_OFF);

#ifdef MEASUREMENT_MODE
	this->throttle[motor] = min(this->throttle[motor], 1180.F);
#endif
}

void MotorManager::SetThrottle(const float throttle[MOT_NoOf])
{
	for (auto i = 0U; i < MOT_NoOf; ++i)
		SetThrottle(i, throttle[i]);
}