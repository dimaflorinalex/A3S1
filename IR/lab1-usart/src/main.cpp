#include <util/delay.h>
#include <usart.c>
#include <string.h>

#define CLOCK_SPEED 12000000
#define BAUD 28800
#define MYUBRR CLOCK_SPEED/16/BAUD-1

#define MORSE_LUNG  3
#define MORSE_SCURT 1

/* Literele alfabetului in cod Morse */
char* morse_alphabet[] = {
    (char*)"13",    // a
    (char*)"3111",  // b
    (char*)"3131",  // c
    (char*)"311",   // d
    (char*)"1",     // e
    (char*)"1131",  // f
    (char*)"331",	// g
    (char*)"1111",	// h
    (char*)"11",	// i
    (char*)"1333",	// j
    (char*)"313",	// k
    (char*)"1311",	// l
    (char*)"33",	// m
    (char*)"31",	// n
    (char*)"333",	// o
    (char*)"1331",	// p
    (char*)"3313",	// q
    (char*)"131",	// r
    (char*)"111",	// s
    (char*)"3",	    // t
    (char*)"113",	// u
    (char*)"1113",	// v
    (char*)"133",	// w
    (char*)"3113",	// x
    (char*)"3133",	// y
    (char*)"3311"	// z
};

/* apelati functia pentru a obtine un ton lung/scurt,
 * folosind macro-urile MORSE_LUNG si MORSE_SCURT
 */
void speaker_morse(int tip)
{
    int delay = 1;
    int i;
    DDRD |= (1 << PD4);     // PD4 trebuie sa fie output

    for (i = 0; i < tip * 50 / delay; i++)
    {
        PIND = (1 << PD4);  // alternam starea iesirii
        _delay_ms(1);
    }
}

void aprindeLEDVerde() {
  PORTD &= ~(1 << PD7); // Setam PD7 pe 0 (Aprindem PD7 - LED verde)
}

void stingeLEDVerde() {
  PORTD |= (1 << PD7);
}

void aprindeLEDRosu() {
  PORTD &= ~(1 << PD5);
}

void stingeLEDRosu() {
  PORTD |= (1 << PD5);
}

void aprindeLEDAlbastru() {
  PORTB &= ~(1 << PB3);
}

void stingeLEDAlbastru() {
  PORTB |= (1 << PB3);
}

void seteazaLEDOutput() {
  DDRD |= (1 << PD5); // LED Rosu output
	DDRD |= (1 << PD7); // LED Verde output
  DDRB |= (1 << PB3); // LED Albastru output
}

void seteazaLEDInput() {
  DDRD &= ~(1 << PD5); // LED Rosu input
	DDRD &= ~(1 << PD7); // LED Verde input
  DDRB &= ~(1 << PB3); // LED Albastru input
}

void seteazaButoaneInput() {
  DDRB &= ~(1 << PB2); // Setam pinul 2 al portului B ca input (0)
  PORTB |= (1 << PB2); // Activam rezistenta de pull-up (PB2 e input)

  DDRD &= ~(1 << PD6); // Setam pinul 6 al portului D ca input (0)
  PORTD |= (1 << PD6); // Activam rezistenta de pull-up (PB2 e input)
}

bool citesteButon1() {
  return PINB & (1 << PB2);
}

bool citesteButon2() {
  return PIND & (1 << PD6);
}

char buffer[7];
int currentIndex = 0;

void resetBufferAndIndex() {
    for (int i = 0; i < 7; i++) {
        buffer[i] = '\0';
    }

    currentIndex = 0;
}

void USART_exec(unsigned char command) {
    if (currentIndex > 7) {
        resetBufferAndIndex();
    }

    buffer[currentIndex++] = command;

    if (strcmp(buffer, "on") == 0) {
        aprindeLEDAlbastru();
        aprindeLEDRosu();
        aprindeLEDVerde();
        resetBufferAndIndex();
    }
    else if (strcmp(buffer, "off") == 0) {
        stingeLEDAlbastru();
        stingeLEDRosu();
        stingeLEDVerde();
        resetBufferAndIndex();
    }
    else if (strcmp(buffer, "red") == 0) {
        stingeLEDVerde();
        stingeLEDAlbastru();
        aprindeLEDRosu();
        resetBufferAndIndex();
    }
    else if (strcmp(buffer, "green") == 0) {
        stingeLEDAlbastru();
        stingeLEDRosu();
        aprindeLEDVerde();
        resetBufferAndIndex();
    }
    else if (strcmp(buffer, "blue") == 0) {
        stingeLEDRosu();
        stingeLEDVerde();
        aprindeLEDAlbastru();
        resetBufferAndIndex();
    }
}

int main() {
	
  USART0_init(MYUBRR);

  DDRB &= ~(1 << PB2); // Setam pinul 2 al portului B ca input (0)
  PORTB |= (1 << PB2); // Activam rezistenta de pull-up (PB2 e input)

  char wasButtonPressed = 0;

  seteazaLEDOutput();

  while (1) {
    char buttonValue = PINB & (1 << PB2);

    if (buttonValue == 0 && wasButtonPressed == 0) {
        wasButtonPressed = 1;
        USART0_print((char *) "Button 1 was pressed!\n");
    }

    if (buttonValue != 0) {
        wasButtonPressed = 0;
    }

    char received = USART0_receive();

    USART_exec(received);

    _delay_ms(200);
  }
 
	return 0;
}

