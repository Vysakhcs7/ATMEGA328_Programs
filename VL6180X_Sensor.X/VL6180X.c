/*
 * File:   VL6180X.c
 * Author: Vysakh C S
 *
 * Created on 12 October, 2023, 4:21 PM
 */
//----------------------------------------------------------------------
#include "VL6180X.h"
//----------------------------------------------------------------------
uint8_t VL6180X_ADDRESS = 0x29; //I2C slave address
uint8_t VL6180X_NEW_ADDRESS = 0x30; //I2C New slave address
uint8_t read_data = 0; //Variable to store the data received from the slave register.
int sensor_range = 0;
//----------------------------------------------------------------------
//Function to write data into the VL6180X register.
//---------------------------------------------------------------------- 

void VL6180X_write_addr16_data8(uint16_t address, uint8_t value) {
    I2C_Start(); //start the I2C communication
    I2C_Write(SLAVE_ADDRESS_WRITE); //SLA + WRITE (0X52)
    //I2C_Write(((address >> 8) & 0xFF)); //Send the most significant byte of the 16-bit address 
    I2C_Write(address & 0xFF00); //Send the most significant byte of the 16-bit address 
    I2C_Write(address & 0xFF); // Send the least significant byte of the 16-bit address 
    I2C_Write(value); //send the desired value to the register 
    I2C_Stop(); //terminate the I2C communication

}
//----------------------------------------------------------------------
//Function to read data from the VL6180X registers.
//----------------------------------------------------------------------   

uint8_t VL6180X_read_addr16_data8(uint16_t address) {
    I2C_Start(); //start the I2C communication
   // I2C_Write(SLAVE_ADDRESS_WRITE); //SLA + WRITE (0X52)
    I2C_Write(SLAVE_ADDRESS_WRITE); //SLA + WRITE (0X52)
    I2C_Write(((address >> 8) & 0xFF)); //Send the most significant byte of the 16-bit address 
    I2C_Write(address & 0xFF); // Send the least significant byte of the 16-bit address 
    I2C_Start(); //Repeated start condition
    //I2C_Write(SLAVE_ADDRESS_READ);  //SLA + READ (0X53)
    I2C_Write(SLAVE_ADDRESS_READ);
    read_data = I2C_Read(); //Assigning the data read from the specified register to the variable read_data.
    I2C_Stop(); ////terminate the I2C communication
    return read_data;

}
//----------------------------------------------------------------------
/*SR03 settings
Below are the recommended settings required to be loaded onto the VL6180X during the 
initialisation of the device.(Refer - AN4545)
 */
//----------------------------------------------------------------------   

void VL6180X_initial_settings(void) {
    VL6180X_write_addr16_data8(0x0207, 0x01);
    VL6180X_write_addr16_data8(0x0208, 0x01);
    VL6180X_write_addr16_data8(0x0096, 0x00);
    VL6180X_write_addr16_data8(0x0097, 0xfd);
    VL6180X_write_addr16_data8(0x00e3, 0x00);
    VL6180X_write_addr16_data8(0x00e4, 0x04);
    VL6180X_write_addr16_data8(0x00e5, 0x02);
    VL6180X_write_addr16_data8(0x00e6, 0x01);
    VL6180X_write_addr16_data8(0x00e7, 0x03);
    VL6180X_write_addr16_data8(0x00f5, 0x02);
    VL6180X_write_addr16_data8(0x00d9, 0x05);
    VL6180X_write_addr16_data8(0x00db, 0xce);
    VL6180X_write_addr16_data8(0x00dc, 0x03);
    VL6180X_write_addr16_data8(0x00dd, 0xf8);
    VL6180X_write_addr16_data8(0x009f, 0x00);
    VL6180X_write_addr16_data8(0x00a3, 0x3c);
    VL6180X_write_addr16_data8(0x00b7, 0x00);
    VL6180X_write_addr16_data8(0x00bb, 0x3c);
    VL6180X_write_addr16_data8(0x00b2, 0x09);
    VL6180X_write_addr16_data8(0x00ca, 0x09);
    VL6180X_write_addr16_data8(0x0198, 0x01);
    VL6180X_write_addr16_data8(0x01b0, 0x17);
    VL6180X_write_addr16_data8(0x01ad, 0x00);
    VL6180X_write_addr16_data8(0x00ff, 0x05);
    VL6180X_write_addr16_data8(0x0100, 0x05);
    VL6180X_write_addr16_data8(0x0199, 0x05);
    VL6180X_write_addr16_data8(0x01a6, 0x1b);
    VL6180X_write_addr16_data8(0x01ac, 0x3e);
    VL6180X_write_addr16_data8(0x01a7, 0x1f);
    VL6180X_write_addr16_data8(0x0030, 0x00);
}
//----------------------------------------------------------------------
// Default configuration recommended by application note AN4545
//----------------------------------------------------------------------

void configure_default(void) {

    VL6180X_write_addr16_data8(READOUT__AVERAGING_SAMPLE_PERIOD, 0x30);
    VL6180X_write_addr16_data8(SYSRANGE__VHV_REPEAT_RATE, 0xFF);
    VL6180X_write_addr16_data8(SYSRANGE__VHV_RECALIBRATE, 0x01);
    VL6180X_write_addr16_data8(SYSRANGE__INTERMEASUREMENT_PERIOD, 0x09);
}
//----------------------------------------------------------------------
// Start a range measurement in single shot mode
//----------------------------------------------------------------------

void VL6180X_Start_Range(void) {
    VL6180X_write_addr16_data8(SYSRANGE__START, 0x03);
}
//----------------------------------------------------------------------
// Poll for new sample ready ready
//----------------------------------------------------------------------

void VL6180X_Poll_Range(void) {
    char status;
    char range_status;
    // check the status
    status = VL6180X_read_addr16_data8(0x04f);
    range_status = status & 0x07;
    // wait for new measurement ready status
    while (range_status != 0x04) {
        status = VL6180X_read_addr16_data8(0x04f);
        range_status = status & 0x07;
       // _delay_ms(1);
    }
}
//----------------------------------------------------------------------
// Read range result
//----------------------------------------------------------------------

int VL6180X_Read_Range() {
    sensor_range = VL6180X_read_addr16_data8(0x062);
    return sensor_range;
}
//----------------------------------------------------------------------
// Clear interrupts
//----------------------------------------------------------------------

void VL6180X_Clear_Interrupts(void) {
    VL6180X_write_addr16_data8(SYSTEM__INTERRUPT_CLEAR, 0x07);
}
//----------------------------------------------------------------------
// Stop Range measurements
//----------------------------------------------------------------------

void VL6180X_stop_range_measurement(void) {
    VL6180X_write_addr16_data8(SYSRANGE__START, 0x01);
}
//----------------------------------------------------------------------
