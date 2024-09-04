/* 
 * File:   USART.h
 * Author: Vysakh C S
 *
 * Created on 12 October, 2023, 4:28 PM
 */
//----------------------------------------------------------------------
#ifndef USART_H
#define	USART_H
//----------------------------------------------------------------------
#include "TWI.h"
//----------------------------------------------------------------------
void UART_init(void);
void USART_Transmit(unsigned char data);
//----------------------------------------------------------------------
#endif	/* USART_H */
//----------------------------------------------------------------------