#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

#define SERVO_MIN_ANGLE (-45)
#define SERVO_MAX_ANGLE 45

void servo_init();
int16_t servo_getAngle();
void servo_setAngle(int16_t angle);

#endif
