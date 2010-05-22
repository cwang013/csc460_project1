#include <avr/interrupt.h>
#include <util/delay.h>
#include "servo.h"


int main(void)
{
    uint8_t angle = SERVO_ANGLE_MIN;
    uint8_t direction = 0;

    // disable global system prescaler
    // this makes the effective clock speed F_CPU
    CLKPR = _BV(CLKPCE);
    CLKPR = 0x00;

    servo_init();

    for (;;) {
        _delay_ms(100);
        servo_setAngle(angle);

        if (angle == SERVO_ANGLE_MIN || angle == SERVO_ANGLE_MAX) {
            direction = !direction;
        }

        if (direction) {
            angle += 5;
        } else {
            angle -= 5;
        }
    }

    return 0;
}
