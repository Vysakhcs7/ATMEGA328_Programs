#include<avr/io.h>
#include<avr/interrupt.h>
void T1Delay();

int main()
{

DDRB = 0xFF;

//PORTB output port

while (1)
{

PORTB ^= (1<<PB5); //toggle PB4

T1Delay(); 
}
}//delay size unknown

void T1Delay()
{
TCNT1 = 49911;
//TEMP = 0xC1

TCCR1A = 0x00;
TCCR1B = 0x05;

//Normal mode //Normal mode, no prescaler

while ((TIFR1& (0x1<<TOV1))==0); //wait for TOVI to roll over

TCCR1B = 0;
TIFR1 = 0x1<<TOV1;

}