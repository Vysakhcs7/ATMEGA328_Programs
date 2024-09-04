/* 
 * File:   UART.c
 * Author: Vysakh
 *
 * Created on 21 August, 2023, 4:28 PM
 */

//----------------------------------------------------------------------
#include<avr/io.h>
void UART_init(void);
void UART_Send_data(char *string);
//----------------------------------------------------------------------

int main() {
    UART_init();
    while (1) {
        UART_Send_data("Hello from ATmega328p\n\r");
    }
}
//----------------------------------------------------------------------
//Function to initialize UART communication
//----------------------------------------------------------------------

void UART_init(void) {

    UCSR0B |= (1 << TXEN0); //| (1 << RXEN0);
    UCSR0B &= (~(1 << UCSZ02));
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    UBRR0 = 103;
}
//----------------------------------------------------------------------
//Function to send character to the console
//----------------------------------------------------------------------

void UART_Send_data(char *string) {

    while ((*string) != '\0') {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = *string;
        string++;
    }
}
//----------------------------------------------------------------------