#include <util/delay.h>
#include "servo.h"


int main(void)
{
    servo_init();

    for (;;) {
        //_delay_ms(1000);
        //servo_setAngle(160);
        //_delay_ms(1000);
        //servo_setAngle(200);
    }

    return 0;
}
