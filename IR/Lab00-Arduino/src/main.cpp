#include <Arduino.h> //header ce conține toate definițiile pentru API
#define RED_LED 13 // este de preferat în continuare să folosim macrouri, nu hardcodăm valori!
#define GREEN_LED 15
#define BLUE_LED 3
#define BTN_1 2
#define BTN_2 14

void aprindeLEDVerde() {
  digitalWrite(GREEN_LED, LOW);
}

void stingeLEDVerde() {
  digitalWrite(GREEN_LED, HIGH);
}

void aprindeLEDRosu() {
  digitalWrite(RED_LED, LOW);
}

void stingeLEDRosu() {
  digitalWrite(RED_LED, HIGH);
}

void aprindeLEDAlbastru() {
  digitalWrite(BLUE_LED, LOW);
}

void stingeLEDAlbastru() {
  digitalWrite(BLUE_LED, HIGH);
}

void seteazaLEDOutput() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}

void seteazaLEDInput() {
  pinMode(RED_LED, INPUT);
  pinMode(GREEN_LED, INPUT);
  pinMode(BLUE_LED, INPUT);
}

void seteazaButoaneInput() {
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
}

bool citesteButon1() {
  return digitalRead(BTN_1);
}

bool citesteButon2() {
  return digitalRead(BTN_2);
}

void setup() {
  seteazaLEDInput();

  stingeLEDRosu();
  stingeLEDVerde();
  aprindeLEDAlbastru();

  seteazaButoaneInput();
}

int button1Counter = 0;
bool button1IsNotPressedLast = 0, button1IsNotPressed = 0;

int button2Counter = 0;
bool button2IsNotPressedLast = 0, button2IsNotPressed = 0;

void loop() {
    button1IsNotPressedLast = button1IsNotPressed;
    button1IsNotPressed = citesteButon1();

    button2IsNotPressedLast = button2IsNotPressed;
    button2IsNotPressed = citesteButon2();

    if (button2IsNotPressedLast && !button2IsNotPressed) {
      button2Counter++;
    }

    if (button2Counter % 2 == 0) {
      seteazaLEDOutput();
    }
    else {
      seteazaLEDInput();
      _delay_ms(200);
      seteazaLEDOutput();
    }

    if (!button1IsNotPressed && button1IsNotPressedLast) {
      if (button1Counter % 3 == 0) {
        stingeLEDVerde();
        stingeLEDAlbastru();
        aprindeLEDRosu();
      }
      else if (button1Counter % 3 == 1) {
        stingeLEDRosu();
        stingeLEDAlbastru();
        aprindeLEDVerde();
      }
      else if (button1Counter % 3 == 2) {
        stingeLEDRosu();
        stingeLEDVerde();
        aprindeLEDAlbastru();
      }

      button1Counter++;
    }

  delay(500);
}