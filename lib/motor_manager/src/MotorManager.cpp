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

	for (auto i = 0U; i < MOT_NoOf; ++i) {
#ifdef MEASUREMENT_MODE
		Serial.printf("Motor %d: %f | ", i, throttle[i]);
#endif

#if !defined(MEASUREMENT_MODE) || !defined(MEASUREMENT_NO_MOTOR_OUTPUT)
		analogWrite(outputPins[i], (int)(1.024F * throttle[i]));
#endif
	}
#ifdef MEASUREMENT_MODE
	Serial.printf("\n");
#endif
}

void MotorManager::SetThrottle(Motor motor, float throttle)
{
	this->throttle[motor] = max(throttle, THROTTLE_OFF);

#if defined(MEASUREMENT_MODE) && defined(MEASUREMENT_LIMIT_THROTTLE)
	this->throttle[motor] = min(this->throttle[motor], 1180.F);
#endif
}

void MotorManager::SetThrottle(const float throttle[MOT_NoOf])
{
	for (auto i = 0U; i < MOT_NoOf; ++i)
		SetThrottle((Motor)i, throttle[i]);
}