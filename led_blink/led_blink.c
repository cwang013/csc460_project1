#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB = 0xFF;
    PORTB = 0xFF;

    for (;;) {
        PORTB = ~(_BV(PINB0) | _BV(PINB2) | _BV(PINB4) | _BV(PINB6));
        _delay_ms(500);
        PORTB = 0xFF;
        _delay_ms(500);
    }
}
