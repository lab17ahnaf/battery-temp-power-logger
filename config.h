#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Adafruit_INA219.h>

#define SSD1306_I2C_ADDRESS 0x3C


// Pin configuration
#define THERMISTOR_1_PIN A0
#define THERMISTOR_2_PIN A2
#define THERMISTOR_3_PIN A1
#define THERMISTOR_4_PIN A3

#define LED_PIN 13 // LED pin

// INA219 sample rate (samples per second)
#define INA219_SAMPLE_RATE 100

// Thermistor parameters
#define NOMINAL_RESISTANCE 10000.0
#define NOMINAL_TEMPERATURE 25.0
#define BETA_COEFFICIENT 3950
#define SERIES_RESISTOR 10000.0

// Data structure to hold sensor readings
struct DataSet {
    float voltage_mv = 0;
    float current_mA = 0;
    float power_mw = 0;
};

#endif