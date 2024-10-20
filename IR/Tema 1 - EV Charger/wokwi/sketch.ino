const int CHARGE_LED_1_PIN = 10; // Pin LED incarcare 25%
const int CHARGE_LED_2_PIN = 9; // Pin LED incarcare 50%
const int CHARGE_LED_3_PIN = 8; // Pin LED incarcare 75%
const int CHARGE_LED_4_PIN = 7; // Pin LED incarcare 100%

const int STATE_RGB_LED_RED_PIN = 6; // Pin LED RGB stare rosu
const int STATE_RGB_LED_GREEN_PIN = 5; // Pin LED RGB stare verde
const int STATE_RGB_LED_BLUE_PIN = 4; // Pin LED RGB stare albastru

const int START_BTN_PIN = 3; // Pin buton start
const int STOP_BTN_PIN = 2; // Pin buton stop

// Pini LED-uri pentru progresul incarcarii (0 => L1 = 25%, 1 => L2 = 50%, 2 => L3 = 75%, 3 => L4 = 100%)
const int CHARGE_LED_PINS[] = { CHARGE_LED_1_PIN, CHARGE_LED_2_PIN, CHARGE_LED_3_PIN, CHARGE_LED_4_PIN };

// Pini LED RGB pentru starea statiei (0 => R, 1 => G, 2 => B)
const int STATE_RGB_LED_PINS[] = { STATE_RGB_LED_RED_PIN, STATE_RGB_LED_GREEN_PIN, STATE_RGB_LED_BLUE_PIN };

// Pini pentru butoane (0 => START, 1 => STOP)
const int BUTTON_PINS[] = { START_BTN_PIN, STOP_BTN_PIN };

bool shouldStartCharging = false; // Flag: Ar trebui initializata procedura de start charging
bool shouldStopCharging = false; // Flag: Ar trebui initializata procedura de stop charging

int startButtonState = HIGH; // Starea curenta stabila a butonului de start
int startButtonLastState = HIGH; // Ultima stare stabila a butonului de start
unsigned long startButtonLastDebounceTime = 0; // Timestamp last debounce pentru butonul de start
unsigned long startButtonDebounceDelay = 50; // Delay ms pentru debounce buton start

int stopButtonState = HIGH; // Starea curenta stabila a butonului de stop
int stopButtonLastState = HIGH; // Ultima stare stabila a butonului de stop
unsigned long stopButtonLastDebounceTime = 0; // Timestamp last debounce pentru butonul de stop
unsigned long stopButtonDebounceDelay = 1000; // Delay ms pentru debounce buton stop

// Verifica starea butonului de start
void checkStartButtonState() {
  int startButtonCurrentState = digitalRead(START_BTN_PIN);

  if (startButtonCurrentState != startButtonLastState) {
    startButtonLastDebounceTime = millis();
  }
 
  if ((millis() - startButtonLastDebounceTime) > startButtonDebounceDelay) {
    if (startButtonCurrentState != startButtonState) {
      startButtonState = startButtonCurrentState;

      if (startButtonState == LOW) {
        shouldStartCharging = true;
      }
    }
  }

  startButtonLastState = startButtonCurrentState;
}

// Verifica starea butonului de stop
void checkStopButtonState() {
  int stopButtonCurrentState = digitalRead(STOP_BTN_PIN);

  if (stopButtonCurrentState != stopButtonLastState) {
    stopButtonLastDebounceTime = millis();
  }

  if ((millis() - stopButtonLastDebounceTime) > stopButtonDebounceDelay) {
    if (stopButtonCurrentState != stopButtonState) {
      stopButtonState = stopButtonCurrentState;

      if (stopButtonState == LOW) {
        shouldStopCharging = true;
      }
    }
  }

  stopButtonLastState = stopButtonCurrentState;
}

// Functie auxiliara pentru setarea LED-ului RGB (starea statiei)
// Doar rosu (statie ocupata) si verde (statie libera) ne sunt utile, albastru il tinem constant oprit
void setStateLED(int redValue, int greenValue) {
  digitalWrite(STATE_RGB_LED_RED_PIN, redValue);
  digitalWrite(STATE_RGB_LED_GREEN_PIN, greenValue);
  digitalWrite(STATE_RGB_LED_BLUE_PIN, LOW);
}

// Reseteaza starea statiei la "libera"
void resetCharging() {
  shouldStartCharging = false; // Reset flag should start charging
  shouldStopCharging = false; // Reset flag should stop charging
  setStateLED(LOW, HIGH); // Aprindem verde (statie libera)

  // Resetam (stingem) LED-urile pentru progresul incarcarii
  for (int i = 0; i < 4; i++) {
    digitalWrite(CHARGE_LED_PINS[i], LOW);
  }
}

// Procesul de incarcare (blink LEDs si posibilitate oprire fortata)
void charging() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(CHARGE_LED_PINS[i], LOW);

    for (int j = 0; j < 3; j++) {
      checkStopButtonState();

      if (shouldStopCharging) {
        return;
      }

      digitalWrite(CHARGE_LED_PINS[i], HIGH);
      delay(500);

      digitalWrite(CHARGE_LED_PINS[i], LOW);
      delay(500);
    }

    digitalWrite(CHARGE_LED_PINS[i], HIGH);
    delay(500);
  }
}

// Procedura de finalizare incarcare (blink LEDs de 3 ori)
void finishCharging() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(CHARGE_LED_PINS[i], HIGH);
  }

  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(CHARGE_LED_PINS[i], LOW);
    }
    
    delay(1000);

    for (int i = 0; i < 4; i++) {
      digitalWrite(CHARGE_LED_PINS[i], HIGH);
    }

    delay(1000);
  }
}

// Procedura de oprire a incarcarii
void stopCharging() {
  finishCharging(); // Finalizam incarcarea
  resetCharging(); // Resetam starea statiei
}

// Procedura de incepere a incarcarii
void startCharging() {
  shouldStartCharging = false; // Resetam marcajul de "ar trebui sa incepem incarcarea"
  setStateLED(HIGH, LOW); // Aprindem becul rosu (statie ocupata)
  charging(); // Rulam procesul de incarcare
  shouldStopCharging = true; // Dupa ce procesul de incarcare s-a terminat (normal sau fortat), marcam faptul ca ar trebui oprita incarcarea
}

// Initializarea sistemului
void setup() {
  // Setam modul pinilor pentru LED-uri progres incarcare ca OUTPUT
  for (int i = 0; i < 4; i++) {
    pinMode(CHARGE_LED_PINS[i], OUTPUT);
  }

  // Setam modul pinilor pentru LED-ul RGB ca OUTPUT
  for (int i = 0; i < 3; i++) {
    pinMode(STATE_RGB_LED_PINS[i], OUTPUT);
  }

  // Setam modul pinilor pentru butoane ca INPUT_PULLUP
  for (int i = 0; i < 2; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  // Resetam (initializam) starea statiei
  resetCharging();
}

// Bucla operationala a sistemului
void loop() {
  checkStartButtonState();

  if (shouldStartCharging) {
    startCharging();
  }

  if (shouldStopCharging) {
    stopCharging();
  }
}
