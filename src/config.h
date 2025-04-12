#ifndef CONFIG_H_
#define CONFIG_H_

// common
#define VERSION "0.1"

#define CYCLE_TIME_MS 4U

// on-board LEDs
#define INTERNAL_LED_GPIO 13U
#define RED_LED_GPIO 5U
#define GREEN_LED_GPIO 6U

// controller receiver
#define MAX_RECEIVER_CHANNELS 10U
#define RECEIVER_PIN 14U

// voltage measurement
#define ADC_INPUT_RESOLUTION_BITS 10U
#define BATTERY_VOLTAGE_INPUT_PIN 15U

#define BATTERY_VOLTAGE_DIVIDER_RATIO 5U
#define BATTERY_CURRENT_DIVIDER_RATIO 0.036F

// current measurement
#define BATTERY_CURRENT_INPUT_PIN 21U

// I2C config
#define I2C_FREQ_IN_HZ 400000U
#define MCP6050_BUS_ID 0x68U

// UART config
#define SERIAL_BAUDRATE 57600L

#endif /* CONFIG_H_ */