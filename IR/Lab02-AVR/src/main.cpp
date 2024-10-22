#include <avr/io.h>
#include <avr/interrupt.h>

ISR(INT0_vect)
{
  PORTD ^= (1 << PD7);
}

ISR(PCINT3_vect){
  // cod întrerupere de tip pin change
 
  if ((PIND & (1 << PD6)) == 0){
     // întrerupere generată de pinul PD6
     PORTD ^= (1 << PD7);
  }
}

unsigned long millisCustom() {

}

int main() {
  DDRD |= (1 << PD7); // LED Verde output
  DDRD &= ~(1 << PD7);

  DDRD |= (1 << PD4); // Speaker output
  DDRD &= ~(1 << PD4);

  DDRB &= ~(1 << PB2); // Setam pinul 2 al portului B ca input (0)
  PORTB |= (1 << PB2); // Activam rezistenta de pull-up (PB2 e input)

  DDRD &= ~(1 << PD6); // Setam pinul 6 al portului D ca input (0)
  PORTD |= (1 << PD6); // Activam rezistenta de pull-up (PD2 e input)

  // EICRA |= (1 << ISC21);    // set INT2 to trigger on FALLING edge
  EIMSK |= (1 << INT2);     // Turns on INT2

  PCICR |= (1 << PCIE3); // enable the pin change interrupt, set PCIE0 to enable PCMSK0 scan
  PCMSK3 |= (1 << PCINT30); // Turns on PCINT30 (PD6)

  sei();

  while (1) {
  }
 
	return 0;
}