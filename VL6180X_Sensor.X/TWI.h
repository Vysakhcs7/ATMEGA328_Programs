/* 
 * File:   TWI.h
 * Author: Vysakh C S
 *
 * Created on 12 October, 2023, 4:05 PM
 */
//------------------------------------------------------------------------------------------
#ifndef TWI_H
#define	TWI_H
//------------------------------------------------------------------------------------------
#define F_CPU 16000000UL // 16MHz clock frequency
#define SCL_DESIRED_FREQ 400000UL //Desired Clcok Frequency
#include <avr/io.h>
#include<stdint.h> 
#include <util/delay.h> //Library for delay function
#include<stdio.h>
//------------------------------------------------------------------------------------------
void I2C_Init(void); //Function for initializing the I2C
void I2C_Start(void); //Function to start I2C communication
void I2C_Stop(void); //Function to stop I2C communication
void I2C_Write(unsigned char data); //Function to assign data to the data register and to transmit the data.
uint8_t I2C_Read(); //Function to read data from the data register.
//------------------------------------------------------------------------------------------
#endif	/* TWI_H */
//------------------------------------------------------------------------------------------
