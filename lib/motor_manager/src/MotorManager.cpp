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
	for (auto i = 0U; i < MOT_NoOf; ++i)
		analogWriteFrequency(outputPins[i], escFreqHz);
}