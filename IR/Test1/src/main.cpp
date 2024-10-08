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

int main() {
  DDRD |= (1 << PD5); // Setam pinul 5 al portului D ca output (1)
	DDRD |= (1 << PD7); // Setam pinul 7 al portului D ca output (1)
  DDRB |= (1 << PB3);

  DDRB &= ~(1 << PB2); // Setam pinul 2 al portului B ca input (0)
  PORTB |= (1 << PB2); // Activam rezistenta de pull-up (PB2 e input)

  DDRD &= ~(1 << PD6); // Setam pinul 6 al portului D ca input (0)
  PORTD |= (1 << PD6); // Activam rezistenta de pull-up (PB2 e input)
  
  int button1Counter = 0;
  bool button1IsNotPressedLast = 0, button1IsNotPressed = 0;

  // int button2Counter = 0;
  // bool button2IsNotPressedLast = 0, button2IsNotPressed = 0;

  while (1) {
    button1IsNotPressedLast = button1IsNotPressed;
    button1IsNotPressed = PINB & (1 << PB2);

    // button2IsNotPressedLast = button2IsNotPressed;
    // button2IsNotPressed = PIND & (1 << PD6);

    // if (button2IsNotPressedLast && !button2IsNotPressed) {
    //   button2Counter++;
    // }

    if (button1IsNotPressed) {
      stingeLEDRosu();
      stingeLEDVerde();
      stingeLEDAlbastru();
    }
    else {
      if (button1IsNotPressedLast) {
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
    }

    _delay_ms(200);
  }
 
	return 0;
}