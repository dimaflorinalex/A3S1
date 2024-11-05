#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"
#include "adc.h"
#include "task1.h"

void task1(void) {
  while (1) {
    // TODO [task1]: Use the previous defined function to read the ADC
    // value of the temperature sensor (PA0) and print it to the serial
    uint16_t temperature_adc_value = myAnalogRead(PA0);

    char buffer[6] = "";
    snprintf(buffer, 6 * sizeof(char), "%d\n", temperature_adc_value);
    
    USART0_print(buffer);

    _delay_ms(200);
  }
}