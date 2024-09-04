/* 
 * File:   I2C.c
 * Author: Vysakh C S
 *
 * Created on 5 September, 2023, 5:11 PM
 */
#include <avr/io.h>
void i2c_init(void);
void i2c_start(void);
void i2c_write(unsigned char);
void i2c_stop(void);

int main(void) {
    i2c_init(); //initialize I2C module
    i2c_start(); //transmit start condition
    i2c_write(0b11010000); //Address of DS1307 for write
    i2c_write(0x07); //set register pointer to 7
    i2c_write(0x00); //set value of location 7 to 0 
    i2c_stop(); //transmit stop condition
    
    for (int k = 0; k < 100; k++);
    
    i2c_start(); //transmit start condition
    i2c_write(0b11010000); //Address of DS1307 for write
    i2c_write(0x07); //set register pointer to 7 or 0
    i2c_write(0x55); //set seconds to 0x55 = 55 BCD
    i2c_write(0x58); //set minutes to 0x58 = 58 BCD
    i2c_write(0b00010110); //set hour = 16 in 24 hour mode
    i2c_stop();  //transmit stop condition
    while (1);
    return 0;
}

void i2c_init(void) {
    TWSR = 0X00;
    TWCR = (1 << TWEN); //TWI Enable Bit
    TWBR = 0X47; //Prescalar selection 
}

void i2c_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); //TWI Interrupt Flag : TWI START Condition Bit : TWI Enable Bit
    while (!(TWCR & (1 << TWINT))); //Polling TWINT flag to see whether start condition transmitted successfully
}

void i2c_write(unsigned char data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN); //TWI Interrupt Flag : TWI Enable Bit
    while (!(TWCR & (1 << TWINT))); //Polling TWINT flag to see whether address transmitted successfully
}

void i2c_stop() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}