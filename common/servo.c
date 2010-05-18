#include "servo.h"

static uint8_t _angle = 180;


void Servo_init(uint8_t pin)
{
    // TODO
}


uint8_t Servo_getAngle()
{
    // TODO
    return 180;
}


void Servo_setAngle(uint8_t angle)
{
    if (angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) {
        return;
    }
    _angle = angle;
    // TODO
}
