#include <Arduino.h>

#include <BatteryMonitor.h>

#define INTERNAL_LED_GPIO 13U
#define RED_LED_GPIO 5U
#define GREE_LED_GPIO 6U

#define BATTERY_VOLTAGE_INPUT_PIN 15U
#define BATTERY_VOLTAGE_INPUT_RESOLUTION_BITS 10U
#define BATTERY_VOLTAGE_DIVIDER_RATIO 5U

#define SERIAL_BAUDRATE 57600L

BatteryMonitor batteryMonitor(BATTERY_VOLTAGE_INPUT_PIN,
                              BATTERY_VOLTAGE_INPUT_RESOLUTION_BITS,
                              BATTERY_VOLTAGE_DIVIDER_RATIO);

void setup()
{
    // configure LED pins
    pinMode(INTERNAL_LED_GPIO, OUTPUT);
    pinMode(RED_LED_GPIO, OUTPUT);
    pinMode(GREE_LED_GPIO, OUTPUT);

    // switch on activity LED and signal start of setup
    digitalWrite(INTERNAL_LED_GPIO, HIGH);
    digitalWrite(RED_LED_GPIO, HIGH);
    digitalWrite(GREE_LED_GPIO, LOW);

    // configure serial monitor
    Serial.begin(SERIAL_BAUDRATE);

    // TODO: perform initialization
    delay(4000U);

    // signal end of setup
    digitalWrite(RED_LED_GPIO, LOW);
    digitalWrite(GREE_LED_GPIO, HIGH);
}

void loop()
{
    auto voltage = batteryMonitor.process();
    Serial.printf("Battery Voltage: %f\n", voltage);
    delay(50U);
}
