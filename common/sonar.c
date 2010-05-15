/*
 * sonar.c
 *
 *  Created on: 2010-05-12
 *      Author: lienh
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "sonar.h"

static volatile uint16_t trigger_time = 0;
static volatile uint16_t echo_time = 0;
static volatile uint8_t received = 0;

uint16_t _get_TCNT3();

void sonar_init()
{
	// set PD5 for output
	DDRD |= _BV(PORTD5);

	// set timer3 to run 1/8 of the microprocessor's clock frequency
	TCCR1B &= ~_BV(CS10);
	TCCR1B |= _BV(CS11);
	TCCR1B &= ~_BV(CS12);

	// enable noise canceler for input capture
	TCCR1B |= _BV(ICNC1);

	// set input capture on rising edge
	TCCR1B |= _BV(ICES1);

	// clear input capture flag
	TIFR1 |= _BV(ICF1);

	// enable input capture interrupt
	TIMSK1 |= _BV(ICIE1);

	sei();
}

void sonar_trigger()
{
	// trigger a 10 us pulse
	PORTD |= _BV(PD5);
	_delay_us(10);
	PORTD &= ~_BV(PD5);
}

uint8_t sonar_echo_received()
{
	char sreg = SREG;
	cli();
	uint8_t temp = received;
	received = 0;
	SREG = sreg;
	return temp;
}

uint16_t sonar_get_distance()
{
	return trigger_time - echo_time;
}

ISR(TIMER1_CAPT_vect)
{
	if (TCCR1B & _BV(ICES1)) { // a rising edge is captured
		echo_time = ICR1;

		// set input capture on falling edge
		TCCR1B &= ~_BV(ICES1);

		// clear input capture flag
		TIFR1 |= _BV(ICF1);

	} else { // a falling edge is captured
		received = 1;
		trigger_time = ICR1;

		// set input capture on rising edge
		TCCR1B |= _BV(ICES1);

		// clear input capture flag
		TIFR1 |= _BV(ICF1);
	}
}
