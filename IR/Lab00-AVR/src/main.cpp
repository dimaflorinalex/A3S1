#include <avr/io.h>
#include <util/delay.h>

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

int main() {
  seteazaLEDOutput();
  
  stingeLEDRosu();
  stingeLEDVerde();
  aprindeLEDAlbastru();

  seteazaButoaneInput();
  
  int button1Counter = 0;
  bool button1IsNotPressedLast = 0, button1IsNotPressed = 0;

  int button2Counter = 0;
  bool button2IsNotPressedLast = 0, button2IsNotPressed = 0;

  while (1) {
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

    _delay_ms(200);
  }
 
	return 0;
}