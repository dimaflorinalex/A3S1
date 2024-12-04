#include <avr/io.h>
#include <util/delay.h>
#include <usart.h>

#define BAUD 28800

/* 1 -> short, 3 -> long */
char* morse_alphabet[] = {
    (char*)"13",    // a
    (char*)"3111",  // b
    (char*)"3131",  // c
    (char*)"311",   // d
    (char*)"1",     // e
    (char*)"1131",  // f
    (char*)"331",	// g
    (char*)"1111",	// h
    (char*)"11",	// i
    (char*)"1333",	// j
    (char*)"313",	// k
    (char*)"1311",	// l
    (char*)"33",	// m
    (char*)"31",	// n
    (char*)"333",	// o
    (char*)"1331",	// p
    (char*)"3313",	// q
    (char*)"131",	// r
    (char*)"111",	// s
    (char*)"3",	    // t
    (char*)"113",	// u
    (char*)"1113",	// v
    (char*)"133",	// w
    (char*)"3113",	// x
    (char*)"3133",	// y
    (char*)"3311"	// z
};

/* Call the function with short or long types */
void speaker_morse(int type)
{
    int delay = 100;

    DDRD |= (1 << PD4);     /* Set PD4 as output */

    for (int i = 0; i < type * 50; i++) {
        PIND = (1 << PD4); /* Switch the output state */
        _delay_ms(1);
    }

    _delay_ms(delay);
}

void t1()
{
    /* Set PB2 as input and activate the pull-up resistor */
    DDRB &= ~(1 << PB2);
    PORTB |= (1 << PB2);

    for (;;) {
        uint8_t button = PINB & (1 << PB2);

        if (!button)
            USART0_print("Butonul 1 a fost apasat!\n");

        _delay_ms(500);
    }
}

void USART_exec(char cmd)
{
    switch (cmd) {
        /* Light off the LEDs */
        case '0': {
            PORTD |= (1 << PD5);
            PORTD |= (1 << PD7);
            PORTB |= (1 << PB3);
            break;
        }
        /* Light on the LEDs */
        case '1': {
            PORTD &= ~(1 << PD5);
            PORTD &= ~(1 << PD7);
            PORTB &= ~(1 << PB3);
            break;
        }
        /* Light on only the red LED */
        case 'r': {
            PORTD &= ~(1 << PD5);
            PORTD |= (1 << PD7);
            PORTB |= (1 << PB3);
            break;
        }
        /* Light on only the green LED */
        case 'g': {
            PORTD |= (1 << PD5);
            PORTD &= ~(1 << PD7);
            PORTB |= (1 << PB3);
            break;
        }
        /* Light on only the blue LED */
        case 'b': {
            PORTD |= (1 << PD5);
            PORTD |= (1 << PD7);
            PORTB &= ~(1 << PB3);
            break;
        }
    }
}

void t2()
{
    DDRD |= (1 << PD5);
    DDRD |= (1 << PD7);
    DDRB |= (1 << PB3);

    PORTD |= (1 << PD5);
    PORTD |= (1 << PD7);
    PORTB |= (1 << PB3);

    for (;;) {
        char cmd = USART0_receive();

        USART_exec(cmd);
    }
}

void t3()
{
    for (;;) {
        char letter = USART0_receive();

        if (letter >= 'a' && letter <= 'z') {
            char *morse = morse_alphabet[letter - 'a'];

            while (*morse != '\0') {
                speaker_morse(*morse - '0');
                morse++;
            }
        }
    }
}

void t4()
{
    DDRD |= (1 << PD5);
    DDRD |= (1 << PD7);

    PORTD &= ~(1 << PD5);
    PORTD |= (1 << PD7);

    int index = 0;
    const char *str = "pieton";

    for (;;) {
        char letter = USART0_receive();

        if (letter == str[index]) {
            if (index == 5) {
                PORTD &= ~(1 << PD5);
                PORTD &= ~(1 << PD7);

                _delay_ms(2000);

                PORTD |= (1 << PD5);
                PORTD &= ~(1 << PD7);

                _delay_ms(5000);

                PORTD &= ~(1 << PD5);
                PORTD |= (1 << PD7);
            }

            index++;
        } else {
            index = 0;
            USART0_print("cerere incorecta!\n");
        }
    }
}

// int main()
// {
//     USART0_init(BAUD);

//     t4();

//     return 0;
// }

#include <Arduino.h>

#define RED_LED 13
#define GREEN_LED 15
#define BLUE_LED 3
#define BTN_PB2 2
#define BTN_PD6 14

/* task1() */
void setup1()
{
    pinMode(BTN_PB2, INPUT_PULLUP);
}

void loop1()
{
    char button = digitalRead(BTN_PB2);

    if (!button)
        Serial.println("Butonul 1 a fost apasat!");

    delay(500);
}

void arduino_exec(char cmd)
{
    switch (cmd) {
        /* Light off the LEDs */
        case '0': {
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(BLUE_LED, HIGH);
            break;
        }
        /* Light on the LEDs */
        case '1': {
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(BLUE_LED, LOW);
            break;
        }
        /* Light on only the red LED */
        case 'r': {
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(BLUE_LED, HIGH);
            break;
        }
        /* Light on only the green LED */
        case 'g': {
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(BLUE_LED, HIGH);
            break;
        }
        /* Light on only the blue LED */
        case 'b': {
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(BLUE_LED, LOW);
            break;
        }
    }
}

/* task2() */
void setup2()
{
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    /* Light off all LEDs */
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
}

void loop2()
{
    if (Serial.available()) {
        char cmd = Serial.read();

        USART_exec(cmd);
    }
}

/* task3() */
void setup3()
{
    
}

void loop3()
{
    if (Serial.available()) {
        char letter = Serial.read();

        if (letter >= 'a' && letter <= 'z') {
            char *morse = morse_alphabet[letter - 'a'];

            while (*morse != '\0') {
                speaker_morse(*morse - '0');
                morse++;
            }
        }
    }
}

void setup()
{
    Serial.begin(BAUD);

    setup3();
}

void loop()
{
    loop3();
}
