/* 
 * File:   SSD1306.c
 * Author: Vysakh C S
 *
 * Created on 21 September, 2023, 7:14 PM
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
void I2C_Write(unsigned char data); //Function to assign data to the data register and to transmit the data.
unsigned char SSD1306_ADDRESS = 0x3D; //I2C slave address
//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------Fundamental Commands---------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------

#define SSD1306_SET_CONTROL_CONTRAST                                (0x81)    //Contrast increases as the value increases.(RESET = 7Fh)
#define ENTIRE_DISPLAY_ON                                           (0xA5)    //Entire display ON, Output ignores RAM content
#define ENTIRE_DISPLAY_ON_FROM_RAM                                  (0xA4)    //Resume to RAM content display (RESET).Output follows RAM content
#define SET_NORMAL_DISPLAY                                          (0xA6)    //Normal display (RESET)
#define SET_INVERSE_DISPLAY                                         (0xA7)    //Inverse display 
#define SET_DISPLAY_ON                                              (0xAF)    //Display ON in normal mode
#define SET_DISPLAY_OFF                                             (0xAE)    //b:Display OFF (sleep mode) (RESET)
//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------- Scrolling Commands----------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
#define RIGHT_HORIZONTAL_SCROLL                                     (0x26)  //Right Horizontal Scroll.(Horizontal scroll by 1 column)
#define LEFT_HORIZONTAL_SCROLL                                      (0x27)  //Left Horizontal Scroll.(Horizontal scroll by 1 column) 
#define VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL                        (0x29)  //Vertical and Right Horizontal Scroll.(Horizontal scroll by 1 column)
#define VERTICAL_AND_LEFT_HORIZONTAL_SCROLL                         (0x2A)  //Vertical and Left Horizontal Scroll.(Horizontal scroll by 1 column)
#define DEACTIVATE_SCROLLING                                        (0x2E)  //Stop scrolling that is configured by command 26h/27h/29h/2Ah. 
#define ACTIVATE_SCROLL                                             (0x2F)  
#define SET_VERTICAL_SCROLL_AREA                                    (0xA3) 
//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------Addressing Setting Commands--------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
#define SET_LOWER_COLOUMN_START_ADDRESS_PAGE_ADDRESSING_MODE		(0x00) 
#define SET_HIGHER_COLOUMN_START_ADDRESS_PAGE_ADDRESSING_MODE       (0x10) 
#define SET_PAGE_START_ADDRESS_PAGE_ADDRESSING_MODE                 (0xB0) 
#define SET_PAGE_ADDRESS                                            (0x22) 
#define SET_MEMORY_ADDRESSING_MODE                                  (0x20) 
#define SET_COLOUMN_ADDRESS                                         (0x21) 
//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------Hardware Configuration (Panel resolution & layout related)//-----------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
#define SET_DISPLAY_START_LINE                                      (0x40) 
#define SET_SEGMENT_REMAP_0                                         (0xA0) 
#define SET_SEGMENT_REMAP_127                                       (0xA1) 
#define MULTIPLEXER_RATIO                                           (0xA8) 
#define SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL_MODE                   (0xC0) 
#define SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED_MODE                 (0xC8) 
#define SET_DISPLAY_OFFSET                                          (0xD3) 
#define COM_PINS_HARDWARE_CONFIGURATION                             (0xDA) 

//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------Timing & Driving Scheme Setting//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------
#define __GLCD_Command_Display_Clock_Div_Ratio_Set                  (0xD5) 
#define __GLCD_Command_Precharge_Period_Set                         (0xD9) 
#define __GLCD_Command_VCOMH_Deselect_Level_Set                     (0xDB) 
#define __GLCD_Command_Nop                                          (0xE3) 
//------------------------------------------------------------------------------------------------------------------------------------------------

int main(void) {
    I2C_Init(); //Function for initializing the LCD
    return 0;
}

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
    // TWCR : TWI Control Register
    //Bit 4 : TWSTO: TWI STOP Condition Bit.
    //Bit 7 : TWINT: TWI Interrupt Flag.
    //Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface.
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}
//----------------------------------------------------------------------------
//Function to assign data to the data register and to transmit the data.

void I2C_Write(unsigned char data) {
    //TWDR ? TWI Data Register
    TWDR = data;
    // TWCR : TWI Control Register
    //Bit 7 : TWINT: TWI Interrupt Flag.
    //Bit 2 : TWEN: TWI Enable Bit.TWEN bit enables TWI operation and activates the TWI interface
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}
//----------------------------------------------------------------------------