/*
 * File:   TIMER.c
 * Author: Vysakh C S
 *
 * Created on 3 October, 2023, 2:04 PM
 */
//----------------------------------------------------------------------------------------------

#define F_CPU 16000000UL // 16MHz clock frequency
#include <avr/io.h>
void T0_delay(void);
int timerOverflowCount = 0;
//----------------------------------------------------------------------------------------------

int main(void) {
    DDRB |= (1 << DDB5); //Setting PIN5 of PORTB as OUTPUT
    while (1) {
        T0_delay(); //calling the delay function    }
    }
    return 0;
}
//----------------------------------------------------------------------------------------------

void T0_delay(void) {
    //Timer calculation
    //Operating frequency = 16MHz
    //Required delay = 1 ms
    //Prescaler value = 64
    //Timer clock frequency = Operating frequency/Prescaler value = 16MHz / 64 = 250KHz
    //Time period for each timer tick = 1/Timer clock frequency = 1/250KHz = 4us
    //Number of timer cycles needed to generate delay = Time delay required/Time taken for each
    //1ms / 4us = 250
    //256 - 250 = 6 which is 0x06 in hex

    TCNT0 = 0x06; //initial value for the timer
    TCCR0B |= (1 << CS01) | (1 << CS00); //For setting prescaler as 64
    TCCR0B &= (~(1 << CS02)); //For setting prescaler as 64
    while (!(TIFR0 & (1 << TOV0))); //monitoring the timer overflow flag to see if it is raised.
    TCNT0 = 0x06; //loading the initial value for the timer
    TIFR0 |= (1 << TOV0); //clearing the timer overflow flag
    timerOverflowCount++; //incrementing the counter variable
    if (timerOverflowCount >= 100) { //checking to see if it is 1 second
        PORTB ^= (1 << PORTB5); //toggle the PIN5 of PORTB
        timerOverflowCount = 0; //Reset the counter variable
    }
}
//----------------------------------------------------------------------------------------------