#ifndef _USART_H_
#define _USART_H_

#include <stdint.h>

/* Init USART controller */
void USART0_init(unsigned int baud);

/* Transmit a char through USART */
void USART0_transmit(char data);

/* Transmit a null terminated string through USART */
void USART0_print(const char *data);

/* Receive a char through USART */
char USART0_receive();

#endif // _USART_H_