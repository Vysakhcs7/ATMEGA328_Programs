/*
 * main.c
 *
 * Created: 7/31/2023 12:02:46 PM
 *  Author: Vysakh C S
 */ 

#include <xc.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include<util/delay.h>

ISR(INT0_vect);
ISR(INT1_vect);


int main(void)
{
	DDRB |= (1<<DDB1); //PB1 as Output
	DDRC |= (1<<DDC5); //PC5 as Output
	DDRD &= (~(1<<DDD2)) & (~(1<<DDD3)); //D2 AND D3 as Input (INT0 pin 4, INT1 pin 5)
	PORTD |= (1<<DDD2) | (1<<DDD3); //D2 and D3 pull up set
	EICRA = 0b00001011;
	EIMSK |= (1<<INT1) | (1<<INT0);
	SREG |= (1<<7); 
    while(1)
	{
		PORTC ^= (1<<PORTC5);
		_delay_ms(100);
		
	}
	return 0;
}

ISR(INT0_vect)
{
	PORTB |= (1<<PORTB1);	
}

ISR(INT1_vect)
{
	PORTB &= (~(1<<PORTB1));
}