#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

#define SERVO_ANGLE_MIN (-45)
#define SERVO_ANGLE_MAX 45

void servo_init();
int16_t servo_getAngle();
void servo_setAngle(int16_t angle);

#endif
