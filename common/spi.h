/*
 * spi.h
 *
 *  Created on: 24-Jan-2009
 *      Author: Neil MacMillan
 *
 *  Macros and prototypes for using the AT90 as an SPI master.
 *
 *  References:
 *    Engelke, Stefan.  AVR-Lib/SPI.  http://www.tinkerer.eu/AVRLib/SPI
 *    Tanner, Justin.  LED Wheel: Code.  http://webhome.csc.uvic.ca/~mcheng/samples/led_wheel/wheel_demo.zip
 *    Atmel.  AT90USB1287 Hardware Manual.  http://www.atmel.com/dyn/resources/prod_documents/doc7593.pdf
 *
 *    All the hard work for this was done by [Engelke].
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void SPI_Init();
void SPI_ReadWrite_Block (uint8_t * data, uint8_t * buffer, uint8_t len);
void SPI_Write_Block (uint8_t * data, uint8_t len);
uint8_t SPI_Write_Byte (uint8_t data);

#endif /* SPI_H_ */
