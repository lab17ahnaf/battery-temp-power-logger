#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include <Adafruit_INA219.h>
#include <thermistor.h>

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// INA219 setup
// Adafruit_INA219 ina219;

// Thermistor setup
#define THERMISTOR_1_PIN A0
#define THERMISTOR_2_PIN A1
#define THERMISTOR_3_PIN A2
#define THERMISTOR_4_PIN A3
#define NOMINAL_RESISTANCE 10000
#define BETA_COEFFICIENT 3950
#define SERIES_RESISTOR 10000

THERMISTOR thermistor1(THERMISTOR_1_PIN, NOMINAL_RESISTANCE, BETA_COEFFICIENT, SERIES_RESISTOR);
THERMISTOR thermistor2(THERMISTOR_2_PIN, NOMINAL_RESISTANCE, BETA_COEFFICIENT, SERIES_RESISTOR);
THERMISTOR thermistor3(THERMISTOR_3_PIN, NOMINAL_RESISTANCE, BETA_COEFFICIENT, SERIES_RESISTOR);
THERMISTOR thermistor4(THERMISTOR_4_PIN, NOMINAL_RESISTANCE, BETA_COEFFICIENT, SERIES_RESISTOR);

void setup() {
  Serial.begin(115200);
  Serial.println("System setup initializing...");
  Serial.println("System boot successfull!");

  // Initialize OLED
  Serial.println("Display setup initializing...");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    Serial.println("Display initialization failed!");
  else
    Serial.println("Display initialization successfull!");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();


  // Initialize INA219
  // Serial.println("INA219 setup initializing...");
  // if (!ina219.begin())
  //   Serial.println("INA219 initialization failed!");
  // else
  //   Serial.println("INA219 initialization successfull!");

  // Display initialization message
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  // Read INA219 data
  // float busVoltage = ina219.getBusVoltage_V();
  // float current_mA = ina219.getCurrent_mA();
  // float power_mW = ina219.getPower_mW();

  float busVoltage = 0;
  float current_mA = 0;
  float power_mW = 0;

  // Read temperatures
  float temp1C = thermistor1.read() / 10.0;
  float temp2C = thermistor2.read() / 10.0;
  float temp3C = thermistor3.read() / 10.0;
  float temp4C = thermistor4.read() / 10.0;

  // float temp1C = 10.0;
  // float temp2C = 10.0;
  // float temp3C = 10.0;
  // float temp4C = 10.0;

  // Print data to Serial Monitor (optional for debugging)
  Serial.print("Temp1: ");
  Serial.print(temp1C);
  Serial.print(" Temp2: ");
  Serial.print(temp2C);
  Serial.print(" Temp3: ");
  Serial.print(temp3C);
  Serial.print(" Temp4: ");
  Serial.print(temp4C);
  Serial.print(" Voltage: ");
  Serial.print(busVoltage);
  Serial.print(" V, Current: ");
  Serial.print(current_mA);
  Serial.print(" mA, Power: ");
  Serial.print(power_mW / 1000.0);
  Serial.println(" W");

  // Display data on OLED
  display.clearDisplay();

  // Display temperatures
  display.setCursor(0, 0);
  display.print("T1: ");
  display.print(temp1C, 1);
  display.print("C T2: ");
  display.print(temp2C, 1);
  display.println("C");

  display.print("T3: ");
  display.print(temp3C, 1);
  display.print("C T4: ");
  display.print(temp4C, 1);
  display.println("C");

  display.println();

  // Display INA219 data
  display.print("V: ");
  display.print(busVoltage, 2);
  display.println(" V");

  display.print("I: ");
  display.print(current_mA, 1);
  display.println(" mA");

  display.print("P: ");
  display.print(power_mW / 1000.0, 2);  // Power in W
  display.println(" W");

  display.display();

  delay(1000);  // Update every second
}