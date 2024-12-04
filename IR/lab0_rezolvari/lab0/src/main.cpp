#include <avr/io.h>
#include <util/delay.h>

void t0()
{
    /* Set pin 7 from port D as output */
    DDRD |= (1 << PD7);

    while(1) {
        PORTD ^= (1 << PD7);
        _delay_ms(500);
    }
}

void t1()
{
    /* Set PD5 (Red LED) as output */
    DDRD |= (1 << PD5);

    /* Set the value to LOW. The LED is light when the value is LOW */
    PORTD &= ~(1 << PD5);

    for (;;);
}

void t2()
{
    /* Set button PB2 as input */
    DDRB &= ~(1 << PB2);

    /* Activate the pull-up resistor */
    PORTB |= (1 << PB2);

    /* Set LED pins to output */
    DDRD |= (1 << PD5);
    DDRD |= (1 << PD7);
    DDRB |= (1 << PB3);

    /* Light up the green LED */
    PORTD |= (1 << PD5);
    PORTD &= ~(1 << PD7);
    PORTB |= (1 << PB3);

    for (;;) {
        /* Read button value. The button is pressed when the value is 0 (LOW) */
        uint8_t button = PINB & (1 << PB2);

        if (!button) {
            PORTD &= ~(1 << PD5);
        } else {
            PORTD |= (1 << PD5);
        }

        _delay_ms(500);
    }
}

#define RED 0
#define GREEN 1
#define BLUE 2
#define NUM_COLORS 3

void t3()
{
    /* Set button PB2 as input and activate the pull-up resistor */
    DDRB &= ~(1 << PB2);
    PORTB |= (1 << PB2);

    /* Set button PD6 as input and activate the pull-up resistor */
    DDRD &= ~(1 << PD6);
    PORTD |= (1 << PD6);

    /* Set LED pins to output */
    DDRD |= (1 << PD5);
    DDRD |= (1 << PD7);
    DDRB |= (1 << PB3);

    /* All LEDs are deactivated at the begining */
    PORTD |= (1 << PD5);
    PORTD |= (1 << PD7);
    PORTB |= (1 << PB3);

    uint8_t color = RED;
    uint8_t blink = 0;
    uint8_t off = 0;

    for (;;) {
        uint8_t color_btn = PINB & (1 << PB2);
        uint8_t blink_btn = PIND & (1 << PD6);

        /* If the color button is pressed, changed the current color */
        if (!color_btn)
            color = (color + 1) % NUM_COLORS;

        /* If the blink button is pressed, changed the blink state and light off all LEDs. */
        if (!blink_btn) {
            blink = 1 - blink;
            off = 1;
        }
            
        switch (color) {
            case RED: {
                PORTB |= (1 << PB3);
                PORTD &= ~(1 << PD5);

                break;
            }
            case GREEN: {
                PORTD |= (1 << PD5);
                PORTD &= ~(1 << PD7);

                break;
            }
            case BLUE: {
                PORTD |= (1 << PD7);
                PORTB &= ~(1 << PB3);

                break;
            }
        }

        if (blink) {
            if (off == 1) {
                PORTD |= (1 << PD5);
                PORTD |= (1 << PD7);
                PORTB |= (1 << PB3);
            }

            off = 1 - off;
        }

        _delay_ms(500);
    }
}

// int main()
// {
//     t3();

//     return 0;
// }

/* Arduino */
#include <Arduino.h>

#define RED_LED 13
#define GREEN_LED 15
#define BLUE_LED 3
#define BTN1 2
#define BTN2 14

/* task 0 */
void setup0()
{
    pinMode(RED_LED, OUTPUT);
}

void loop0()
{
    digitalWrite(RED_LED, HIGH);
    delay(500);
    digitalWrite(RED_LED, LOW);
    delay(500);
}

/* task 1*/
void setup1()
{
    pinMode(GREEN_LED, OUTPUT);
    digitalWrite(GREEN_LED, LOW);
}
void loop1()
{

}

/* task2 */
void setup2()
{
    pinMode(BTN1, INPUT_PULLUP);

    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
}

void loop2()
{
    uint8_t button = digitalRead(BTN1);

    digitalWrite(RED_LED, button);
}

/* task3 */
void setup3()
{
    pinMode(BTN1, INPUT_PULLUP);
    pinMode(BTN2, INPUT_PULLUP);

    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
}

uint8_t color = RED;
uint8_t blink = 0;
uint8_t off = 0;

void loop3()
{
    uint8_t color_btn = digitalRead(BTN1);
    uint8_t blink_btn = digitalRead(BTN2);

    if (!color_btn)
        color = (color + 1) % NUM_COLORS;

    if (!blink_btn) {
        blink = 1 - blink;
        off = 1;
    }

    switch (color) {
        case RED: {
            digitalWrite(BLUE_LED, HIGH);
            digitalWrite(RED_LED, LOW);

            break;
        }
        case GREEN: {
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, LOW);

            break;
        }
        case BLUE: {
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(BLUE_LED, LOW);

            break;
        }
    }

    if (blink) {
        if (off == 1) {
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(BLUE_LED, HIGH);
        }

        off = 1 - off;
    }

    delay(500);
}

void setup()
{
    setup3();
}

void loop()
{
    loop3();
}
