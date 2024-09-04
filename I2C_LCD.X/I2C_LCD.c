/////* 
//// * File:   I2C_LCD.c
//// * Author: Vysakh C S
//// *Program for interfacing ATMEGA328P with PCF8574 I2C Expander and 16 x2 LCD
//// * Created on 16 September, 2023, 12:10 PM
//// */

//----------------------------------------------------------------------------
#define F_CPU 16000000UL // 16MHz clock frequency
#include <avr/io.h> 
#include <util/delay.h> //Library for delay function
//----------------------------------------------------------------------------
//Function declaration
void I2C_Init(); //Function for initializing the LCD
void I2C_Start(); //Function to start I2C communication
void I2C_Stop(); //Function to stop I2C communication
void I2C_Write(unsigned char data); //Function to assign data to the data register and to transmit the data.
void LCD_SendCommand(unsigned char command); //Function to send commands to LCD
void LCD_SendData(unsigned char val); //Function to send single character to LCD 
void LCD_SendString(char *stringValue); //Function to send string to LCD 
unsigned char PCF8574_ADDRESS = 0x3F; //I2C slave address
//----------------------------------------------------------------------------
//Main Function

int main(void) {
    I2C_Init(); //Function for initializing the LCD
    LCD_SendCommand(0x28); //4bit mode
    LCD_SendCommand(0x0E); //Display ON, Cursor ON
    LCD_SendCommand(0x01); //Clear Display Screen
    LCD_SendCommand(0x80); //Force cursor to begin on 1st row
    LCD_SendData('L'); //Passing a single character to the LCD
    LCD_SendCommand(0xC0); //Change position to 1st coloumn of 2nd row
    LCD_SendString("Hello"); ////Passing a string to the LCD
    return 0;
}
//----------------------------------------------------------------------------
//Function for initializing the I2C

void I2C_Init() {
    //TWSR (Two wire Status Register)
    //TWPS1 = 0 = TWSP0 = 0.Sets the prescaler value to 1.(TWPS1 and TWSP0 are the 1st and 0th bit in TWSR)
    TWSR = 0;
    //Two Wire Control Register
    // TWEN: TWI Enable Bit.The TWEN bit enables TWI operation and activates the TWI interface.
    TWCR = (1 << TWEN);
    //TWI Bit Rate Register
    // Set bit rate register for 100 kHz
    //SCL Frequency =  CPU Clock Frequency/16+2(TWBR) X (Prescaler Value)
    TWBR = ((F_CPU / 100000) - 16) / 2;
}
//----------------------------------------------------------------------------
//Function to start I2C communication

void I2C_Start() {
    //TWCR - (Two wire control register)
    //TWSTA: TWI START Condition Bit
    //TWEN: TWI Enable Bit
    //TWINT: TWI Interrupt Flag (1 = Clear the interrupt flag to initiate operation of the TWI module.)
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}
//----------------------------------------------------------------------------
//Function to stop I2C communication

void I2C_Stop() {
    // TWCR - TWI Control Register
    //Bit 4 - TWSTO: TWI STOP Condition Bit.
    //Bit 7 - TWINT: TWI Interrupt Flag.
    //Bit 2 - TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface.
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}
//----------------------------------------------------------------------------
//Function to assign data to the data register and to transmit the data.

void I2C_Write(unsigned char data) {
    //TWDR - TWI Data Register
    TWDR = data;
    // TWCR - TWI Control Register
    //Bit 7 - TWINT: TWI Interrupt Flag.
    //Bit 2 - TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
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

void LCD_SendString(char *stringValue) {
    while ((*stringValue) != '\0') {
        LCD_SendData(*stringValue);
        stringValue++;
    }
}

//----------------------------------------------------------------------------