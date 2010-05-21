/*
 * main.c
 *
 *  Created on: 2010-05-12
 *      Author: lienh
 */
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "radio.h"
#include "sonar.h"

#define     clock8MHz()    CLKPR = _BV(CLKPCE); CLKPR = 0x00;

uint8_t station_addr[5] = { 0x98, 0x76, 0x54, 0x32, 0x10 };

// Note, change to a unique address
uint8_t remote_addr[5] = {0x99, 0x76, 0x54, 0x32, 0x10};

volatile uint8_t rxflag = 0;
volatile float distance = 0;

radiopacket_t packet;

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

	Radio_Init();
	Radio_Configure_Rx(RADIO_PIPE_0, remote_addr, ENABLE);
	Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);

	// enable interrupts
	sei();

	// print a message to UART to indicate that the program has started up
	snprintf((char*)output, 128, "STATION START\n\r");
	print(output);

	packet.type = COMMAND;
	packet.payload.command.sender_address[0] = remote_addr[0];
	packet.payload.command.sender_address[1] = remote_addr[1];
	packet.payload.command.sender_address[2] = remote_addr[2];
	packet.payload.command.sender_address[3] = remote_addr[3];
	packet.payload.command.sender_address[4] = remote_addr[4];

	for (;;)
	{

		if (rxflag) {
			if (Radio_Receive(&packet) != RADIO_RX_MORE_PACKETS)
			{
				// if there are no more packets on the radio, clear the receive flag;
				// otherwise, we want to handle the next packet on the next loop iteration.
				rxflag = 0;
			}

			snprintf((char*)output, 128, "RECV message\n\r");
			print(output);
		}

		// Set the transmit address to the one specified in the message packet.
		Radio_Set_Tx_Addr(station_addr);
		Radio_Transmit(&packet, RADIO_WAIT_FOR_TX);
		_delay_ms(1000);
	}

	return 0;
}

/**
 * This function is a hook into the radio's ISR.  It is called whenever the radio generates an RX_DR (received data ready) interrupt.
 */
void radio_rxhandler(uint8_t pipenumber)
{
	rxflag = 1;
}

