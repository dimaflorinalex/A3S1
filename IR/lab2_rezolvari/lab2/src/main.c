#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

#include <usart.h>
#include <stdbool.h>
#include <stdlib.h>

#define BAUD 28800

/************************************************** Task 0.1 **************************************************/
// volatile uint8_t on = 0;

// ISR(INT2_vect)
// {
//     on = 1 - on;
// }

// void task01()
// {
//     /* Make green LED as output and turn it off */
//     DDRD |= (1 << PD7);
//     PORTD |= (1 << PD7);

//     /* Set PB2 as input and activate pull-up resistance */
//     DDRB &= ~(1 << PB2);
//     PORTB |= (1 << PB2); 

//     /* Set INT2 to activate on falling edge */
//     EICRA |= (2 << ISC20);

//     /* Activate external interrupt INT2 */
//     EIMSK |= (1 << INT2);

//     /* Activate global interrupts */
//     sei();

//     for (;;) {
//         if (on) {
//             /* LED on */
//             PORTD &= ~(1 << PD7);
//         } else {
//             /* LED off */
//             PORTD |= (1 << PD7);
//         }
//     }
// }

/************************************************** Task 0.2 **************************************************/
// volatile uint8_t on = 0;

// ISR(PCINT3_vect)
// {
//     /* Verify that the pin PD6 causes the interrupt */
//     if ((PIND & (1 << PD6)) == 0)
//         on = 1 - on;
// }

// void task02()
// {
//     /* Make green LED as output and turn it off */
//     DDRD |= (1 << PD7);
//     PORTD |= (1 << PD7);

//     /* Set PD6 as input and activate pull-up resistance */
//     DDRD &= ~(1 << PD6);
//     PORTD |= (1 << PD6);

//     /* Activate PCINT3 interrupt */
//     PCICR |= (1 << PCIE3);

//     /* Activate PCINT30 (PD6) pin interrupt */
//     PCMSK3 |= (1 << PCINT30);

//     /* Activate global interrupts */
//     sei();

//     for (;;) {
//         if (on) {
//             /* LED on */
//             PORTD &= ~(1 << PD7);
//         } else {
//             /* LED off */
//             PORTD |= (1 << PD7);
//         }
//     }
// }

/************************************************** Task 1 **************************************************/
// #define NUM_FREQS 3
// #define FREQ0 100
// #define FREQ1 200
// #define FREQ2 300

// volatile uint8_t freq;
// volatile uint8_t btn2;


// ISR(TIMER1_COMPA_vect)
// {
//     /* Alternate buzzer value */
//     if (btn2 == 0)
//         PORTD ^= (1 << PD4);
// }

// ISR(INT2_vect)
// {
//     freq = (freq + 1) % NUM_FREQS;

//     switch (freq) {
//     case 0: {
//         OCR1A = (uint16_t)(F_CPU / (8 * FREQ0) - 1);

//         PORTD &= ~(1 << PD5);
//         PORTD |= (1 << PD7);
//         PORTB |= (1 << PB3);

//         break;
//     }
//     case 1: {
//         OCR1A = (uint16_t)(F_CPU / (8 * FREQ1) - 1);

//         PORTD |= (1 << PD5);
//         PORTD &= ~(1 << PD7);
//         PORTB |= (1 << PB3);

//         break;
//     }
//     case 2: {
//         OCR1A = (uint16_t)(F_CPU / (8 * FREQ2) - 1);

//         PORTD |= (1 << PD5);
//         PORTD |= (1 << PD7);
//         PORTB &= ~(1 << PB3);

//         break;
//     }
//     }
// }

void timer1_init()
{
    /* Enable compare with OCR1A interrupt */
    TIMSK1 |= (1 << OCIE1A);
}

void timer1_stop()
{
    /* Disable compare with OCR1A interrupt */
    TIMSK1 &= ~(1 << OCIE1A);
}

// void task1()
// {
//     /* Setup LEDs */
//     DDRD |= (1 << PD5);
//     DDRD |= (1 << PD7);
//     DDRB |= (1 << PB3);

//     /* Light up red LED */
//     PORTD &= ~(1 << PD5);
//     PORTD |= (1 << PD7);
//     PORTB |= (1 << PB3);

//     /* Setup buttons */
//     DDRB &= ~(1 << PB2);
//     PORTB |= (1 << PB2);

//     DDRD &= ~(1 << PD6);
//     PORTD |= (1 << PD6);

//     /* Setup Buzzer */
//     DDRD |= (1 << PD4);

//     /* Set timer1 on mode CTC */
//     TCCR1B |= (1 << WGM12);

//     /* Init frequency */
//     freq = 0;
//     OCR1A = (uint16_t)(F_CPU / (8 * FREQ0) - 1);

//     /* Set timer1 prescaler to 8 */
//     TCCR1B |= (1 << CS11);

//     /* Set INT2 to activate on falling edge */
//     EICRA |= (2 << ISC20);

//     /* Activate external interrupt INT2 */
//     EIMSK |= (1 << INT2);

//     timer1_init();
//     sei();

//     for (;;) {
//         btn2 = PIND & (1 << PD6);
//     }
// }

/************************************************** Task 2.1 **************************************************/
// #define CNT 46874

// volatile uint16_t seconds = 0;
// volatile uint8_t print = 0;

// ISR(TIMER1_COMPA_vect)
// {
//     seconds++;
//     print = 1;
// }

// uint32_t millis()
// {
//     return seconds * 1000 + TCNT1 * 1000 / CNT;
// }

// void task21()
// {
//     /* Set timer1 on mode CTC */
//     TCCR1B |= (1 << WGM12);

//     /* Set timer1 prescaler to 256 */
//     TCCR1B |= (1 << CS12);

//     OCR1A = CNT;

//     timer1_init();
//     sei();

//     USART0_init(BAUD);

//     for (;;) {
//         if (print == 1) {
//             USART0_print("A second has passed!\n");
//             print = 0;
//         }
//     }
// }

/************************************************** Task 2.2 **************************************************/
// void debounce()
// {
//     uint8_t reading = 1;
//     uint8_t last_reading = 1;
//     uint8_t button_state = 1;

//     uint8_t debounce_delay = 50;
//     uint16_t last_debounce_time = 0;

//     for (;;) {
//         reading = PINB & (1 << PB2);
    
//         if (reading != last_reading)
//             last_debounce_time = millis();

//         if (millis() - last_debounce_time > debounce_delay) {
//             if (reading != button_state) {
//                 button_state = reading;
                
//                 if (button_state == 0) {
//                     /* your logic */
//                 }
//             }
//         }
        
//         last_reading = reading;
//     }
// }

/************************************************** Task 3 **************************************************/
#define NUM_FREQS 3
#define FREQ1 5
#define FREQ2 10

#define NUM_COLORS 3
#define RED 0
#define GREEN 1
#define BLUE 2

volatile uint8_t color;
volatile uint8_t freq;
volatile bool on;

ISR(INT2_vect)
{
    color = (color + 1) % NUM_COLORS;
}

ISR(PCINT3_vect)
{
    /* Verify that the pin PD6 causes the interrupt */
    if ((PIND & (1 << PD6)) == 0) {
        freq = (freq + 1) % NUM_FREQS;

        /* Make sure LED is on when blinking is disable */
        switch (freq) {
        case 0: {
            on = true;
            timer1_stop();
            break;
        }
        case 1: {
            OCR1A = (uint16_t)(F_CPU / (64 * FREQ1) - 1);
            timer1_init();
            break;
        }
        case 2: {
            OCR1A = (uint16_t)(F_CPU / (64 * FREQ2) - 1);
            timer1_init();
            break;
        }
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    if (freq != 0)
        on = !on;
}

void task3()
{
    /* Setup LEDs */
    DDRD |= (1 << PD5);
    DDRD |= (1 << PD7);
    DDRB |= (1 << PB3);

    /* Light up RED LED */
    PORTD &= ~(1 << PD5);
    PORTD |= (1 << PD7);
    PORTB |= (1 << PB3);

    /* Set PB2 as input and activate pull-up resistance */
    DDRB &= ~(1 << PB2);
    PORTB |= (1 << PB2);

    /* Activate external interrupt INT2 (PB2) on falling edge */
    EICRA |= (2 << ISC20);
    EIMSK |= (1 << INT2);

    /* Set PD6 as input and activate pull-up resistance */
    DDRD &= ~(1 << PD6);
    PORTD |= (1 << PD6);

    /* Activate pin change interrupt PCINT30 (PD6) */
    PCICR |= (1 << PCIE3);
    PCMSK3 |= (1 << PCINT30);

    /* Set timer1 on mode CTC */
    TCCR1B |= (1 << WGM12);

    /* Set timer1 prescaler to 64 */
    TCCR1B |= (3 << CS10);

    /* Initial values */
    color = RED;
    freq = 0;
    on = true;

    sei();

    for (;;) {
        if (on) {
            switch (color) {
            case RED: {
                PORTD &= ~(1 << PD5);
                PORTD |= (1 << PD7);
                PORTB |= (1 << PB3);
                break;
            }
            case GREEN: {
                PORTD |= (1 << PD5);
                PORTD &= ~(1 << PD7);
                PORTB |= (1 << PB3);
                break;
            }
            case BLUE: {
                PORTD |= (1 << PD5);
                PORTD |= (1 << PD7);
                PORTB &= ~(1 << PB3);
                break;
            }
            }
        } else {
            /* Light off all LEDs */
            PORTD |= (1 << PD5);
            PORTD |= (1 << PD7);
            PORTB |= (1 << PB3);
        }
    }
}

int main()
{
    task3();

    return 0;
}
