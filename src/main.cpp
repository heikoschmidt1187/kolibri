#include <Arduino.h>
#include <Wire.h>

#include <BatteryMonitor.h>
#include <Gyroscope.h>
#include <Receiver.h>

#define MAX_RECEIVER_CHANNELS 10U
#define RECEIVER_PIN 14U

#define INTERNAL_LED_GPIO 13U
#define RED_LED_GPIO 5U
#define GREEN_LED_GPIO 6U

#define BATTERY_VOLTAGE_INPUT_PIN 15U
#define BATTERY_VOLTAGE_INPUT_RESOLUTION_BITS 10U
#define BATTERY_VOLTAGE_DIVIDER_RATIO 5U

#define I2C_FREQ_IN_HZ 400000U

#define SERIAL_BAUDRATE 57600L
#define MCP6050_BUS_ID 0x68U

#define VERSION "0.1"

BatteryMonitor batteryMonitor(BATTERY_VOLTAGE_INPUT_PIN,
                              BATTERY_VOLTAGE_INPUT_RESOLUTION_BITS,
                              BATTERY_VOLTAGE_DIVIDER_RATIO);
Gyroscope gyroscope(MCP6050_BUS_ID);
Receiver receiver(RECEIVER_PIN);

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

    // init Gyro
    gyroscope.Init();

    // init receiver
    receiver.Init();

    // signal end of setup
    digitalWrite(RED_LED_GPIO, LOW);
    digitalWrite(GREEN_LED_GPIO, HIGH);
}

void loop()
{
    // check voltage
    auto voltage = batteryMonitor.process();
    Serial.printf("Battery Voltage: %f\n", voltage);

    // check rates
    gyroscope.Process();
    Serial.printf("Roll-Rate: %f °/s | Pitch-Rate: %f °/s | Yaw-Rate: %f °/s\n",
                  gyroscope.GetRollRate(), gyroscope.GetPitchRate(), gyroscope.GetYawRate());

    // check channel values
    // receiver.Process();
    // for (auto i = 0; i < receiver.GetActualChannelCount(); ++i)
    // Serial.printf("Channel %d: %f\n", i, receiver.GetChannelValue(i));

    delay(50);
}
