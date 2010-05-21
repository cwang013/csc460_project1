/*
 * spi.c
 *
 *  Created on: 24-Jan-2009
 *      Author: Neil MacMillan
 *
 *  Functions for using the AT90 as an SPI master.
 */

#include "spi.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define SPI_DDR	 DDRB    	// DDR of SPI port
#define SPI_PORT PORTB  	// SPI port
#define SPI_MOSI PB2 		// MOSI pin (Master out, Slave in)
#define SPI_MISO PB3 		// MISO pin (Master in, Slave out)
#define SPI_SCK PB1  		// SCK pin (SPI clock)
#define SPI_SS PB0   		// SS pin (Slave Select)

// wait for an SPI read/write operation to complete
#define SPI_WAIT()              while (!(SPSR & _BV(SPIF)));

void SPI_Init()
{
	// set MOSI, SCK, and SS output, MISO input
	SPI_DDR |= _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_SS) | ~(_BV(SPI_MISO));

	SPI_PORT |= _BV(SPI_SS);

	SPCR |= _BV(SPE) | _BV(MSTR);
}

void SPI_ReadWrite_Block(uint8_t* data, uint8_t* buffer, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++) {
          SPDR = data[i];
          SPI_WAIT();
          buffer[i] = SPDR;
    }
}

void SPI_Write_Block(uint8_t* data, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++) {
          SPDR = data[i];
          SPI_WAIT();
    }
}

uint8_t SPI_Write_Byte(uint8_t byte)
{
    SPDR = byte;
    SPI_WAIT();
    return SPDR;
}
