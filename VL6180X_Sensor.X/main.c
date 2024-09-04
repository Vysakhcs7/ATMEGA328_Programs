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
#include "TWI.h"
#include "VL6180X.h"
#include "USART.h"
//----------------------------------------------------------------------
// Main function
//----------------------------------------------------------------------

int main(void) {
    //DDRB |= (1 << DDB4); //PB4 as Output
    //PORTB &= (1 << PORTB4); //PB4 given LOW output
    //_delay_ms(1);
    //PORTB |= (1 << PORTB4); //PB4 given HIGH output
    //_delay_ms(1);

    I2C_Init(); //initialize TWI module
    UART_init(); ////initialize UART module

    VL6180X_read_addr16_data8(SYSTEM__FRESH_OUT_OF_RESET);
    VL6180X_initial_settings();
    configure_default();
    VL6180X_write_addr16_data8(SYSTEM__FRESH_OUT_OF_RESET, 0x00);
    //VL6180X_write_addr16_data8(I2C_SLAVE__DEVICE_ADDRESS, VL6180X_NEW_ADDRESS);
    //int a = VL6180X_read_addr16_data8(I2C_SLAVE__DEVICE_ADDRESS);
    VL6180X_Start_Range(); //Start range measurements.Write 0x03 to SYSRANGE__START (0x18)

    while (1) {
        VL6180X_Poll_Range(); //Wait for range measurement to complete.
        int range = VL6180X_Read_Range(); //Reading range result.
        
                char sensorValueString[10];
         //Convert sensorValue to a string
        sprintf(sensorValueString, "%d\n\r", range);
        for (int i = 0; sensorValueString[i] != '\0'; i++) {
            USART_Transmit(sensorValueString[i]);
        }
        
        VL6180X_Clear_Interrupts(); //Clear the Interrupt status.
    }
    return 0;
}
//----------------------------------------------------------------------
