/*
 * packet.h
 *
 *  Created on: 26-Apr-2009
 *      Author: Neil MacMillan
 *  Modified by: Heng Lien
 */

#ifndef PACKET_H_
#define PACKET_H_

#include <avr/io.h>
#include "sensor_struct.h"

/*  Add labels for the packet types to the enumeration */
typedef enum _pt
{
        COMMAND,                        
        SENSOR_DATA,            
} PACKET_TYPE;

/* labels for command type */
typedef enum _cmdtype {
	Trigger,
	Ping,
} COMMAND_TYPE;

/* Construct payload format structures */
// structures must be 29 bytes long or less.
typedef struct _cmd
{
        uint8_t sender_address[5];      
        uint8_t command;                        

        uint8_t num_arg_bytes;          
        uint8_t arguments[16];          

} pf_command_t;


typedef struct _sensors
{
        roomba_sensor_data_t sensors;   

} pf_sensors_t;

/*****                                                  Add format structures to the union                                                      *****/

typedef union _pf
{
        uint8_t _filler[29];    // makes sure the packet is exactly 32 bytes long - this array should not be accessed directly.
        pf_command_t command;
        pf_sensors_t sensors;
} payloadformat_t;

/*****                                          Leave the radiopacket_t structure alone.                                                *****/

typedef struct _rp
{
        PACKET_TYPE type;
        uint16_t timestamp;
        payloadformat_t payload;
} radiopacket_t;

#endif /* PACKET_H_ */
