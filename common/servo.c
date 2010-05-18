#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo.h"

static uint16_t _angle = 180;


void servo_init()
{
    uint8_t sreg;
    /*
     * set non-inverted PCM mode (COM4B1 set, COM4B0 unset),
     * mode 14 (fast PWM, TOP in ICR4) * set prescale to 64 - CS 40, 41 set, 42 unset
     */
    TCCR4A = 0b100010; // COM4B1, COM4B0, COM4C1, COM4C0, WGM41, and WGM40
    TCCR4B = 0b11010; // WGM43, WGM42, CS42, CS41, and CS40

    // OC4B–PortH,Bit4
    /* OC4B, Output Compare Match B output: The PH4 pin can serve as an external 
     * output for the Timer/Counter2 Output Compare B. The pin has to be configured as 
     * an output (DDH4 set) to serve this function. The OC4B pin is also the output 
     * pin for the PWM mode timer function.
     */
    // set Port H, pin 4 as output
    DDRH |= _BV(DDH4);

    /*
     * Set top to 40000 (which will be 20ms with the prescaler)
     */
    sreg = SREG; // save interrupts
    cli(); // disable interrupts
    ICR4 = 40000;
    SREG = sreg; // restore interrupts

    /*
     * the time it should be up for no movement is 3054, this
     * is just an initial value. it gets set to servoValue the
     * first time the interrupt is hit.
     */
    sreg = SREG;
    cli();
    OCR4B = (unsigned int)1500;
    SREG = sreg;

    return;
}


uint16_t servo_getAngle()
{
    return _angle;
}


void servo_setAngle(uint16_t angle)
{
    uint8_t sreg;

    if (angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) {
        return;
    }
    _angle = angle;
    uint16_t servoValue = 1500; // TODO calculate servoValue from angle
    sreg = SREG; // save interrupts
    cli(); // clear (disable) interrupts
    OCR4B = servoValue; // set pulse width for port H, pin 4
    SREG = sreg; // restore interrupts
}