/*
 * File:   VL6180X.c
 * Author: Vysakh C S
 *
 * Created on 5 October, 2023, 12:23 PM
 *
 */
//------------------------------------------------------------------------------------------
/*Initialization 
 
The latest Standard Ranging (SR) settings must be loaded onto VL6180X after the device 
has powered up. The following is the recommended procedure for loading the settings into 
the VL6180X.
1. Check device has powered up (Optional) 
a) Check SYSTEM__FRESH_OUT_OF_RESET {0x16} register is equal to 0x01.
2. Load settings onto VL6180X
a) See Section 9 for the settings.
3. Apply other specific settings e.g. cross talk, GPIO, max convergence time etc. 
(Optional)
4. Write 0x00 to SYSTEM__FRESH_OUT_OF_RESET {0x16} (Optional) 
a) Help host determine if settings have been loaded.
5. VL6180X is ready to start a range measurement. 
Note: This procedure must be repeated if the VL6180X has been power cycled or if GPIO-0 has 
been toggled. SYSTEM__FRESH_OUT_OF_RESET {0x16} will reset to 0x01 if the 
VL6180X has been power cycled or if GPIO-0 was toggled
------------------------------------------------------------------------------------------
 Performing a range measurement in continuous mode
 
1. Check device is ready to start range measurement. (Optional)
2. Check bit 0 of RESULT__RANGE_STATUS {0x4d} is set.
3. Start range measurements.
a) Write 0x03 to SYSRANGE__START {0x18}.
4. Wait for range measurement to complete.
a) Poll RESULT__INTERRUPT_STATUS_GPIO {0x4f} register till bit 2 is set to 1. 
(New Sample Ready threshold event).
5. Reading range result.
a) Read RESULT__RANGE_VAL {0x62}.
b) This is the range measurement between the VL6180X and target in mm.
6. Clear the Interrupt status.
a) Write 0x07 to SYSTEM__INTERRUPT_CLEAR {0x15}.
7. Repeat the steps 2 to 4 for more range measurements.
8. Stop range measurements.
a) Write 0x01 to SYSRANGE__START {0x18}
 */
//----------------------------------------------------------------------
// Header files
//----------------------------------------------------------------------
#define F_CPU 16000000UL // 16MHz clock frequency
#define SCL_DESIRED_FREQ 100000UL
#include <avr/io.h>
#include<stdint.h> 
#include <util/delay.h> //Library for delay function
#include<stdio.h>
//----------------------------------------------------------------------
// Function declaration
//----------------------------------------------------------------------
void I2C_Init(void); //Function for initializing the I2C
void I2C_Start(void); //Function to start I2C communication
void I2C_Stop(void); //Function to stop I2C communication
void I2C_Write(unsigned char data); //Function to assign data to the data register and to transmit the data.
uint8_t I2C_Read(); //Function to read data from the data register.

uint8_t VL6180X_ADDRESS = 0x29; //I2C slave address
uint8_t read_data = 0;
uint8_t status = 0;
int sensor_range = 0;
uint8_t TWI_read_data = 0;
 
void VL6180X_write_addr16_data8(uint16_t address, uint8_t value);
uint8_t VL6180X_read_addr16_data8(uint16_t address);
void VL6180X_initial_settings(void);
void configure_default(void);
void VL6180X_Start_Range(void);
void VL6180X_Poll_Range(void);
void VL6180X_Clear_Interrupts(void);
int VL6180X_Read_Range();
void VL6180X_stop_range_measurement(void);

void UART_init(void);
void UART_Send_data(const char *string);
  void USART_Transmit(unsigned char data);

//----------------------------------------------------------------------
// Register Address
//----------------------------------------------------------------------
#define REG_FRESH_OUT_OF_RESET (0x0016)
#define REG_AVERAGING_SAMPLE_PERIOD (0x010A)
#define REG_SYSALS_ANALOGUE_GAIN (0x003F)
#define REG_SYSRANGE_VHV_REPEAT_RATE (0x0031)
#define REG_SYSALS_INTEGRATION_PERIOD (0x0040)
#define REG_SYSRANGE_VHV_RECALIBRATE (0x002E)
#define REG_SYSRANGE_INTERMEASUREMENT_PERIOD (0x001B)
#define REG_SYSALS_INTERMEASUREMENT_PERIOD (0x003E)
#define REG_INTERRUPT_CONFIG_GPIO (0x014)
#define REG_MAX_CONVERGENCE_TIME (0x01C)
#define REG_INTERLEAVED_MODE_ENABLE (0x2A3)
#define REG_RANGE_START (0x018)
#define REG_RESULT_RANGE_STATUS (0x04d)
#define REG_INTERRUPT_STATUS_GPIO (0x04F)
#define REG_RANGE_VAL (0x062)
#define REG_INTERRUPT_CLEAR (0x015)
#define REG_SLAVE_DEVICE_ADDRESS (0x212)
//----------------------------------------------------------------------
// Main function
//----------------------------------------------------------------------
int main(void) {
//Device ready for measaurement
    I2C_Init(); //initialize I2C module
    UART_init();
    status = VL6180X_read_addr16_data8(REG_FRESH_OUT_OF_RESET);
    VL6180X_initial_settings();
    configure_default();
    VL6180X_write_addr16_data8(REG_FRESH_OUT_OF_RESET,0x00);
    
    //VL6180X_read_addr16_data8(REG_RESULT_RANGE_STATUS);
    
    VL6180X_Start_Range(); //Start range measurements.Write 0x03 to SYSRANGE__START (0x18)
  
    while (1) {
    VL6180X_Poll_Range(); //Wait for range measurement to complete.
    int range = VL6180X_Read_Range(); //Reading range result.
    VL6180X_Clear_Interrupts(); //Clear the Interrupt status.
    char sensorValueString[10];
    // Convert sensorValue to a string
    sprintf(sensorValueString, "%d",range);
       for (int i = 0; sensorValueString[i] != '\0'; i++) {
        USART_Transmit(sensorValueString[i]);
    }
         USART_Transmit('\n');
            USART_Transmit('\r');
    }
 
    
    return 0;
}

//----------------------------------------------------------------------
  void USART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));  // Wait for empty transmit buffer
    UDR0 = data;  // Put data into buffer and send
}

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
    TWI_read_data = TWDR;
    return TWI_read_data; // Return received data
}
//----------------------------------------------------------------------
//Function to write data into the VL6180X registers.
//---------------------------------------------------------------------- 

void VL6180X_write_addr16_data8(uint16_t address, uint8_t value) {
    I2C_Start(); //start the I2C communication
    I2C_Write(VL6180X_ADDRESS << 1); //SLA + WRITE (0X52)
    I2C_Write(((address >> 8) & 0xFF)); //Send the most significant byte of the 16-bit address 
    I2C_Write(address & 0xFF); // Send the least significant byte of the 16-bit address 
    I2C_Write(value); //send the desired value to the register 
    I2C_Stop(); //terminate the I2C communication

}
//----------------------------------------------------------------------
//Function to read data from the VL6180X registers.
//----------------------------------------------------------------------   

uint8_t VL6180X_read_addr16_data8(uint16_t address) {
    I2C_Start(); //start the I2C communication
    I2C_Write(VL6180X_ADDRESS << 1); //SLA + WRITE (0X52)
    I2C_Write(((address >> 8) & 0xFF)); //Send the most significant byte of the 16-bit address 
    I2C_Write(address & 0xFF); // Send the least significant byte of the 16-bit address 
    I2C_Start(); //Repeated start condition
    I2C_Write(0x53); //SLA + READ (0X53)
    read_data = I2C_Read(); //read the data from the specified register
    I2C_Stop(); ////terminate the I2C communication
    return read_data;

}
//----------------------------------------------------------------------
//Function to initialize UART communication
//---------------------------------------------------------------------- 
void UART_init(void) {

    UCSR0B |= (1 << TXEN0); //| (1 << RXEN0);
    UCSR0B &= (~(1 << UCSZ02));
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    UBRR0 = 103;
    
}
//----------------------------------------------------------------------
//Function to send character to the console
//----------------------------------------------------------------------
void UART_Send_data(const char *string) {

    while ((*string) != '\0') {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = *string;
        string++;
    }
}
//----------------------------------------------------------------------
/*SR03 settings
Below are the recommended settings required to be loaded onto the VL6180X during the 
initialisation of the device.(Refer - AN4545)
 */
//----------------------------------------------------------------------   
void VL6180X_initial_settings(void) 
{
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

    VL6180X_write_addr16_data8(REG_AVERAGING_SAMPLE_PERIOD, 0x30);
    VL6180X_write_addr16_data8(REG_SYSALS_ANALOGUE_GAIN, 0x46);
    VL6180X_write_addr16_data8(REG_SYSRANGE_VHV_REPEAT_RATE, 0xFF);
    VL6180X_write_addr16_data8(REG_SYSALS_INTEGRATION_PERIOD, 0x63);
    VL6180X_write_addr16_data8(REG_SYSRANGE_VHV_RECALIBRATE, 0x01);
    VL6180X_write_addr16_data8(REG_SYSRANGE_INTERMEASUREMENT_PERIOD, 0x09);
    VL6180X_write_addr16_data8(REG_SYSALS_INTERMEASUREMENT_PERIOD, 0x31);
    VL6180X_write_addr16_data8(REG_INTERRUPT_CONFIG_GPIO, 0x24);
}
//----------------------------------------------------------------------
// Start a range measurement in single shot mode
//----------------------------------------------------------------------
void VL6180X_Start_Range(void) {
    VL6180X_write_addr16_data8(REG_RANGE_START,0x03);
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
_delay_ms(1);
}
}
//----------------------------------------------------------------------
// Read range result
//----------------------------------------------------------------------
  int VL6180X_Read_Range()
    {
 sensor_range = VL6180X_read_addr16_data8(0x062);
 return sensor_range;
  }
 //----------------------------------------------------------------------
// Clear interrupts
//----------------------------------------------------------------------
void VL6180X_Clear_Interrupts(void) {
    VL6180X_write_addr16_data8(REG_INTERRUPT_CLEAR,0x07);
}
//----------------------------------------------------------------------
// Stop Range measurements
//----------------------------------------------------------------------
void VL6180X_stop_range_measurement(void) {
    VL6180X_write_addr16_data8(REG_RANGE_START,0x01);
}
//----------------------------------------------------------------------