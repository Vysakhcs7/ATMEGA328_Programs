/* 
 * File:   VL6180.h
 * Author: Vysakh C S
 *
 * Created on 12 October, 2023, 4:19 PM
 */
//---------------------------------------------------------------------- 
#ifndef VL6180_H
#define	VL6180_H
//---------------------------------------------------------------------- 
#include "TWI.h"
//----------------------------------------------------------------------
#define SLAVE_ADDRESS_WRITE (VL6180X_ADDRESS << 1) //SLA + WRITE (0X52)
#define SLAVE_ADDRESS_READ ((SLAVE_ADDRESS_WRITE) | 0x53) //SLA + READ (0X53)

#define NEW_SLAVE_ADDRESS_WRITE (VL6180X_NEW_ADDRESS << 1) //SLA + WRITE (0X60)
#define NEW_SLAVE_ADDRESS_READ ((NEW_SLAVE_ADDRESS_WRITE) | 0x61) //SLA + READ (0X53)
//---------------------------------------------------------------------- 
// Register Address
//----------------------------------------------------------------------
#define SYSTEM__FRESH_OUT_OF_RESET (0x0016) //Fresh out of reset bit, default of 1, user can set this to 0 after initial boot and can therefore use this to check for a reset condition
#define READOUT__AVERAGING_SAMPLE_PERIOD (0x010A) //The internal readout averaging sample period can be adjusted from 0 to 255.
#define SYSRANGE__VHV_REPEAT_RATE (0x0031)
#define SYSRANGE__VHV_RECALIBRATE (0x002E)
#define SYSRANGE__INTERMEASUREMENT_PERIOD (0x001B)
#define SYSRANGE__START (0x018)
#define SYSTEM__INTERRUPT_CLEAR (0x015)
#define I2C_SLAVE__DEVICE_ADDRESS (0x212)
//----------------------------------------------------------------------
void VL6180X_write_addr16_data8(uint16_t address, uint8_t value);
uint8_t VL6180X_read_addr16_data8(uint16_t address);
void VL6180X_initial_settings(void);
void configure_default(void);
void VL6180X_Start_Range(void);
void VL6180X_Poll_Range(void);
void VL6180X_Clear_Interrupts(void);
int VL6180X_Read_Range();
void VL6180X_stop_range_measurement(void);
//---------------------------------------------------------------------- 
#endif	/* VL6180_H */
//----------------------------------------------------------------------
