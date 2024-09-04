/*
 * File:   I2C_OLED.c
 * Author: vysakh C S
 *Program to interface SSD1306 with ATMEGA328P
 * Created on 19 September, 2023, 4:49 PM
 */


//----------------------------------------------------------------------------
#define F_CPU 16000000UL // 16MHz clock frequency
#include <avr/io.h> 
#include <util/delay.h> //Library for delay function
//----------------------------------------------------------------------------
//Function declaration
void I2C_Init(); //Function for initializing the LCD
void I2C_Start(); //Function to start I2C communication
void I2C_Stop(); //Function to stop I2C communication
void I2C_Write(unsigned char data); //Function to 
void LCD_SendCommand(unsigned char command); //Function to send commands to LCD
void LCD_SendData(unsigned char val); //Function to send data to LCD 
unsigned char PCF8574_ADDRESS = 0x3F; //I2C slave address
//----------------------------------------------------------------------------
//Main Function

int main(void) {
}
//----------------------------------------------------------------------------