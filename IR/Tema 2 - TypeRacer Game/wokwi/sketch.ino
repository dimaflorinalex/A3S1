// RGB LED Pins
const int rgbLedRedPin = 6;
const int rgbLedGreenPin = 5;
const int rgbLedBluePin = 4;
const int rgbLedPins[] = { rgbLedRedPin, rgbLedGreenPin, rgbLedBluePin };

// Buttons Pins
const int buttonStartStopPin = 3;
const int buttonCycleDifficultyPin = 2;
const int buttonsPins[] = { buttonStartStopPin, buttonCycleDifficultyPin };

// Buttons debouncing
volatile unsigned long buttonCycleDifficultyLastDebounceTimeMilliseconds = 0;
const unsigned long buttonCycleDifficultyDebounceTimeMilliseconds = 50;

volatile unsigned long buttonStartStopLastDebounceTimeMilliseconds = 0;
const unsigned long buttonStartStopDebounceTimeMilliseconds = 50;

// Game constants
const int roundTimeMilliseconds = 30000; // 30000 ms = 30 s (Round time)
const int startGameAnimationDurationSeconds = 3; // Start Game animation (3, 2, 1 and LED blink)

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

enum Difficulty {
  UNKNOWN = -1,
  EASY = 0,
  MEDIUM = 1,
  HARD = 2
};

// Game state
volatile bool shouldCycleDifficulty = false;
volatile bool shouldStartGame = false;
volatile bool shouldStopGame = false;
volatile bool isGameStarted = false;
volatile bool shouldChangeWord = false;

enum Difficulty difficulty = Difficulty::UNKNOWN;
int timePerWord = 0; // Maximum time per word. It must be correlated with difficulty. 0 is invalid, meaning that difficulty is unknown.

long roundTimeStartMilliseconds = 0; // Timestamp round start
int correctWordsCounter = 0; // Score
String currentWord = ""; // Current random word
String inputText = ""; // Current user input

// Sets values for RGB LED digital
void setRgbLed(int redValue, int greenValue, int blueValue) {
  digitalWrite(rgbLedRedPin, redValue);
  digitalWrite(rgbLedGreenPin, greenValue);
  digitalWrite(rgbLedBluePin, blueValue);
}

// Handler for Cycle Difficulty Button
void triggerCycleDifficulty() {
  if (millis() - buttonCycleDifficultyLastDebounceTimeMilliseconds > buttonCycleDifficultyDebounceTimeMilliseconds) {
    buttonCycleDifficultyLastDebounceTimeMilliseconds = millis();

    if (isGameStarted) {
      return;
    }

    shouldCycleDifficulty = true;
  }
}

// Handler for Cycle Difficulty Event (shouldCycleDifficulty flag)
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

// Handler for Start/Stop Button
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

// Handler for StartGame Event (shouldStartGame flag)
void startGame() {
  shouldStartGame = false;

  if (difficulty == Difficulty::UNKNOWN) {
    Serial.println("Please choose difficulty.");
    return;
  }

  correctWordsCounter = 0;
  inputText = "";

  // Start game animation
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
  isGameStarted = true;
  shouldChangeWord = true;
}

// ISR for timer 1 (timer for change word)
ISR(TIMER1_COMPA_vect) {
  shouldChangeWord = true;
}

// Handler for ChangeWord Event (shouldChangeWord flag)
void changeWord() {
  shouldChangeWord = false;

  TCNT1 = 0; // Reset change word timer

  // Choose random word
  int index = random(0, wordsCount);
  currentWord = words[index];

  inputText = ""; // Reset user input

  Serial.print("\n\n");
  Serial.println(currentWord);
}

// Resets game to inactive state
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
  inputText = "";

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

// Stops the game
void stopGame() {
  Serial.print("\n\nScore: ");
  Serial.println(correctWordsCounter);

  setInactiveState();
}

// Initial load
void setup() {
  Serial.begin(115200);

  randomSeed(analogRead(0)); // Sets a random seed for ensuring a random sequence of words

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

// Operational loop
void loop() {
  // Handle cycle difficulty
  if (shouldCycleDifficulty) {
    cycleDifficulty();
  }

  // Handle start game
  if (shouldStartGame) {
    startGame();
  }

  // Handle stop game
  if (shouldStopGame) {
    stopGame();
  }

  // Handle actual game
  if (isGameStarted) {
    // Handle change word
    if (shouldChangeWord) {
      changeWord();
    }

    // Check if round should finish (time elapsed)
    if ((millis() - roundTimeStartMilliseconds >= roundTimeMilliseconds) && !shouldStopGame) {
      shouldStopGame = true;
    }

    // Handle user input
    if (Serial.available()) {
      char c = Serial.read(); // Read character

      if (c == '\b') { // If character is a backspace
        if (inputText.length() > 0) {
          inputText.remove(inputText.length() - 1); // Remove last character in inputText (buffer for user input)

          // Replace current line in terminal with spaces and then replace spaces with user input (reflection for backspace)
          String clearText = "";

          clearText += "\r";
          for (unsigned int i = 0; i <= inputText.length(); i++) {
            clearText += " ";
          }
          clearText += "\r";

          Serial.print(clearText);
          Serial.print(inputText);
        }
      }
      else if (c == '\n') { // Else if character is a new line
        inputText = ""; // Clear input
      }
      else { // Else it is a normal character
        inputText += c; // Append to input
      }

      if (currentWord.startsWith(inputText)) { // If current word starts with input text, then user is on the right path
        setRgbLed(LOW, HIGH, LOW); // Turn on green LED, so the user knows that he doesn't have mistakes in his word until now
      }
      else { // Else user input contains mistakes (impossible to get to the current word)
        setRgbLed(HIGH, LOW, LOW); // Turn on red LED, so the user knows that he must correct his input
      }

      if (currentWord.equals(inputText)) { // If input is correct (equals to current word)
        correctWordsCounter++; // Increment the score
        shouldChangeWord = true; // Request new word
      }
    }
  }
}