#include <util/delay.h>
#include "servo.h"


int main(void)
{
    uint8_t angle = SERVO_MIN_ANGLE;
    uint8_t direction = 0;

    servo_init();

    for (;;) {
        _delay_ms(100);
        servo_setAngle(angle);

        if (angle == SERVO_MIN_ANGLE || angle == SERVO_MAX_ANGLE) {
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
