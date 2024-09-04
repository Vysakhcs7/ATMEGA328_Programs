/* 
 * File:   voltage_divider.c
 * Author: Vysakh
 *
 * Created on 21 August, 2023, 6:11 PM
 */


#include<avr/io.h>
void ADC_init();
void UART_init();

int main() {
    UART_init();
    ADC_init();
uint16_t analogVal = 0;
    while (1) {
        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC));
        //analogVal = ADCL | (ADCH << 8);
        analogVal = ADC;
        for(int i = 0; i< 5; i++)
        {
            while (!(UCSR0A & (1 << UDRE0)));
            UDR0 = analogVal;
        }
    }
}
void ADC_init() {
    ADMUX |= (1 << REFS0); //default Ch-0; Vref = 5V
    //ADMUX &= (~(1<<ADLAR));
    // ADMUX |= (1<<ADLAR);
    ADCSRA |= (1 << ADEN) | (0 << ADSC) | (0 << ADATE); //auto-trigger OFF
    ADCSRB = 0x00;
}

void UART_init() {
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
    UCSR0B &= (~(1 << UCSZ02));
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    UBRR0 = 103;
}