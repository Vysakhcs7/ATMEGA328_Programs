/*
 * File:   TWI.c
 * Author: Vysakh C S
 *
 * Created on 12 October, 2023, 4:08 PM
 */
//---------------------------------------------------------------------- 
#include "TWI.h"
uint8_t TWI_read_data = 0; //variable to store data received from the slave through the TWDR.
//---------------------------------------------------------------------- 
// Function for initializing the TWI of AMTEGA328P
//---------------------------------------------------------------------- 
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
    //TWBR = ((CPU_clock / SCL_desired ) - 16)/(2*Prescaler_Value)
    TWBR = ((F_CPU / SCL_DESIRED_FREQ) - 16) / 2;
}
//----------------------------------------------------------------------
//Function to start I2C communication
//---------------------------------------------------------------------- 

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
//----------------------------------------------------------------------
//ATMEGA328P TWI write function
//---------------------------------------------------------------------- 

void I2C_Write(unsigned char data) {
    //TWDR : TWI Data Register
    TWDR = data;
    // TWCR : TWI Control Register
    //Bit 7 : TWINT: TWI Interrupt Flag.
    //Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

//----------------------------------------------------------------------
//ATMEGA328P TWI read function
//---------------------------------------------------------------------- 
//TWDR : TWI Data Register
// TWCR : TWI Control Register
//Bit 7 : TWINT: TWI Interrupt Flag.
//Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface

uint8_t I2C_Read() {

    //TWCR = (1 << TWINT) | (1 << TWEN | (1 << TWEA)); //to read more than 1 byte
    TWCR = (1 << TWINT) | (1 << TWEN); //to read a single byte
    while (!(TWCR & (1 << TWINT))); // Wait until reception is complete
    TWI_read_data = TWDR; //Assigning the received data from the slave through the TWDR to the variable TWI_read_data.
    return TWI_read_data; // Return received data.
}

//----------------------------------------------------------------------
//Function to stop I2C communication
//---------------------------------------------------------------------- 

void I2C_Stop(void) {
    // TWCR : TWI Control Register
    //Bit 4 : TWSTO: TWI STOP Condition Bit.
    //Bit 7 : TWINT: TWI Interrupt Flag.
    //Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface.
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}
//----------------------------------------------------------------------