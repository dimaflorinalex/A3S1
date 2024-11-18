#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <SPI.h>

// Arduino UNO
const uint32_t clockFrequency = 16000000L;
const uint32_t baudRate = 115200;

// LCD
const int rsPin = 9, enPin = 8, d4Pin = 5, d5Pin = 4, d6Pin = 3, d7Pin = 2;
LiquidCrystal lcd(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin);
const int lcdRows = 2, lcdColumns = 16;

// Servo
const int servoPin = 6;
Servo servo;
int servoPos = 0;

// Game logic
const unsigned long playerTurnTimeMilliseconds = 3000;
const unsigned long roundTimeMilliseconds = 30000;
unsigned long roundStartTimeMilliseconds = 0;

int player1Score = 0;
int player2Score = 0;

bool shouldShowMainView = true;
bool shouldStartGame = false;
bool shouldStopGame = false;
bool isGameStarted = false;
int currentPlayer = 0;

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

void showMainView() {
  shouldShowMainView = false;

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Press any button");

  lcd.setCursor(0, 1);
  lcd.print("to start playing");

  servo.write(0);
}

void stopGame() {
  shouldStopGame = false;
  isGameStarted = false;

  int winnerPlayer = 0;
  int winnerScore = 0;

  if (player1Score > player2Score) {
    winnerPlayer = 1;
    winnerScore = player1Score;
  }
  else if (player1Score < player2Score) {
    winnerPlayer = 2;
    winnerScore = player2Score;
  }
  else {
    winnerPlayer = 0;
    winnerScore = player1Score;
  }

  lcd.clear();

  lcd.setCursor(0, 0);
  if (winnerPlayer == 0) {
    lcd.print("It's a draw!");
  }
  else if (winnerPlayer == 1) {
    lcd.print("Player 1 has won");
  }
  else if (winnerPlayer == 2) {
    lcd.print("Player 2 has won");
  }
  
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(winnerScore);

  delay(10000);

  shouldShowMainView = true;
}

void updateGameView() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Player 1: ");
  lcd.print(player1Score);

  lcd.setCursor(0, 1);
  lcd.print("Player 2: ");
  lcd.print(player2Score);
}

void startGame() {
  Serial.println("Starting game...");
  shouldStartGame = false;

  player1Score = 0;
  player2Score = 0;

  updateGameView();

  currentPlayer = 0;
  roundStartTimeMilliseconds = millis();

  isGameStarted = true;
  Serial.println("Started game");
}

bool anyButtonsPressed(Events event) {
  return event == Events::Button1Pressed
      || event == Events::Button2Pressed
      || event == Events::Button3Pressed
      || event == Events::Button4Pressed
      || event == Events::Button5Pressed
      || event == Events::Button6Pressed;
}

void handleSlave() {
  if (!isGameStarted) {
    digitalWrite(SS, LOW);
    SPI.transfer(Events::MasterRequestsInputFromSlave);
    delay(100);
    byte response = SPI.transfer(Events::Dummy);
    digitalWrite(SS, HIGH);

    Serial.print("Received: ");
    Serial.print(response);
    Serial.println("");

    if (anyButtonsPressed((Events)response)) {
      Serial.println("Should start game");
      shouldStartGame = true;
    }
  }
  else {
    if (shouldStopGame) {
      digitalWrite(SS, LOW);
      SPI.transfer(Events::MasterRequestsStopGame);
      digitalWrite(SS, HIGH);

      stopGame();
      return;
    }

    if (currentPlayer == 1) {
      Events randomPlayer1LedEvent = (Events)random(CurrentPlayer1Red, CurrentPlayer1Blue + 1);

      digitalWrite(SS, LOW);
      SPI.transfer(randomPlayer1LedEvent);

      long currentMillis = millis();
      byte response = 0;

      while (millis() - currentMillis <= playerTurnTimeMilliseconds) {
        long millisecondsLeft = (roundStartTimeMilliseconds + roundTimeMilliseconds) - millis();
        if (millisecondsLeft > 0) {
          int servoPos = min(round((double)millisecondsLeft / roundTimeMilliseconds * 180), 180);
          servo.write(servoPos);
        }
        
        SPI.transfer(Events::MasterRequestsInputFromSlave);
        delay(100);
        response = SPI.transfer(Events::Dummy);

        Serial.print("Received: ");
        Serial.print(response);
        Serial.println("");

        if ((randomPlayer1LedEvent == Events::CurrentPlayer1Red && response == Events::Button1Pressed)
         || (randomPlayer1LedEvent == Events::CurrentPlayer1Green && response == Events::Button2Pressed)
         || (randomPlayer1LedEvent == Events::CurrentPlayer1Blue && response == Events::Button3Pressed)) {
          player1Score++;
          updateGameView();
          break;
        }
        delay(100);
      }

      digitalWrite(SS, HIGH);
      currentPlayer = 2;
    }
    else if (currentPlayer == 2) {
      Events randomPlayer2LedEvent = (Events)random(CurrentPlayer2Red, CurrentPlayer2Blue + 1);

      digitalWrite(SS, LOW);
      SPI.transfer(randomPlayer2LedEvent);

      long currentMillis = millis();
      byte response = 0;

      while (millis() - currentMillis <= playerTurnTimeMilliseconds) {
        long millisecondsLeft = (roundStartTimeMilliseconds + roundTimeMilliseconds) - millis();
        if (millisecondsLeft > 0) {
          int servoPos = min(round((double)millisecondsLeft / roundTimeMilliseconds * 180), 180);
          servo.write(servoPos);
        }

        SPI.transfer(Events::MasterRequestsInputFromSlave);
        delay(100);
        response = SPI.transfer(Events::Dummy);

        Serial.print("Received: ");
        Serial.print(response);
        Serial.println("");

        if ((randomPlayer2LedEvent == Events::CurrentPlayer2Red && response == Events::Button4Pressed)
         || (randomPlayer2LedEvent == Events::CurrentPlayer2Green && response == Events::Button5Pressed)
         || (randomPlayer2LedEvent == Events::CurrentPlayer2Blue && response == Events::Button6Pressed)) {
          player2Score++;
          updateGameView();
          break;
        }
        delay(100);
      }

      digitalWrite(SS, HIGH);
      currentPlayer = 1;
    }
    else {
      currentPlayer = random(1, 3);
    }
  }
}

void setup() {
  randomSeed(analogRead(0));

  SPI.begin();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  lcd.begin(lcdColumns, lcdRows);
  lcd.clear();
  
  servo.attach(servoPin);
  
  shouldShowMainView = true;

  // Testing only
  Serial.begin(baudRate);
  Serial.println("Master");
}

void loop() {
  if (shouldShowMainView) {
    showMainView();
  }
  
  if (shouldStartGame) {
    startGame();
  }

  if (isGameStarted) {
    long millisecondsLeft = (roundStartTimeMilliseconds + roundTimeMilliseconds) - millis();
    if (millisecondsLeft > 0) {
      int servoPos = min(round((double)millisecondsLeft / roundTimeMilliseconds * 180), 180);
      servo.write(servoPos);
    }

    if (millis() - roundStartTimeMilliseconds > roundTimeMilliseconds) {
      shouldStopGame = true;
    }
  }
  
  handleSlave();
}