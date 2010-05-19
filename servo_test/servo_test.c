#include <util/delay.h>
#include "servo.h"


int main(void)
{
	uint8_t angle = 0;
	uint8_t direction = 1;

    servo_init();

	for (;;) {
		_delay_ms(100);
		servo_setAngle(angle);

		if (angle == 0 || angle == 180) {
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
