/*
 * File:   led_blink.c
 * Author: vysak
 *
 * Created on 12 August, 2023, 3:32 PM
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void) {
    DDRB |= (1<<PB5);
    while(1)
    {
        PORTB |= (1<<PB5);
        _delay_ms(100);
        PORTB &= (~(1<<PB5));
        _delay_ms(100);
        
    }
    return 0;
}
