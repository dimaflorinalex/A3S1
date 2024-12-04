#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "usart.h"
#include "timers.h"
#include "colors.h"
#include "sound.h"

/* Configuration constants: */
#define PM_BAUD 28800

#define RGB_PULSE  1
#define RGB_HSV    2

#define LED_ANIMATION  RGB_HSV
#define SPEAKER_ENABLED 1


/**
 * Initialize the GPIOs of LEDs / speaker etc.
 */
int GPIO_init() {
    /* RGB led directions & default PORTs to 1 (OFF state) */
    DDRB |= (1 << PB3);
    DDRD |= (1 << PD5) | (1 << PD7);
    PORTB |= (1 << PB3);
    PORTD |= (1 << PD5) | (1 << PD7);
    return 0;
}

void buzzer_init()
{
    DDRD |= (1 << PD4);
    OCR1A = (uint16_t)(F_CPU / (8 * surprise_notes[0]) - 1);
}

int main() {
    /* initialize peripheral modules: */
    GPIO_init();
    USART0_init(CALC_USART_UBRR(PM_BAUD));
    USART0_use_stdio();

    if (SPEAKER_ENABLED) {
        Timer1_init_ctc();
        buzzer_init();
    } else {
        Timer1_init_pwm();
    }
    Timer2_init_systicks();
    /* TODO add more *_init_*() calls here? */
    Timer0_init_pwm();

    sei(); /* activate global interrupts */

    printf("It's timer hammer!\n");

    uint32_t last_ping = 0;
    uint32_t last_note = 0;
    uint8_t note_index = 0;

    while (1) {
        /* Print alive message */
        if (SYSTICKS_PASSED(last_ping, 3000)) {
            last_ping = systicks;
            printf("Stayin' alive! [ticks=%ld]\n", systicks);
        }

        if (SPEAKER_ENABLED) {
            /* TODO task 4 (bonus): you know what you have to do ;) */
            if (SYSTICKS_PASSED(last_note, durations[note_index])) {
                note_index = (note_index + 1) % num_notes;
                last_note = systicks;

                TCNT1 = 0;
                OCR1A = (uint16_t)(F_CPU / (8 * surprise_notes[note_index]) - 1);
            }
        }

        if (LED_ANIMATION == RGB_PULSE) {
            /*
             * Pulse de LEDs based on a triangle wave:
             * y = (P - abs(t % (2*P) - P) )
             * Note: outputs values between [0, 3000]!
             */
            int wave1 = 3000 - abs((systicks % 6000) - 3000);

            /* rescale [0, 3000] to [0, 255] using ceil(3000 / 255) */
            if (SPEAKER_ENABLED) {
                OCR1B = wave1 / 12 * OCR1A / 255;
            } else {
                OCR1A = wave1 / 12;
            }

            /* TODO task 1 & 2: do the same for other LEDs */
            int wave2 = 1000 - abs((systicks % 2000) - 1000);
            OCR0A = wave2 / 4;

            int wave3 = 2000 - abs((systicks % 4000) - 2000);
            OCR2B = wave3 / 8 * OCR2A / 255;

        } else if (LED_ANIMATION == RGB_HSV) {
            /* TODO task 3: output all different Hue colors */
            uint8_t rgb[3];

            int hue = (systicks % 3600) / 10;

            convert_HSV_to_RGB(hue, 1.0, 1.0, rgb);

            if (SPEAKER_ENABLED) {
                OCR1B = rgb[0] * OCR1A / 225;
            } else {
                OCR1A = rgb[0];
            }
            OCR2B = (uint8_t)((uint16_t)rgb[1] * 188 / 255);
            OCR0A = rgb[2];
        }
    }

    return 0;
}
