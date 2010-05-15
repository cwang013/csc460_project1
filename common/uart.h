#ifndef __UART_H__
#define __UART_H__
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum _uart_bps
{
        UART_38400,
        UART_57600,
        UART_DEFAULT,
} UART_BPS;

#define UART_BUFFER_SIZE 100			// size of Rx ring buffer.

volatile uint8_t uart_rx; 		// Flag to indicate uart received a byte

void uart_init(UART_BPS bitrate);
void uart_putchar(char c);
char uart_getchar(int index);
void uart_putstr(char *s);

uint8_t uart_bytes_recv(void);
void uart_reset_recv(void);
#endif
