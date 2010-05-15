/*
 * main.c
 *
 *  Created on: 2010-05-12
 *      Author: lienh
 */
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"
#include "sonar.h"

#define     clock8MHz()    CLKPR = _BV(CLKPCE); CLKPR = 0x00;

volatile uint8_t rxflag = 0;
volatile float distance = 0;

void print(uint8_t* output)
{
	uint8_t i;
	for (i = 0; i < 64 && output[i] != 0; i++)
	{
		uart_putchar(output[i]);
	}
}

int main()
{
	// string buffer for printing to UART
	uint8_t output[128];

	// disable the clock prescaler
	clock8MHz();

	// disable interrupts during setup
	cli();

	uart_init(UART_38400);
	sonar_init();

	// enable interrupts
	sei();


	for (;;)
	{
		sonar_trigger();
		_delay_ms(36);
		if (sonar_echo_received() == 1) {
			distance = 0.0171 * sonar_get_distance() -  0.8192;
			snprintf((char*)output, 128, "distance %d\n\r", (int)distance);
			print(output);
		}else {
			snprintf((char*)output, 128, "sonar failed\n\r");
			print(output);
		}

		_delay_ms(1000);
	}

	return 0;
}
