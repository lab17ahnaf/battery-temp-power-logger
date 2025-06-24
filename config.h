#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Pin configuration
#define THERMISTOR_1_PIN A0
#define THERMISTOR_2_PIN A1
#define THERMISTOR_3_PIN A2
#define THERMISTOR_4_PIN A3

#define LED_PIN 13

// Thermistor parameters
#define NOMINAL_RESISTANCE 10000.0
#define BETA_COEFFICIENT 3950
#define SERIES_RESISTOR 10000.0

// Thermistor disconnect detection
#define MAX_VALID_ADC 1010
#define MIN_VALID_ADC 10

// EEPROM address to store last boot format
#define EEPROM_BOOT_FLAG_ADDR 0

// Debug mode (enable/disable serial prints)
#define DEBUG_MODE false

#endif
