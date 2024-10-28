#include <Arduino.h>

const int rgbLedRedPin = 6;
const int rgbLedGreenPin = 5;
const int rgbLedBluePin = 4;
const int rgbLedPins[] = { rgbLedRedPin, rgbLedGreenPin, rgbLedBluePin };

const int buttonStartStopPin = 3;
const int buttonCycleDifficultyPin = 2;
const int buttonsPins[] = { buttonStartStopPin, buttonCycleDifficultyPin };

volatile unsigned long buttonCycleDifficultyLastDebounceTimeMilliseconds = 0;
const unsigned long buttonCycleDifficultyDebounceTimeMilliseconds = 50;

volatile unsigned long buttonStartStopLastDebounceTimeMilliseconds = 0;
const unsigned long buttonStartStopDebounceTimeMilliseconds = 50;

const int roundTimeMilliseconds = 30000; // 30000 ms = 30 s
const int startGameAnimationDurationSeconds = 3;

int roundTimeStartMilliseconds = 0;

const int wordsCount = 50;
const String words[wordsCount] = {
  "snake",
  "imminent",
  "tendency",
  "pretend",
  "separate",
  "cooing",
  "agreeable",
  "dress",
  "six",
  "many",
  "half",
  "righteous",
  "metal",
  "scatter",
  "wheel",
  "astonishing",
  "neck",
  "wire",
  "oil",
  "moor",
  "dull",
  "threatening",
  "anxious",
  "rambunctious",
  "actor",
  "electric",
  "spy",
  "wholesale",
  "test",
  "minor",
  "burn",
  "warm",
  "recondite",
  "mourn",
  "rabid",
  "clap",
  "belief",
  "dramatic",
  "medical",
  "fuzzy",
  "even",
  "bucket",
  "grumpy",
  "quiet",
  "company",
  "absorbing",
  "idiotic",
  "tie",
  "support",
  "scandalous"
};

String currentWord = "";

volatile bool shouldCycleDifficulty = false;
volatile bool shouldStartGame = false;
volatile bool shouldStopGame = false;
volatile bool isGameStarted = false;
volatile bool shouldChangeWord = false;

enum Difficulty {
  UNKNOWN = -1,
  EASY = 0,
  MEDIUM = 1,
  HARD = 2
};

enum Difficulty difficulty = Difficulty::UNKNOWN;
int timePerWord = 0;

int correctWordsCounter = 0;

void setRgbLed(int redValue, int greenValue, int blueValue) {
  digitalWrite(rgbLedRedPin, redValue);
  digitalWrite(rgbLedGreenPin, greenValue);
  digitalWrite(rgbLedBluePin, blueValue);
}

void triggerCycleDifficulty() {
  if (millis() - buttonCycleDifficultyLastDebounceTimeMilliseconds > buttonCycleDifficultyDebounceTimeMilliseconds) {
    buttonCycleDifficultyLastDebounceTimeMilliseconds = millis();

    if (isGameStarted) {
      return;
    }

    shouldCycleDifficulty = true;
  }
}

void cycleDifficulty() {
  shouldCycleDifficulty = false;

  difficulty = (Difficulty)(((int)difficulty + 1) % 3); // Cycle through 3 difficulties: Easy / Medium / Hard

  switch (difficulty) {
    case Difficulty::EASY:
      Serial.println("Easy mode on!");
      timePerWord = 4;
      break;
    case Difficulty::MEDIUM:
      Serial.println("Medium mode on!");
      timePerWord = 3;
      break;
    case Difficulty::HARD:
      Serial.println("Hard mode on!");
      timePerWord = 2;
      break;
    default:
      Serial.println("Invalid difficulty. Please try again!");
      difficulty = Difficulty::UNKNOWN;
      timePerWord = 0;
      break;
  }
}

void triggerStartStopGame() {
  if (millis() - buttonStartStopLastDebounceTimeMilliseconds > buttonStartStopDebounceTimeMilliseconds) {
    buttonStartStopLastDebounceTimeMilliseconds = millis();

    if (isGameStarted) {
      shouldStopGame = true;
    }
    else {
      shouldStartGame = true;
    }
  }
}

void startGame() {
  shouldStartGame = false;

  if (difficulty == Difficulty::UNKNOWN) {
    Serial.println("Please choose difficulty.");
    return;
  }

  isGameStarted = true;
  correctWordsCounter = 0;

  for (int i = startGameAnimationDurationSeconds; i > 0; i--) {
    Serial.println(i);
    setRgbLed(HIGH, HIGH, HIGH);
    delay(500);
    setRgbLed(LOW, LOW, LOW);
    delay(500);
  }

  setRgbLed(LOW, HIGH, LOW);

  noInterrupts();

  // Initialize Timer 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  const unsigned long clock = 16000000; // 16 MHz clock frequency
  const unsigned long prescaler = 1024;
  const unsigned long timeCompareTarget = (clock / prescaler) * timePerWord - 1;

  OCR1A = timeCompareTarget;

  TCCR1B |= (1 << WGM12); // Turn on CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set prescaler to 1024
  TIMSK1 |= (1 << OCIE1A); // Enable timer interrupt for compare mode

  interrupts();

  roundTimeStartMilliseconds = millis();
  shouldChangeWord = true;
}

ISR(TIMER1_COMPA_vect) {
  shouldChangeWord = true;
}

void changeWord() {
  shouldChangeWord = false;

  TCNT1 = 0;

  int index = random(0, wordsCount);
  currentWord = words[index];

  Serial.println(currentWord);
}

void stopGame() {
  Serial.print("Score: ");
  Serial.println(correctWordsCounter);

  setInactiveState();
}

void setInactiveState() {
  setRgbLed(HIGH, HIGH, HIGH);
  difficulty = Difficulty::UNKNOWN;
  timePerWord = 0;
  shouldCycleDifficulty = false;
  shouldStartGame = false;
  shouldStopGame = false;
  isGameStarted = false;
  correctWordsCounter = 0;
  shouldChangeWord = false;
  currentWord = "";

  noInterrupts();

  // Reset Timer 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 0;

  TCCR1B &= ~(1 << WGM12); // Turn off CTC mode
  TCCR1B &= ~(1 << CS12) & ~(1 << CS10); // Reset prescaler 1024
  TIMSK1 &= ~(1 << OCIE1A); // Disable timer interrupt for compare mode

  interrupts();

  Serial.println("You must choose the difficulty before you can start the game.");
}

void setup() {
  Serial.begin(115200);

  randomSeed(analogRead(0));

  for (int i = 0; i < 3; i++) {
    pinMode(rgbLedPins[i], OUTPUT);
  }

  for (int i = 0; i < 2; i++) {
    pinMode(buttonsPins[i], INPUT_PULLUP);
  }

  attachInterrupt(digitalPinToInterrupt(buttonCycleDifficultyPin), triggerCycleDifficulty, LOW);
  attachInterrupt(digitalPinToInterrupt(buttonStartStopPin), triggerStartStopGame, LOW);

  setInactiveState();
}

void loop() {
  if (shouldCycleDifficulty) {
    cycleDifficulty();
  }

  if (shouldStartGame) {
    startGame();
  }

  if (shouldStopGame) {
    stopGame();
  }

  if (isGameStarted) {
    if (shouldChangeWord) {
      changeWord();
    }

    if (millis() - roundTimeStartMilliseconds >= roundTimeMilliseconds && !shouldStopGame) {
      shouldStopGame = true;
    }

    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');

      if (input.equals(currentWord)) {
        correctWordsCounter++;
        shouldChangeWord = true;
        setRgbLed(LOW, HIGH, LOW);
      }
      else {
        setRgbLed(HIGH, LOW, LOW);
      }
    }
  }
}