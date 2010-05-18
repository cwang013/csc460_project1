#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

#define SERVO_MIN_ANGLE 60
#define SERVO_MAX_ANGLE 300

void Servo_init(uint8_t pin);
uint8_t Servo_getAngle();
void Servo_setAngle(uint8_t angle);

#endif
