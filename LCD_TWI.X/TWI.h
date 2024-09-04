/* 
 * File:   TWI.h
 * Author: Vysakh C S
 *
 * Created on 29 September, 2023, 4:06 PM
 */
//------------------------------------------------------------------------
//Header Files

#ifndef I2C_H
#define I2C_H
#define F_CPU 16000000UL // 16MHz clock frequency
#include <avr/io.h> 
#include <util/delay.h> //Library for delay function

//------------------------------------------------------------------------
//Function Declarations

void I2C_Init(); //Function for initializing the I2C
void I2C_Start(); //Function to start I2C communication
void I2C_Stop(); //Function to stop I2C communication
void I2C_Write(unsigned char data); //Function to assign data to the data register and to transmit the data.

//------------------------------------------------------------------------
#endif 
//-------------------------------------------------------------------------