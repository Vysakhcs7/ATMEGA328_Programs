/*
 * File:   USART.c
 * Author: Vysakh C S
 *
 * Created on 12 October, 2023, 4:29 PM
 */

//----------------------------------------------------------------------
#include "USART.h"
//----------------------------------------------------------------------
//Function to initialize UART communication
//---------------------------------------------------------------------- 

void UART_init(void) {
    UBRR0H = 0;
    UBRR0L = 8; //Baud rate 115200
    //UBRR0 = 103; //baud rate 9600
    // Enable transmitter and receiver
    UCSR0B |= (1 << TXEN0); //| (1 << RXEN0);
    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0B &= (~(1 << UCSZ02));
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

}
//----------------------------------------------------------------------
//Function to send character to the console
//----------------------------------------------------------------------

void USART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
    UDR0 = data; // Put data into buffer and send
}
//----------------------------------------------------------------------