/*
 * File:   eeprom.c
 * Author: Vysakh C S
 *
 * Created on 5 October, 2023, 6:47 PM
 */
/*---------------------------------------------------------------------------------------------------------------*/
/*Steps:
 * Erase all previous data from the EEPROM
 * Read the entire EEPROM memory to check if it is erased or not.
 * Write a single byte data to a particular EEPROM memory.
 * Read and display the data from the same EEPROM memory to check if data was writeen properly.
 * Write an entire string from a given starting memroy location of EEPROM.
 * Read the string from the same starting memory location of the EEPROM to check if the string was written correctly or not.
 */
/*---------------------------------------------------------------------------------------------------------------*/

#define F_CPU 16000000UL // 16MHz clock frequency
//#include <avr/io.h>
#include <util/delay.h> //Library for delay function
#include <xc.h>
void I2C_Init(void);
void I2C_Start(void);
void I2C_Write(unsigned char data);
void I2C_Stop(void);
unsigned char I2C_Single_byte_Read(void);
void I2C_Multi_byte_Read(int n);
void at24c02_write(unsigned char mem_addr, unsigned char at24c02_data);
unsigned char eeprom_buff[60];
unsigned char AT24C02_ADDRESS = 0x50;
char w = 0x00;
char r = 0x01;
unsigned char at24c02_read(unsigned char mem_addr);
void at24c02_write_sequential(unsigned char mem_addr, unsigned char str[]);
void at24c02_read_sequential(unsigned char mem_addr, int n);
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------ATMEGA328P TWI DRIVER---------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

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
/*---------------------------------------------------------------------------------------------------------------*/
//Function to start I2C communication

void I2C_Start(void) {
    //TWCR - (Two wire control register)
    //TWSTA: TWI START Condition Bit
    //TWEN: TWI Enable Bit
    //TWINT: TWI Interrupt Flag (1 = Clear the interrupt flag to initiate operation of the TWI module.)
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
    //if ((TWSR & 0xF8) != START && (TWSR & 0xF8) != REPEATED_START) {
    // Handle error
    // You can implement error handling here (e.g., print an error message).
    //return;
    //}
}
/*---------------------------------------------------------------------------------------------------------------*/
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

/*---------------------------------------------------------------------------------------------------------------*/
//Function to stop I2C communication

void I2C_Stop(void) {
    // TWCR : TWI Control Register
    //Bit 4 : TWSTO: TWI STOP Condition Bit.
    //Bit 7 : TWINT: TWI Interrupt Flag.
    //Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface.
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}
/*---------------------------------------------------------------------------------------------------------------*/
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
//Function to read multiple bytes from the data register.
//TWDR : TWI Data Register
// TWCR : TWI Control Register
//Bit 7 : TWINT: TWI Interrupt Flag.
//Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface

void I2C_Multi_byte_Read(int n) {
    int x;
    for (x = 0; x < n; x++) {
        if (x < (n - 1)) {
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); //to read more than 1 byte
            while (!(TWCR & (1 << TWINT))); // Wait until reception is complete
            eeprom_buff[x] = TWDR;
        } else {
            TWCR = (1 << TWINT) | (1 << TWEN); //to read more than 1 byte
            while (!(TWCR & (1 << TWINT))); // Wait until reception is complete
            eeprom_buff[x] = TWDR;
        }

    }
}

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------AT24C02------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
//Function for writing the data to the EEPROM

void at24c02_write(unsigned char mem_addr, unsigned char at24c02_data) {
    I2C_Start();
    I2C_Write((AT24C02_ADDRESS << 1) + w);
    I2C_Write(mem_addr);
    I2C_Write(at24c02_data);
    I2C_Stop();
}
/*---------------------------------------------------------------------------------------------------------------*/
//Function for reading the data from the EEPROM

unsigned char at24c02_read(unsigned char mem_addr) {
    unsigned char read_data = '\0';
    I2C_Start();
    I2C_Write((AT24C02_ADDRESS << 1) + w);
    I2C_Write(mem_addr);
    I2C_Stop();
    I2C_Start();
    I2C_Write((AT24C02_ADDRESS << 1) + r);
    read_data = I2C_Single_byte_Read();
    return read_data;

    I2C_Stop();
}
/*---------------------------------------------------------------------------------------------------------------*/
//Function for reading the data from the EEPROM sequentially.

void at24c02_read_sequential(unsigned char mem_addr, int n) {
    I2C_Start();
    I2C_Write((AT24C02_ADDRESS << 1) + w);
    I2C_Write(mem_addr);
    I2C_Stop();
    I2C_Start();
    I2C_Write((AT24C02_ADDRESS << 1) + r);
    I2C_Multi_byte_Read(n);
    I2C_Stop();
    //some more to be written
}
/*---------------------------------------------------------------------------------------------------------------*/
//Function for writing the data to the EEPROM sequentially.

void at24c02_write_sequential(unsigned char mem_addr, unsigned char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        I2C_Start();
        I2C_Write((AT24C02_ADDRESS << 1) + w);
        I2C_Write(mem_addr);
        I2C_Write(str[i]);
        I2C_Stop();
        i++;
        mem_addr += 0x01;
        _delay_ms(1000);
    }
}

/*---------------------------------------------------------------------------------------------------------------*/

    int main(void) {
        I2C_Init();
        while (1) {

        }
        return 0;
    }
/*---------------------------------------------------------------------------------------------------------------*/