#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

#define SERVO_MIN_ANGLE 60
#define SERVO_MAX_ANGLE 300

void servo_init();
uint16_t servo_getAngle();
void servo_setAngle(uint16_t angle);

#endif
