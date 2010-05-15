#include <avr/io.h>
#include <stdio.h>			
#include <util/delay.h>
#include "uart.h"				// routines for UART comms.

#define MAXLINE			81
#define CLOCK_8MHZ()	CLKPR = _BV(CLKPCE); CLKPR = 0x00;
int main()
{
	char buf[MAXLINE];
	char byte;

	CLOCK_8MHZ();
	uart_init(UART_38400);

	snprintf(buf, MAXLINE, "HELLO WORLD: starts echoing\n\r");
	uart_putstr(buf);
	
	for(;;) // Loop forever
	{
		if (uart_rx == 1) {
			byte = uart_getchar(uart_bytes_recv()-1);
			uart_rx = 0;
			_delay_ms(100);
			uart_putchar(byte);			
		}	
	}
}
