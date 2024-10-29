#include <Arduino.h>

const int rgbLedRedPin = 13;
const int rgbLedGreenPin = 15;
const int redLedBluePin = 3;

int analogRed = 0, analogGreen = 0, analogBlue = 0;

// Convert a given HSV (Hue Saturation Value) to RGB(Red Green Blue) and set the led to the color
//  h is hue value, integer between 0 and 360
//  s is saturation value, double between 0 and 1
//  v is value, double between 0 and 1
void setLedColorHSV(int h, double s, double v) {
  // this is the algorithm to convert from RGB to HSV
  double r = 0;
  double g = 0;
  double b = 0;
 
  double hf = h / 60.0;
 
  int i = (int)floor(h / 60.0);
  double f = h / 60.0 - i;
  double pv = v * (1 - s);
  double qv = v * (1 - s * f);
  double tv = v * (1 - s * (1 - f));
 
  switch (i)
  {
    case 0:
      r = v;
      g = tv;
      b = pv;
      break;
    case 1:
      r = qv;
      g = v;
      b = pv;
      break;
    case 2:
      r = pv;
      g = v;
      b = tv;
      break;
    case 3:
      r = pv;
      g = qv;
      b = v;
      break;
    case 4:
      r = tv;
      g = pv;
      b = v;
      break;
    case 5:
      r = v;
      g = pv;
      b = qv;
      break;
  }
 
  //set each component to a integer value between 0 and 255
  analogRed = constrain((int)255 * r, 0, 255);
  analogGreen = constrain((int)255 * g, 0, 255);
  analogBlue = constrain((int)255 * b, 0, 255);
}

void setup() {
  pinMode(rgbLedRedPin, OUTPUT);
  pinMode(rgbLedGreenPin, OUTPUT);
  pinMode(redLedBluePin, OUTPUT);

  digitalWrite(rgbLedRedPin, LOW);
  digitalWrite(rgbLedGreenPin, HIGH);
  digitalWrite(redLedBluePin, HIGH);
}

void loop() {
  // int waveRed = (3000 - abs(((int)millis() % 6000) - 3000));
  // int waveGreen = (2000 - abs(((int)millis() % 4000) - 2000));
  // int waveBlue = (1000 - abs(((int)millis() % 2000) - 1000));
  
  // analogWrite(rgbLedRedPin, waveRed / 12);
  // analogWrite(rgbLedGreenPin, waveGreen / 8);
  // analogWrite(redLedBluePin, waveBlue / 4);


  setLedColorHSV((millis() / 10) % 360, 1, 1);

  analogWrite(rgbLedRedPin, analogRed);
  analogWrite(rgbLedGreenPin, analogGreen);
  analogWrite(redLedBluePin, analogBlue);
}