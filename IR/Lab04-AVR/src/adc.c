#include <stdio.h>
#include <avr/io.h>

#include "adc.h"

void adc_init(void)
{
    //(pg. 260) Set ADC prescaler to 128 (don't need higher precision for this project)
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    //(pg. 258) Set ADC reference voltage to AVCC
    ADMUX |= (1 << REFS0);

    //(pg. 260) Enable ADC
    ADCSRA |= (1 << ADEN);
}

uint16_t myAnalogRead(uint8_t channel)
{
    // Force input channel to be between 0 and 7 (as ADC pins are PA0-7)
    channel &= 0b00000111;

    // TODO [task0]:
    // 1. Clear the old channel value (if any, last 5 bits in ADMUX)
    // 2. Select the new channel in ADMUX
    // 3. Start single conversion
    // 4. Busy wait for conversion to complete

    ADMUX = 0;
    /* ADC1 - channel */
    ADMUX |= (channel << MUX0);
    /* AVCC with external capacitor at AREF pin */
    ADMUX |= (1 << REFS0);
    
    ADCSRA = 0;
    /* set prescaler at 128 */
    ADCSRA |= (7 << ADPS0);
    /* enable ADC */
    ADCSRA |= (1 << ADEN);

    /* start conversion */
    ADCSRA |= (1 << ADSC);
    /* wait until conversion is complete */
    while ((ADCSRA & (1 << ADSC)));

    // Return ADC value
    return (ADC);
}