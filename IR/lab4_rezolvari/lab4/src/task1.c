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
        uint16_t adccount = myAnalogRead(PA0);
        float temp = ((float)adccount * 3.3 / 1023 - 0.4) / 0.0195;
        int16_t temp_int = (int16_t)(temp * 100);

        printf("Temperature: %d.%02d!\n", temp_int / 100, temp_int % 100);

        _delay_ms(500);
    }
}