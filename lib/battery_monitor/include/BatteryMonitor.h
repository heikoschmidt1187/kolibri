#ifndef BATTERY_MONITOR_H_
#define BATTERY_MONITOR_H_

#include <Arduino.h>

class BatteryMonitor {
public:
	enum BatteryLevel
	{
		BAT_NORMAL,
		BAT_LOW
	};

public:
	BatteryMonitor(const uint8_t voltageMeasurementPin,
				   const uint8_t currentMeasurementPin,
				   const uint8_t resolutionBits,
				   const uint8_t voltageDividerRatio,
				   const float currentDividerRatio, const float cycleTimeS);

	BatteryLevel Init();
	BatteryLevel Process();

	float MeasureVoltage();
	float MeasureCurrent();

private:
	static constexpr auto TOTAL_BATTERY_CAPACITY = 1300.F;

private:
	uint8_t voltageMeasurementPin;
	uint8_t currentMeasurementPin;

	float voltageDivider;
	float currentDivider;

	float remainingCapacity;
	float capacityAtStart;
	float consumedCapacity;

	float cycleTimeS;

	uint16_t statusLogCycleCtr = 0;
};

#endif /* BATTERY_MONITOR_H_ */
