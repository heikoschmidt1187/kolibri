#include <Arduino.h>
#include <Wire.h>

#include <BatteryMonitor.h>
#include <Mpu6050.h>
#include <Receiver.h>
#include <MotorManager.h>
#include <RateController.h>
#include <AttitudeEstimator.h>

#include <DroneConfig.h>

BatteryMonitor batteryMonitor(BATTERY_VOLTAGE_INPUT_PIN,
							  BATTERY_CURRENT_INPUT_PIN,
							  ADC_INPUT_RESOLUTION_BITS,
							  BATTERY_VOLTAGE_DIVIDER_RATIO,
							  BATTERY_CURRENT_DIVIDER_RATIO,
							  (float)CYCLE_TIME_MS / 1000.F);

Mpu6050 mpu6050(MCP6050_BUS_ID, ACCELEROMETER_CALIBRATION_OFFSET_X,
				ACCELEROMETER_CALIBRATION_OFFSET_Y,
				ACCELEROMETER_CALIBRATION_OFFSET_Z);

Receiver receiver(RECEIVER_PIN);
MotorManager motorManager(ESC_INPUT_FREQ_HZ, MOTOR_FR_PIN, MOTOR_RR_PIN,
						  MOTOR_RL_PIN, MOTOR_FL_PIN);

RateController rateController((float)CYCLE_TIME_MS / 1000.F);

// battery level handling
static uint16_t batteryLowBlinkCtr = 0U;
static bool batteryLow = false;
static auto batteryLowCurOutput = LOW;

// throttle safety handling
static bool validThrottleCommandSeen = false;

// precision loop timing
static uint32_t loopTimer = 0U;

// receiver disconnect detection
static uint32_t lastReceiverTimestamp = 0U;

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
	static float throttleReq = 0.F;

	static float rollReq = 0.F;
	static float pitchReq = 0.F;
	static float yawReq = 0.F;

	static bool loggedReceiverFallback = false;
	static bool emergencyRoutineActivated = false;

	if (receiver.GetActualChannelCount() >= 8) {
		auto throttle = receiver.GetChannelValue(CHANNEL_THROTTLE);

		// wait until the throttle stick has been moved around it's lower position
		// to avoid unintended rotor movement
		if (!validThrottleCommandSeen && (throttle > 1020.F) &&
			(throttle < 1050.F))
			validThrottleCommandSeen = true;

		throttleReq = validThrottleCommandSeen ? throttle : 0.F;

		rollReq = receiver.GetChannelValue(CHANNEL_ROLL);
		pitchReq = receiver.GetChannelValue(CHANNEL_PITCH);
		yawReq = receiver.GetChannelValue(CHANNEL_YAW);

		// check the emergency switch
		auto emergencyInput =
			receiver.GetChannelValue(CHANNEL_SWITCH_EMERGENCY);

		if ((emergencyInput > RateController::USER_INPUT_MID) &&
			!emergencyRoutineActivated) {
			Serial.printf("Connection lost, emergency activated!\n");
			emergencyRoutineActivated = true;
		}

		lastReceiverTimestamp = micros();
		loggedReceiverFallback = false;

	} else if (validThrottleCommandSeen) {
		// TODO: lost receiver during flight
		// for now: keep turning at idle and hope the best
		if (validThrottleCommandSeen &&
			(micros() - lastReceiverTimestamp > RECVR_DISCONNECT_TIMEOUT_US)) {
			if (!loggedReceiverFallback) {
				Serial.printf("Receiver failed, fallback activated!\n");
				loggedReceiverFallback = true;
			}
		} else {
			loggedReceiverFallback = false;
		}
	} else {
		// nothing to do, wait for commands
	}

	// handle emergency
	if (emergencyRoutineActivated || loggedReceiverFallback) {
		throttleReq = RateController::USER_THROTTLE_IDLE;
		rollReq = RateController::USER_INPUT_MID;
		pitchReq = RateController::USER_INPUT_MID;
		yawReq = RateController::USER_INPUT_MID;
	}

	rateController.Process(mpu6050, motorManager, throttleReq, rollReq,
						   pitchReq, yawReq);
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
	mpu6050.Init();

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

	loopTimer = micros();
	lastReceiverTimestamp = micros();
}

void loop()
{
	HandleBattery();

	// get system input
	receiver.Process();
	mpu6050.Process();

	// logic depends on controller
	HandleRateController();

	// set actuators
	motorManager.Process();

	// cross check if we're still in target
	auto diff = micros() - loopTimer;
	if ((micros() - loopTimer) > CYCLE_TIME_US)
		Serial.printf("Time limit exceeded! %llu us\n", diff);

	// wait until loop is finished
	while (diff < CYCLE_TIME_US)
		diff = micros() - loopTimer;

	loopTimer = micros();
}
