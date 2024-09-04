/////* 
//// * File:   LCD_TWI.c
//// * Author: Vysakh C S
//// *Program for interfacing ATMEGA328P with PCF8574 I2C Expander and 16 x2 LCD
//// * Created on 16 September, 2023, 12:10 PM
//// */

//----------------------------------------------------------------------------
//Header File
#include"TWI.h"
//----------------------------------------------------------------------------
//Function declaration
void LCD_SendCommand(unsigned char command); //Function to send commands to LCD
void LCD_SendData(unsigned char val); //Function to send single character to LCD 
void LCD_SendString(const char *stringValue); //Function to send string to LCD 
unsigned char PCF8574_ADDRESS = 0x27; //I2C slave address
//----------------------------------------------------------------------------
//Main Function

int main(void) {
    I2C_Init(); //Function for initializing the LCD
    LCD_SendCommand(0x28); //4bit mode
    LCD_SendCommand(0x0E); //Display ON, Cursor ON
    LCD_SendCommand(0x01); //Clear Display Screen
    LCD_SendCommand(0x80); //Force cursor to begin on 1st row
    LCD_SendData('4'); //Passing a single character to the LCD
    LCD_SendCommand(0xC0); //Change position to 1st coloumn of 2nd row
    LCD_SendString("Hello"); ////Passing a string to the LCD
    return 0;
}

//----------------------------------------------------------------------------
//Function to send commands to LCD

void LCD_SendCommand(unsigned char command) {
    unsigned char temp, cmd;
    //Masking the MSB 4 bits
    temp = command & 0xF0;
    //Backlight ON(P3 =1) and Register Select (RS) = 0 for command mode.
    cmd = (temp | 0x08) & (~0x01);
    I2C_Start();
    I2C_Write(PCF8574_ADDRESS << 1);
    //EN(Enable bit, P2) = 1.
    I2C_Write(cmd | 0x04);
    _delay_ms(2);
    //EN(Enable bit, P2) = 0.
    I2C_Write(cmd & (~0x04));
    I2C_Stop();

    //Shifting the 4 LSB bit to MSB.
    temp = command << 4;
    //Backlight ON(P3 =1) and Register Select (RS) = 0 for command mode.
    cmd = (temp | 0x08) & (~0x01);
    I2C_Start();
    I2C_Write(PCF8574_ADDRESS << 1);
    //EN(Enable bit, P2) = 1.
    I2C_Write(cmd | 0x04);
    _delay_ms(2);
    //EN(Enable bit, P2) = 0.
    I2C_Write(cmd & (~0x04));
    I2C_Stop();
}
//----------------------------------------------------------------------------
//Function to send single character to LCD 

void LCD_SendData(unsigned char val) {
    unsigned char k, str;
    //Masking the MSB 4 bits
    k = val & 0xF0;
    //Backlight ON(P3 =1) and Register Select (RS) = 1 for data mode.
    str = k | 0x08 | 0x01;
    I2C_Start();
    I2C_Write(PCF8574_ADDRESS << 1);
    //EN(Enable bit, P2) = 1.
    I2C_Write(str | 0x04);
    _delay_ms(2);
    //EN(Enable bit, P2) = 0.
    I2C_Write(str & (~0x04));
    I2C_Stop();

    //Shifting the 4 LSB bit to MSB.
    k = val << 4;
    //Backlight ON(P3 =1) and Register Select (RS) = 1 for data mode.
    str = k | 0x08 | 0x01;
    I2C_Start();
    I2C_Write(PCF8574_ADDRESS << 1);
    //EN(Enable bit, P2) = 1.
    I2C_Write(str | 0x04);
    _delay_ms(2);
    //EN(Enable bit, P2) = 0.
    I2C_Write(str & (~0x04));
    I2C_Stop();
}
//----------------------------------------------------------------------------
//Function to send string to LCD 

void LCD_SendString(const char *stringValue) {
    while ((*stringValue) != '\0') {
        LCD_SendData(*stringValue);
        stringValue++;
    }
}
//----------------------------------------------------------------------------