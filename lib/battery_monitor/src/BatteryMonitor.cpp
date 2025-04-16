#include "BatteryMonitor.h"

#include <DroneConfig.h>

BatteryMonitor::BatteryMonitor(const uint8_t voltageMeasurementPin,
							   const uint8_t currentMeasurementPin,
							   const uint8_t resolutionBits,
							   const uint8_t voltageDividerRatio,
							   const float currentDividerRatio,
							   const float cycleTimeS)
	: voltageMeasurementPin(voltageMeasurementPin)
	, currentMeasurementPin(currentMeasurementPin)
	, consumedCapacity(0.F)
	, cycleTimeS(cycleTimeS)
{
	voltageDivider =
		(powf(2.F, float(resolutionBits)) - 1) / (3.3F * voltageDividerRatio);
	currentDivider =
		((powf(2.F, float(resolutionBits)) - 1) / 3.3F) * currentDividerRatio;
}

BatteryMonitor::BatteryLevel BatteryMonitor::Init()
{
	// during init, motors are off so the voltage is a good indicator for
	// the remaining capacity
	auto voltage = MeasureVoltage();

	Serial.printf("Measured battery voltage: %f V\n", voltage);

	if (voltage < 7.5) {
		// battery has less than 30 %
		capacityAtStart = 30.F / 100.F * TOTAL_BATTERY_CAPACITY;
		return BAT_LOW;
	}

	if (voltage > 8.3F)
		capacityAtStart =
			TOTAL_BATTERY_CAPACITY; // battery considered to be full
	else
		capacityAtStart = (82.F * voltage - 580.F) / 100.F *
						  TOTAL_BATTERY_CAPACITY; // linear dependency between
	// the two thresholds
	//
	return BAT_NORMAL;
}

BatteryMonitor::BatteryLevel BatteryMonitor::Process()
{
	// integrate the measured current
	auto current = MeasureCurrent();
	consumedCapacity =
		current * 1000.F * cycleTimeS / 3600.F + consumedCapacity;
	auto remaining =
		(capacityAtStart - consumedCapacity) / TOTAL_BATTERY_CAPACITY * 100;

	if (statusLogCycleCtr == 0U) {
#ifndef MEASRUEMENT_MODE
		Serial.printf(
			"Measured voltage: %f V | Measured current: %f A | Remaining capacity: %f %\n",
			MeasureVoltage(), current, remaining);
#endif
		statusLogCycleCtr = (uint16_t)(5.F / cycleTimeS);
	}
	statusLogCycleCtr--;

	return remaining < 30.F ? BAT_LOW : BAT_NORMAL;
}

float BatteryMonitor::MeasureVoltage()
{
	return (float)analogRead(voltageMeasurementPin) / voltageDivider;
}

float BatteryMonitor::MeasureCurrent()
{
	return (float)analogRead(currentMeasurementPin) / currentDivider;
}
