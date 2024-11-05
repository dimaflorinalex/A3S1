#include <Arduino.h>

const int temperatureSensorPin = A0;

const float voltageReference = 1.1f; // Internal Reference Voltage is 1.1V on ATmega328P
const float milivoltsPerCelsiusDegree = 19.5f;

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL); // Internal Reference Voltage is 1.1V on ATmega328P
}

void loop() {
  int temperatureSensorValue = analogRead(temperatureSensorPin);

  double volts = voltageReference * temperatureSensorValue / 1024;
  double millivolts = volts * 1000;

  double temperatureCelsius = millivolts / milivoltsPerCelsiusDegree; 

  Serial.println(temperatureCelsius);

  delay(200);
}