#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_INA219.h>
#include <thermistor.h>
#include "config.h"

// INA219 instance
Adafruit_INA219 ina219;
bool ina219_ok = false;

// Thermistors
THERMISTOR thermistor1(THERMISTOR_1_PIN, NOMINAL_RESISTANCE, BETA_COEFFICIENT, SERIES_RESISTOR);
THERMISTOR thermistor2(THERMISTOR_2_PIN, NOMINAL_RESISTANCE, BETA_COEFFICIENT, SERIES_RESISTOR);
THERMISTOR thermistor3(THERMISTOR_3_PIN, NOMINAL_RESISTANCE, BETA_COEFFICIENT, SERIES_RESISTOR);
THERMISTOR thermistor4(THERMISTOR_4_PIN, NOMINAL_RESISTANCE, BETA_COEFFICIENT, SERIES_RESISTOR);

// State
float temperatures[4];
bool thermistorConnected[4];
float voltage = 0.0, current_mA = 0.0, power_W = 0.0;
bool useCSVFormat = true;
bool csvHeaderPrinted = false;

bool isThermistorConnected(int analogPin) {
  int value = analogRead(analogPin);
  return (value > MIN_VALID_ADC && value < MAX_VALID_ADC);
}

void readSensors() {
  int pins[] = {THERMISTOR_1_PIN, THERMISTOR_2_PIN, THERMISTOR_3_PIN, THERMISTOR_4_PIN};
  THERMISTOR* sensors[] = {&thermistor1, &thermistor2, &thermistor3, &thermistor4};

  for (int i = 0; i < 4; i++) {
    thermistorConnected[i] = isThermistorConnected(pins[i]);
    temperatures[i] = thermistorConnected[i] ? sensors[i]->read() / 10.0 : -999.0;
  }

  if (ina219_ok) {
    voltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_W = ina219.getPower_mW() / 1000.0;
  } else {
    voltage = current_mA = power_W = -1.0;
  }
}

void printHumanReadable() {
  // if (!DEBUG_MODE) return;

  Serial.print("[DATA] ");
  for (int i = 0; i < 4; i++) {
    Serial.print("Temp ");
    Serial.print(i + 1);
    Serial.print(": ");
    if (thermistorConnected[i]) {
      Serial.print(temperatures[i], 1);
      Serial.print(" °C, ");
    } else {
      Serial.print("NOT CONNECTED, ");
    }
  }

  if (ina219_ok) {
    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.print(" V, Current: ");
    Serial.print(current_mA, 1);
    Serial.print(" mA, Power: ");
    Serial.print(power_W, 2);
    Serial.println(" W");
  } else {
    Serial.println("INA219: NOT CONNECTED");
  }
}

void printCSV() {
  // if (!DEBUG_MODE) return;

  if (!csvHeaderPrinted) {
    Serial.println("Temp1,Temp2,Temp3,Temp4,Voltage,Current,Power");
    csvHeaderPrinted = true;
  }

  for (int i = 0; i < 4; i++) {
    if (thermistorConnected[i]) {
      Serial.print(temperatures[i], 1);
    } else {
      Serial.print("NA");
    }
    Serial.print(",");
  }

  if (ina219_ok) {
    Serial.print(voltage, 2);
    Serial.print(",");
    Serial.print(current_mA, 1);
    Serial.print(",");
    Serial.println(power_W, 2);
  } else {
    Serial.println("NA,NA,NA");
  }
}


void printAllData() {
  if (useCSVFormat) {
    printCSV();
  } else {
    printHumanReadable();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  if (DEBUG_MODE) Serial.println("[BOOT] Initializing system...");

  // INA219
  ina219_ok = ina219.begin();
  if (DEBUG_MODE) {
    Serial.print("[INA219] ");
    Serial.println(ina219_ok ? "Initialized" : "FAILED!");
  }

  // Load and toggle boot format
  byte bootFlag = EEPROM.read(EEPROM_BOOT_FLAG_ADDR);
  useCSVFormat = (bootFlag == 0);  // If 0, use CSV, else HUMAN
  EEPROM.write(EEPROM_BOOT_FLAG_ADDR, useCSVFormat ? 1 : 0); // Toggle for next boot

  if (DEBUG_MODE) {
    Serial.print("[BOOT FORMAT] This boot: ");
    Serial.println(useCSVFormat ? "CSV" : "HUMAN");
  }
}

void loop() {
  readSensors();
  printAllData();
  delay(1000);
}
