#include <Arduino.h>

#define INTERNAL_LED_GPIO 13U
#define RED_LED_GPIO 5U
#define GREE_LED_GPIO 6U

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

    // TODO: perform initialization
    delay(4000U);

    // signal end of setup
    digitalWrite(RED_LED_GPIO, LOW);
    digitalWrite(GREE_LED_GPIO, HIGH);
}

void loop()
{
}
