#include <avr/io.h>				
#include <avr/interrupt.h>		// ISR handling.
#include "uart.h"

/*
 Global Variables:
 Variables appearing in both ISR/Main are defined as 'volatile'.
*/
static volatile int rxn; // buffer 'element' counter.
static volatile char rx[UART_BUFFER_SIZE]; // buffer of 'char'.

void uart_putchar (char c)
{
	cli();
	while ( !( UCSR0A & (1<<UDRE0)) ); // Wait for empty transmit buffer           
	UDR0 = c;  // Putting data into the buffer, forces transmission
	sei();
}

char uart_getchar (int index)
{
	if (index < UART_BUFFER_SIZE) {
		return rx[index];
	}
	return 0;
}

void uart_putstr(char *s)
{
	while(*s) uart_putchar(*s++);
	
}

void uart_init(UART_BPS bitrate){

	DDRB = 0xff;
	PORTB = 0xff;

	rxn = 0;
	uart_rx = 0;

	/* Set baud rate */
	UBRR0H = 0;
	switch (bitrate) {
    case UART_38400:
	    UBRR0L = 12;
		break;
    case UART_57600:
        UBRR0L = 6;
        break;
    default:
        UBRR0L = 6;
    }

	/* Enable receiver and transmitter */
	UCSR0B = _BV(RXEN0)|_BV(TXEN0) | _BV(RXCIE0);

	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(1<<UCSZ00) | _BV(UCSZ01);
}

uint8_t uart_bytes_recv(void)
{
	return rxn;
}

void uart_reset_recv(void)
{
	rxn = 0;
}

/*
 Interrupt Service Routine (ISR):
*/

ISR(USART0_RX_vect)
{
	while ( !(UCSR0A & (1<<RXC0)) );

	//PORTB = ~_BV(PINB1);

	rx[rxn] = UDR0;
	rxn = (rxn + 1) % UART_BUFFER_SIZE;
	uart_rx = 1; // notify main of receipt of data.
	//PORTB = PORTB | _BV(PINB1);
}


