
/* File:   main.c
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
unsigned char I2C_Read(unsigned char ackval);

void USART_Init(void);
void USART_Transmit(unsigned char data);
void usart_send_packedBCD(unsigned char data);

void DS1307_setTime(unsigned char h, unsigned char m, unsigned char s);
void DS1307_init(void);
void DS1307_getTime(unsigned char *h, unsigned char *m, unsigned char *s);

unsigned char DS1307_Address = 0x68;
#define BAUD_RATE 9600   // Desired baud rate
#define UBRR_VALUE ((F_CPU / (16UL * BAUD_RATE)) - 1)
//------------------------------------------------------------------------------------------------------

int main(void) {
    unsigned char i, j, k;
    DS1307_init();
    DS1307_setTime(0x23, 0x59, 0x00);
    USART_Init();
    while (1) {
        DS1307_getTime(&i, &j, &k);
        usart_send_packedBCD(i);
        USART_Transmit(':');
        usart_send_packedBCD(j);
        USART_Transmit(':');
        usart_send_packedBCD(k);
        USART_Transmit('\n');
    }
    return 0;
}
//------------------------------------------------------------------------------------------

void USART_Init(void) {
    // Set baud rate
    UBRR0H = UBRR_VALUE >> 8;
    UBRR0L = UBRR_VALUE;

    // Enable transmitter
    UCSR0B = (1 << TXEN0);

    // Set frame format: 8 data bits, no parity, 1 stop bit
    UCSR0C = (3 << UCSZ00);
}
//------------------------------------------------------------------------------------------------------

void USART_Transmit(unsigned char data) {
    // Wait for the transmit buffer to be empty
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into the buffer, which sends it
    UDR0 = data;
}
//------------------------------------------------------------------------------------------------------

void usart_send_packedBCD(unsigned char data) {
    USART_Transmit('0' + (data >> 4));
    USART_Transmit('0' + (data & 0x0F));
}
//------------------------------------------------------------------------------------------------------

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

unsigned char I2C_Read(unsigned char ackval) {
    //TWDR : TWI Data Register
    // TWCR : TWI Control Register
    //Bit 7 : TWINT: TWI Interrupt Flag.
    //Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface
    TWCR = (1 << TWINT) | (1 << TWEN) | (ackval << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}
//----------------------------------------------------------------------------

//Function to stop I2C communication

void I2C_Stop(void) {
    // TWCR : TWI Control Register
    //Bit 4 : TWSTO: TWI STOP Condition Bit.
    //Bit 7 : TWINT: TWI Interrupt Flag.
    //Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface.
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
    //for ( int k = 0 ; k < 100; k++) ; //wait for a short time
    while (TWCR & (1 << TWSTO));
}
//----------------------------------------------------------------------------

void DS1307_init(void) {
    I2C_Init();
    I2C_Start();
    I2C_Write(DS1307_Address << 1); //D0 - 1101 0000 To Write    
    I2C_Write(0X07);
    I2C_Write(0X00);
    I2C_Stop();

}
//----------------------------------------------------------------------------

void DS1307_setTime(unsigned char h, unsigned char m, unsigned char s) {
    I2C_Start();
    I2C_Write(DS1307_Address << 1); //D0 - 1101 0000 To Write
    I2C_Write(0);
    I2C_Write(s);
    I2C_Write(m);
    I2C_Write(h);
    I2C_Stop();
}
//----------------------------------------------------------------------------

void DS1307_getTime(unsigned char *h, unsigned char *m, unsigned char *s) {
    I2C_Start();
    I2C_Write(DS1307_Address << 1); //D0 - 1101 0000 To Write
    I2C_Write(0);
    I2C_Stop();

    I2C_Start();
    I2C_Write(0xD1);
    *s = I2C_Read(1);
    *m = I2C_Read(1);
    *h = I2C_Read(0);
    I2C_Stop();
}


//----------------------------------------------------------------------------


