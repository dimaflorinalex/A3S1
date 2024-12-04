#include "usart.h"
#include "avr/io.h"

/* Init USART controller */
void USART0_init(unsigned int baud)
{
    /* The value to put inside the UBRR register to set the baud rate */
    unsigned int ubrr = F_CPU / baud / 16 - 1;

    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    /* Enable receiver and transmitter */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    /* Set frame format: 8 data bits, 2 stop bits and no parity */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

/* Transmit a char through USART */
void USART0_transmit(char data)
{
    /* Wait until the buffer is empty */
    while (!(UCSR0A & (1 << UDRE0)));

    /* Put the char in buffer. The transmission will automatically start */
    UDR0 = data;
}

/* Transmit a null terminated string through USART */
void USART0_print(const char *data)
{
    while (*data != '\0')
        USART0_transmit(*data++);
}

/* Receive a char through USART */
char USART0_receive()
{
    while (!(UCSR0A & (1 << RXC0)));

    /* Return data from buffer */
    return UDR0;
}
