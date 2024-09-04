/*
 * File:   main.c
 * Author: Vysakh C S
 *
 * Created on 6 October, 2023, 2:02 PM
 */
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//----------------------------------------DS1307--------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
#define F_CPU 16000000UL // 16MHz clock frequency
//#include <xc.h>
#include <avr/io.h>
#include <util/delay.h> //Library for delay function.
void I2C_Init(void); //Function for initializing the I2C.
void I2C_Start(void); //Function to start I2C communication.
void I2C_Stop(void); //Function to stop I2C communication.
void I2C_Write(unsigned char data); //Function to assign data to the data register and to transmit the data.
void DS1307_Write(void); //Function to write in to the DS1307 register.
void DS1307_Read(void); //Function to read from the registers of DS1307.
void I2C_Read(unsigned char data);
unsigned char DS1307_Address = 0x68;
//------------------------------------------------------------------------------------------------------
int main(void) {
    I2C_Init();
    while(1)
    {
        
    }
    return 0;
}
//------------------------------------------------------------------------------------------
//Function for initializing the I2C

void I2C_Init(void) {
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
//------------------------------------------------------------------------------------------
//Function to start I2C communication

void I2C_Start(void) {
    //TWCR - (Two wire control register)
    //TWSTA: TWI START Condition Bit
    //TWEN: TWI Enable Bit
    //TWINT: TWI Interrupt Flag (1 = Clear the interrupt flag to initiate operation of the TWI module.)
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
    //  if ((TWSR & 0xF8) != START && (TWSR & 0xF8) != REPEATED_START) {
    //        // Handle error
    //        // You can implement error handling here (e.g., print an error message).
    //        return;
    //    }
}
//----------------------------------------------------------------------------
//Function to assign data to the data register and to transmit the data.

void I2C_Write(unsigned char data) {
    //TWDR : TWI Data Register
    TWDR = data;
    // TWCR : TWI Control Register
    //Bit 7 : TWINT: TWI Interrupt Flag.
    //Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

//----------------------------------------------------------------------------
//Function to read data from the data register

void I2C_Read(unsigned char data) {
    //TWDR : TWI Data Register
    TWDR = data;
    // TWCR : TWI Control Register
    //Bit 7 : TWINT: TWI Interrupt Flag.
    //Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}
//----------------------------------------------------------------------------

//Function to stop I2C communication

void I2C_Stop(void) {
    // TWCR : TWI Control Register
    //Bit 4 : TWSTO: TWI STOP Condition Bit.
    //Bit 7 : TWINT: TWI Interrupt Flag.
    //Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface.
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}
//----------------------------------------------------------------------------
void DS1307_Read(void)
{
   I2C_Start();
   I2C_Write( (DS1307_Address << 1) | 0XD1 ); //D1 - 1101 0001 To read
 unsigned char read_data = I2C_Single_byte_Read();
}
//----------------------------------------------------------------------------
void DS1307_Write(void)
{
   I2C_Start();
   I2C_Write(DS1307_Address << 1); //D0 - 1101 0000 To Write    
}
//----------------------------------------------------------------------------
//Function to read single byte from the data register.
//TWDR : TWI Data Register
// TWCR : TWI Control Register
//Bit 7 : TWINT: TWI Interrupt Flag.
//Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface

unsigned char I2C_Single_byte_Read() {

    TWCR = (1 << TWINT) | (1 << TWEN); //to read more than 1 byte
    while (!(TWCR & (1 << TWINT))); // Wait until reception is complete
    return TWDR; // Return received data
}

/*---------------------------------------------------------------------------------------------------------------*/