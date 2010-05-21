/*
 * radio.h
 *
 *  Created on: 24-Jan-2009
 *      Author: Neil MacMillan
 *
 *  References:
 *    Ball, Brennen.  DIYEmbedded Tutorials.  http://www.diyembedded.com/
 *    Engelke, Stefan.  AVR-Lib/nRF24L01.  http://www.tinkerer.eu/AVRLib/nRF24L01
 *    Nordic Semiconductor.  nRF24L01 Product Specification 2.0.  http://www.nordicsemi.no/files/Product/data_sheet/nRF24L01_Product_Specification_v2_0.pdf
 *
 *    Most of the hard work for this was done by [Engelke].
 */

#ifndef RADIO_H_
#define RADIO_H_

#include <stddef.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "nRF24L01.h"
#include "packet.h"
#include "spi.h"

#define RADIO_ADDRESS_LENGTH 5

typedef enum _radio_pipe {
        RADIO_PIPE_0 = 0,
        RADIO_PIPE_1 = 1,
        RADIO_PIPE_2 = 2,
        RADIO_PIPE_3 = 3,
        RADIO_PIPE_4 = 4,
        RADIO_PIPE_5 = 5,
        RADIO_PIPE_EMPTY = 7,   // FIFO is empty when pipe number bits in status register are 0b111.
} RADIO_PIPE;

typedef enum _radio_tx_power {
        RADIO_LOWEST_POWER = 0,         // -18 dBm (about 16 uW)
        RADIO_LOW_POWER = 1,            // -12 dBm (about 63 uW)
        RADIO_HIGH_POWER = 2,           // -6 dBm (about 251 uW)
        RADIO_HIGHEST_POWER = 3,        // 0 dBm (1 mW)
} RADIO_TX_POWER;

typedef enum _radio_dr {
        RADIO_1MBPS = 0,                // that's Mbps, not MBps.
        RADIO_2MBPS = 1,
} RADIO_DATA_RATE;

typedef enum _radio_receive {
        RADIO_RX_INVALID_ARGS,          // one of the arguments to Radio_Receive was invalid
        RADIO_RX_TRANSMITTING,          // the radio was transmitting
        RADIO_RX_FIFO_EMPTY,            // there aren't any packets in the Rx FIFO to receive (Radio_Receive does not receive data)
        RADIO_RX_MORE_PACKETS,          // after copying out the head of the Rx FIFO, there is still another packet in the FIFO.
        RADIO_RX_SUCCESS,               // there was a packet to receive, it was successfully received, and the Rx FIFO is now empty.
} RADIO_RX_STATUS;

typedef enum _radio_transmit {
        RADIO_TX_MAX_RT,
        RADIO_TX_SUCCESS,
} RADIO_TX_STATUS;

typedef enum _radio_tx_wait {
        RADIO_WAIT_FOR_TX,
        RADIO_RETURN_ON_TX,
} RADIO_TX_WAIT;

typedef enum _radio_ack {
        RADIO_ACK,
        RADIO_NO_ACK,
} RADIO_USE_ACK;

typedef enum _ed {
        DISABLE=0,
        ENABLE=1,
} ON_OFF;               // there's got to be a better name for this.

void Radio_Init();
void Radio_Configure_Rx(RADIO_PIPE pipe, uint8_t* address, ON_OFF enable);
void Radio_Configure(RADIO_DATA_RATE dr, RADIO_TX_POWER power);
void Radio_Set_Tx_Addr(uint8_t* address);
uint8_t Radio_Transmit(radiopacket_t* payload, RADIO_TX_WAIT wait);
RADIO_RX_STATUS Radio_Receive(radiopacket_t* buffer);
uint8_t Radio_Drop_Rate();

#endif /* RADIO_H_ */
