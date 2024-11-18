#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"

#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7
#define DDR_SPI DDRB

void SPI_init(){
	/* TODO1: set MOSI and SCK output, all others input */
	DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK);

	/* TODO1: set SS PIN as output and set it on HIGH */
	DDRB |= (1 << PB4);
	PORTB |= (1 << PB4);

	/* TODO1: enable SPI, set it as Master, set clock rate at fosc/16 */
	SPCR0 = (1 << SPE0) | (1 << MSTR0) | (1 << SPR00);
}

uint8_t SPI_exchange(uint8_t data){
	// TODO1: send a byte of data to the slave and return the response byte received from him in this transmission

	/* Start transmission */
	SPDR0 = data;

	/* Wait for transmission complete */
	while (!(SPSR0 & (1 << SPIF0)));
	
	return SPDR0;
}

