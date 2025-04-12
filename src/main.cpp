#include <Arduino.h>
#include <Wire.h>

#include <BatteryMonitor.h>
#include <Gyroscope.h>
#include <Receiver.h>
#include <MotorManager.h>
#include <RateController.h>

#include <config.h>

BatteryMonitor batteryMonitor(BATTERY_VOLTAGE_INPUT_PIN,
							  BATTERY_CURRENT_INPUT_PIN,
							  ADC_INPUT_RESOLUTION_BITS,
							  BATTERY_VOLTAGE_DIVIDER_RATIO,
							  BATTERY_CURRENT_DIVIDER_RATIO,
							  (float)CYCLE_TIME_MS / 1000.F);

Gyroscope gyroscope(MCP6050_BUS_ID);
Receiver receiver(RECEIVER_PIN);
MotorManager motorManager(ESC_INPUT_FREQ_HZ, MOTOR_FR_PIN, MOTOR_RR_PIN,
						  MOTOR_RL_PIN, MOTOR_FL_PIN);

RateController rateController;

// battery level handling
static uint16_t batteryLowBlinkCtr = 0U;
static bool batteryLow = false;
static auto batteryLowCurOutput = LOW;

// throttle safety handling
static bool validThrottleCommandSeen = false;

void HandleBattery()
{
	// check battery and visualize
	auto level = batteryMonitor.Process();
	if (batteryLow) {
		if (batteryLowBlinkCtr == 0U) {
			batteryLowBlinkCtr = 500U / CYCLE_TIME_MS;
			batteryLowCurOutput = batteryLowCurOutput == HIGH ? LOW : HIGH;
		}
		batteryLowBlinkCtr--;
		digitalWrite(RED_LED_GPIO, batteryLowCurOutput);
	} else {
		batteryLow = (level == BatteryMonitor::BAT_LOW);
	}
}

void HandleRateController()
{
	// determine user inputs with throttle safety
	float throttleReq = 0.F;

	float rollReq = 0.F;
	float pitchReq = 0.F;
	float yawReq = 0.F;

	if (receiver.GetActualChannelCount() >= 4) {
		auto throttle = receiver.GetChannelValue(2U);

		// wait until the throttle stick has been moved around it's lower position
		// to avoid unintended rotor movement
		if (!validThrottleCommandSeen && (throttle > 1020.F) &&
			(throttle < 1050.F))
			validThrottleCommandSeen = true;

		throttleReq = validThrottleCommandSeen ? throttle : 0.F;

		rollReq = receiver.GetChannelValue(0U);
		pitchReq = receiver.GetChannelValue(1U);
		yawReq = receiver.GetChannelValue(3U);

	} else {
		// TODO: lost connection to controller, activate safety routine
	}

	rateController.Process(throttleReq, rollReq, pitchReq, yawReq);
}

void setup()
{
	// configure serial monitor
	Serial.begin(SERIAL_BAUDRATE);
	Serial.println("Welcome to Kolibri v" VERSION);
	Serial.println("=======================");

	// configure LED pins
	pinMode(INTERNAL_LED_GPIO, OUTPUT);
	pinMode(RED_LED_GPIO, OUTPUT);
	pinMode(GREEN_LED_GPIO, OUTPUT);

	// switch on activity LED and signal start of setup
	digitalWrite(INTERNAL_LED_GPIO, HIGH);
	digitalWrite(RED_LED_GPIO, HIGH);
	digitalWrite(GREEN_LED_GPIO, LOW);

	// configure I2C and start bus
	Wire.setClock(I2C_FREQ_IN_HZ);
	Wire.begin();
	delay(250U);

	// init gyro
	gyroscope.Init();

	// init receiver
	receiver.Init();

	// init battery monitor
	if (batteryMonitor.Init() == BatteryMonitor::BatteryLevel::BAT_LOW)
		batteryLow = true;

	// init motor manager
	motorManager.Init();

	// init rate controller
	rateController.Init();

	// signal end of setup
	if (!batteryLow)
		digitalWrite(RED_LED_GPIO, LOW);

	digitalWrite(GREEN_LED_GPIO, HIGH);
}

void loop()
{
	HandleBattery();

	// get system input
	receiver.Process();
	gyroscope.Process();

	// logic depends on controller
	HandleRateController();

	// set actuators
	motorManager.Process();

	// TODO: use cycle taking calculation into consideration
	delay(100);
}
