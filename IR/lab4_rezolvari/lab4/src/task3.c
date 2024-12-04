#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "usart.h"
#include "adc.h"
#include "task3.h"

volatile uint8_t print = 0;

// TODO [task3]: Implement the following function to auto-trigger the ADC conversion
void adc_auto_trigger_init(void) {
    /* Auto-trigger enable */
    ADCSRA |= (1 << ADATE);

    /* Enable ADC Interrupt */
    ADCSRA |= (1 << ADIE);

    /* Trigger event: Timer1 compare channel B */
    ADCSRB = (5 << ADTS0);
}

// TODO [task3]: Implement the following function to configure the ADC channel
// HINT: similar to myAnalogRead function in adc.c
void configure_adc_channel(uint8_t channel) {
    // Force input channel to be between 0 and 7 (as ADC pins are PA0-7)
    channel &= 0b00000111;

    // 1. Clear the old channel value (if any, last 5 bits in ADMUX)
    ADMUX &= 0b11100000;

    // 2. Select the new channel in ADMUX
    ADMUX |= channel;
}

//  TODO [task3]: Interrupt Service Routine for ADC conversion complete
ISR(ADC_vect) {
    print = 1;
}

// Timer1 configured to trigger ISR every 1 second
void timer1_init(void) {
    // Set the Timer Mode to CTC
    TCCR1B |= (1 << WGM12);

    // Set the value that you want to count to
    OCR1A = 46875;
    OCR1B = OCR1A;

    // Start the timer with a prescaler of 256
    TCCR1B |= (1 << CS12);

    // Enable the compare B match interrupt
    // The ADC conversion will not start without this
    TIMSK1 |= (1 << OCIE1B);
}

void task3(void) {
    timer1_init();
    configure_adc_channel(PA0);
    adc_auto_trigger_init();

    for (;;) {
        // TODO [task3]: Implement the main loop to read the ADC value of the temperature sensor (PA0)
        // and print it to the serial every 1 second

        if (print) {
            /* This formula can be found in the last link */
            float temp = ((float)ADC * 3.3 / 1023 - 0.4) / 0.0195;
            int16_t temp_int = (int16_t)(temp * 100);

            printf("Temperature: %d.%02d!\n", temp_int / 100, temp_int % 100);
            print = 0;
        }
    }
}