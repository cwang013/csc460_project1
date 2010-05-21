#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "radio.h"
#include "sonar.h"
#include "servo.h"

#define     clock8MHz()    CLKPR = _BV(CLKPCE); CLKPR = 0x00;

// the beacon address
uint8_t station_addr[5] = { 0x98, 0x76, 0x54, 0x32, 0x10 };

// our unique address
uint8_t remote_addr[5] = {0xb0, 0x00, 0x00, 0xb1, 0xe5};

// string buffer for printing to UART
static uint8_t output[128];

volatile uint8_t rxflag = 0;

static void print(uint8_t* output);
static uint16_t ping(radiopacket_t *packet);
static uint16_t scanForBeacon(radiopacket_t *packet); // returns distance in cm


static void print(uint8_t* output)
{
	uint8_t i;
	for (i = 0; i < 64 && output[i] != 0; i++) {
		uart_putchar(output[i]);
	}
}


static uint16_t ping(radiopacket_t *packet)
{
		// Set the transmit address to the one specified in the message packet.
		Radio_Set_Tx_Addr(station_addr);
        packet->payload.command.command = Trigger;
        Radio_Transmit(packet, RADIO_WAIT_FOR_TX);

        // wait to receive packet
        while (!rxflag);
        while (Radio_Receive(packet) == RADIO_RX_MORE_PACKETS); // throw away the packets
        rxflag = 0;

        // TODO: get sonar ping
        _delay_ms(50); // TODO: get rid of this

        snprintf((char*)output, 128, "PONG!!!\n\r");
        print(output);

        return 100;
}


static uint16_t scanForBeacon(radiopacket_t *packet)
{
    uint8_t angle = SERVO_MIN_ANGLE;
    uint8_t bestAngle = SERVO_MIN_ANGLE;
    uint16_t shortestDistance = UINT16_MAX;

    servo_setAngle(angle);
    _delay_ms(1000); // allow servo time to rotate
    
    for (;;) {
        uint16_t distance;
        
        distance = ping(packet);
        if (distance < shortestDistance && distance > 0) {
            bestAngle = angle;
            shortestDistance = distance;
        }
        angle += 5;
        if (angle > SERVO_MAX_ANGLE) {
            break;
        }
        servo_setAngle(angle);
        _delay_ms(200); // allow servo time to rotate
    }

    servo_setAngle(bestAngle);
    _delay_ms(1000); // allow servo time to rotate

    return shortestDistance;
}


int main()
{
    radiopacket_t packet;

    // disable the clock prescaler
    clock8MHz();

    // disable interrupts during setup
    cli();

    uart_init(UART_38400);

    Radio_Init();
    Radio_Configure_Rx(RADIO_PIPE_0, remote_addr, ENABLE);
    Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);

    servo_init();

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

    for (;;) {
        uint16_t distance;
        
        distance = scanForBeacon(&packet);
        snprintf((char*)output, 128, "Distance: %u\n\r", distance);
        _delay_ms(5000);
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

