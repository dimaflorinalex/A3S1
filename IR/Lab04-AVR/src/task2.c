#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"
#include "adc.h"
#include "task1.h"

// Macro to check if the value is in the range of a button press
#define VALUE_IN_RANGE(read_value, threshold_value) \
	(((read_value - threshold_value) <= 10) || ((threshold_value - read_value) <= 10))
  
// TODO [task2]: Complete the following defines with the correct values from ADC
#define BTN1 512
#define BTN2 614
#define BTN3 684
#define BTN4 732
#define BTN5 769
#define BTN6 798


// Initialize the LEDs and turn them off
void init_LEDs(void) {
  DDRB |= (1 << PB3);
  DDRD |= (1 << PD5) | (1 << PD7);
  PORTB |= (1 << PB3);
  PORTD |= (1 << PD5) | (1 << PD7);
}

void task2(void) {
  init_LEDs();

  while (1) {
    // TODO [task2]:
    // 1. Map the ADC values of the buttons to the defines (PA5)
    // 2. Turn on the corresponding LED when the button is pressed
    uint16_t button_adc_value = myAnalogRead(PA5);

    // Turn LEDs on/off based on button ADC values
    if (VALUE_IN_RANGE(button_adc_value, BTN2)) {
      // Turn on RED and turn off the rest
      PORTD &= ~(1 << PD5); // R
      PORTD |= (1 << PD7); // G
      PORTB |= (1 << PB3); // B
    } else if (VALUE_IN_RANGE(button_adc_value, BTN3)) {
      // Turn on GREEN and turn off the rest
      PORTD |= (1 << PD5); // R
      PORTD &= ~(1 << PD7); // G
      PORTB |= (1 << PB3); // B
    } else if (VALUE_IN_RANGE(button_adc_value, BTN5)) {
      // Turn on BLUE and turn off the rest
      PORTD |= (1 << PD5); // R
      PORTD |= (1 << PD7); // G
      PORTB &= ~(1 << PB3); // B
    }

    _delay_ms(100);
  }
}