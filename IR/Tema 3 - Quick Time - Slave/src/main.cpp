#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

const uint32_t baudRate = 115200;

const int buttonAnalogValues[] = { 996, 970, 941, 917, 906, 895 }; // Analog values corresponding to buttons
const int errorAnalogValue = 10;

const int rgbLedPinsPlayer1[] = { 7, 6, 5 }; // Player 1 RGB LED Pins
const int rgbLedPinsPlayer2[] = { 4, 3, 2 }; // Player 2 RGB LED Pins

enum Events {
  Empty = 0,
  Button1Pressed = 1,
  Button2Pressed = 2,
  Button3Pressed = 3,
  Button4Pressed = 4,
  Button5Pressed = 5,
  Button6Pressed = 6,
  NoButtonPressed = 7,
  MasterRequestsInputFromSlave = 8,
  CurrentPlayer1Red = 9,
  CurrentPlayer1Green = 10,
  CurrentPlayer1Blue = 11,
  CurrentPlayer2Red = 12,
  CurrentPlayer2Green = 13,
  CurrentPlayer2Blue = 14,
  MasterRequestsStopGame = 15,
  Dummy = 255
};

bool isButtonPressed(int readAnalogValue, int buttonAnalogValue) {
  return abs(readAnalogValue - buttonAnalogValue) <= errorAnalogValue;
}

void setRGBLed(const int rgbLedPins[3], int red, int green, int blue) {
  digitalWrite(rgbLedPins[0], red);
  digitalWrite(rgbLedPins[1], green);
  digitalWrite(rgbLedPins[2], blue);
}

ISR(SPI_STC_vect) { 
  byte data = SPDR;

  Serial.print("Received: ");
  Serial.print(data);
  Serial.println("");

  if (data == Events::MasterRequestsInputFromSlave) {
    Serial.println("Master requested input from Slave");
    int buttonAnalogValue = analogRead(A0);

    if (isButtonPressed(buttonAnalogValue, buttonAnalogValues[0])) {
      Serial.println("Button 1 pressed");
      SPDR = Events::Button1Pressed;
    }
    else if (isButtonPressed(buttonAnalogValue, buttonAnalogValues[1])) {
      Serial.println("Button 2 pressed");
      SPDR = Events::Button2Pressed;
    }
    else if (isButtonPressed(buttonAnalogValue, buttonAnalogValues[2])) {
      Serial.println("Button 3 pressed");
      SPDR = Events::Button3Pressed;
    }
    else if (isButtonPressed(buttonAnalogValue, buttonAnalogValues[3])) {
      Serial.println("Button 4 pressed");
      SPDR = Events::Button4Pressed;
    }
    else if (isButtonPressed(buttonAnalogValue, buttonAnalogValues[4])) {
      Serial.println("Button 5 pressed");
      SPDR = Events::Button5Pressed;
    }
    else if (isButtonPressed(buttonAnalogValue, buttonAnalogValues[5])) {
      Serial.println("Button 6 pressed");
      SPDR = Events::Button6Pressed;
    }
    else {
      Serial.println("No button pressed");
      SPDR = Events::NoButtonPressed;
    }
  }
  else if (data == Events::CurrentPlayer1Red) {
    setRGBLed(rgbLedPinsPlayer1, HIGH, LOW, LOW);
    setRGBLed(rgbLedPinsPlayer2, LOW, LOW, LOW);
  }
  else if (data == Events::CurrentPlayer1Green) {
    setRGBLed(rgbLedPinsPlayer1, LOW, HIGH, LOW);
    setRGBLed(rgbLedPinsPlayer2, LOW, LOW, LOW);
  }
  else if (data == Events::CurrentPlayer1Blue) {
    setRGBLed(rgbLedPinsPlayer1, LOW, LOW, HIGH);
    setRGBLed(rgbLedPinsPlayer2, LOW, LOW, LOW);
  }
  else if (data == Events::CurrentPlayer2Red) {
    setRGBLed(rgbLedPinsPlayer1, LOW, LOW, LOW);
    setRGBLed(rgbLedPinsPlayer2, HIGH, LOW, LOW);
  }
  else if (data == Events::CurrentPlayer2Green) {
    setRGBLed(rgbLedPinsPlayer1, LOW, LOW, LOW);
    setRGBLed(rgbLedPinsPlayer2, LOW, HIGH, LOW);
  }
  else if (data == Events::CurrentPlayer2Blue) {
    setRGBLed(rgbLedPinsPlayer1, LOW, LOW, LOW);
    setRGBLed(rgbLedPinsPlayer2, LOW, LOW, HIGH);
  }
  else if (data == Events::MasterRequestsStopGame) {
    setRGBLed(rgbLedPinsPlayer1, LOW, LOW, LOW);
    setRGBLed(rgbLedPinsPlayer2, LOW, LOW, LOW);
  }

  Serial.print("Sent: ");
  Serial.print(SPDR);
  Serial.println("");
}

void debugButtonAnalogValues() {
  Serial.println(analogRead(A0)); // Read analog values for buttons
  delay(200);
}

void setup() { 
  pinMode(SS, INPUT_PULLUP);
  pinMode(MISO, OUTPUT);
  pinMode(SCK, INPUT);
  SPCR |= _BV(SPE);
  SPI.attachInterrupt(); //allows SPI interrupt

  for (int i = 0; i < 3; i++) {
    pinMode(rgbLedPinsPlayer1[i], OUTPUT);
    pinMode(rgbLedPinsPlayer2[i], OUTPUT);
  }

  setRGBLed(rgbLedPinsPlayer1, LOW, LOW, LOW);
  setRGBLed(rgbLedPinsPlayer2, LOW, LOW, LOW);

  // Testing only
  Serial.begin(baudRate);
  Serial.println("Slave");
}

void loop() {
  // debugButtonAnalogValues();
}