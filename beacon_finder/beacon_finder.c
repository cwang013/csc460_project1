#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "radio.h"
#include "sonar.h"
#include "servo.h"

typedef struct _beaconInfo_t {
    uint16_t pingTime;
    int16_t angle;
} beaconInfo_t;

// the beacon address
uint8_t station_addr[5] = { 0x98, 0x76, 0x54, 0x32, 0x10 };

// our unique address
uint8_t remote_addr[5] = {0xb0, 0x00, 0x00, 0xb1, 0xe5};

// string buffer for printing to UART
static uint8_t output[128];

volatile uint8_t rxflag = 0;

static void print(uint8_t* output);
static uint16_t ping(radiopacket_t *packet); // returns time in usec
static void scanForBeacon(radiopacket_t *packet, beaconInfo_t *beaconInfo);


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
    //packet->payload.command.command = Trigger;
    packet->payload.command.command = Ping; // Ping and Trigger seem to be equivalent
    Radio_Transmit(packet, RADIO_WAIT_FOR_TX);

    // try to receive packet
    for (uint16_t numTries = 0; !rxflag && numTries < UINT16_MAX; numTries++);
    if (!rxflag) {
        snprintf((char*)output, 128, "radio failed\n\r");
        print(output);
        return UINT16_MAX;
    }
    sonar_trigger(); // start listening for a sonar ping
    while (Radio_Receive(packet) == RADIO_RX_MORE_PACKETS); // throw away the packets
    rxflag = 0;

    _delay_ms(50); // no valid sonar echo takes longer than 50 ms
    if (sonar_echo_received()) {
        uint16_t distance = sonar_get_distance();
        snprintf((char*)output, 128, "time %u us\n\r", distance);
        print(output);
        return distance;
    } else {
        snprintf((char*)output, 128, "sonar failed\n\r");
        print(output);
        return 0;
    }
}


static void scanForBeacon(radiopacket_t *packet, beaconInfo_t *beaconInfo)
{
    int16_t angle = SERVO_ANGLE_MIN;

    beaconInfo->angle = angle;
    beaconInfo->pingTime = UINT16_MAX;

    servo_setAngle(angle);
    _delay_ms(1000); // allow servo time to rotate

    for (;;) {
        uint16_t pingTime;

        snprintf((char*)output, 128, "pinging at %d degrees... ", angle);
        print(output);
        pingTime = ping(packet);
        if (pingTime < beaconInfo->pingTime && pingTime > 0) {
            beaconInfo->angle = angle;
            beaconInfo->pingTime = pingTime;
        }
        angle += 1;
        if (angle > SERVO_ANGLE_MAX) {
            break;
        }
        servo_setAngle(angle);
        _delay_ms(400); // allow servo time to rotate
    }
}


int main()
{
    radiopacket_t packet;

    // disable global system prescaler
    // this makes the effective clock speed F_CPU
    CLKPR = _BV(CLKPCE);
    CLKPR = 0x00;

    // disable interrupts during setup
    cli();

    uart_init(UART_38400);

    Radio_Init();
    Radio_Configure_Rx(RADIO_PIPE_0, remote_addr, ENABLE);
    Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);

    servo_init();

    sonar_init();

    // enable interrupts
    sei();

    packet.type = COMMAND;
    packet.payload.command.sender_address[0] = remote_addr[0];
    packet.payload.command.sender_address[1] = remote_addr[1];
    packet.payload.command.sender_address[2] = remote_addr[2];
    packet.payload.command.sender_address[3] = remote_addr[3];
    packet.payload.command.sender_address[4] = remote_addr[4];

    for (;;) {
        beaconInfo_t beaconInfo;
        uint16_t distance;

        snprintf((char*)output, 128, "\n\rLet's find the beacon!\n\r");
        print(output);
        scanForBeacon(&packet, &beaconInfo);
        snprintf((char*)output, 128, "The beacon has been found:\n\r");
        print(output);

        servo_setAngle(beaconInfo.angle);
        _delay_ms(1000); // allow servo time to rotate

        // cm = 0.034489 * µsec - 14.989621 (linear regression on experimental data)
        distance = (uint16_t)(0.034489 * beaconInfo.pingTime - 14.989621);

        snprintf((char*)output, 128, "time: %u us\n\r", beaconInfo.pingTime);
        print(output);
        snprintf((char*)output, 128, "distance: %u cm\n\r", distance);
        print(output);
        snprintf((char*)output, 128, "angle: %d degrees\n\r", beaconInfo.angle);
        print(output);
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
