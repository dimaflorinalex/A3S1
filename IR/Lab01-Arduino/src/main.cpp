#include <Arduino.h>

#define RED_LED 13
#define GREEN_LED 15
#define BLUE_LED 3

#define BTN_1 2
#define BTN_2 14

void setLEDState(char redState, char greenState, char blueState) {
  digitalWrite(RED_LED, redState);
  digitalWrite(GREEN_LED, greenState);
  digitalWrite(BLUE_LED, blueState);
}

void setup() {
  Serial.begin(28800);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  // pinMode(BTN_1, INPUT_PULLUP);
  // pinMode(BTN_2, INPUT_PULLUP);
}

void loop() {
  if (Serial.available()) {
    // char x = Serial.read();
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equals("red")) {
      setLEDState(LOW, HIGH, HIGH);
    }
    else if (command.equals("green")) {
      setLEDState(HIGH, LOW, HIGH);
    }
    else if (command.equals("blue")) {
      setLEDState(HIGH, HIGH, LOW);
    }
    else if (command.equals("on")) {
      setLEDState(LOW, LOW, LOW);
    }
    else if (command.equals("off")) {
      setLEDState(HIGH, HIGH, HIGH);
    }
  }
}