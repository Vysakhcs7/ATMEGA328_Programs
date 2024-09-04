/*
 * File:   main.c
 * Author: windows 11
 *
 * Created on 27 December, 2023, 2:43 PM
 */


#include <xc.h>
#include "rtc.h"
#include "clcd.h"
#pragma config WDTE = OFF     
static void init_config(void) {
    init_clcd();
}

void i2c_init()
{
    SMP=1;
    SSPEN=1;
    SSPM3=1;
    SSPADD=49;

}
void start_bit()
{
    SEN=1;
    while(SEN);
    SSPIF=0;
}
void write( short int  *add)
{
   SSPBUF= add;
   while(!SSPIF);
   SSPIF=0;
   if(ACKSTAT)
   {
       PEN=1;
   }
}

char read()
{
    char data;
    RCEN=1;
    while(!BF);
    data=SSPBUF;
    return data;
}
void ack()
{
    ACKDT=0;
    ACKEN=1;
    while(ACKEN);
}

void nack()
{
    ACKDT=1;
    ACKEN=1;
    while(ACKEN);
}
void stop_bit()
{
    PEN=1;
    while(PEN);
    SSPIF=0;
}
void main(void) 
{
    char min;
    init_config();
    clcd_print ("SECOND",LINE1(0)); 
    while (1) 
    {
    i2c_init();
    
    start_bit();
     
    write(slave_read);
    ack();
    
    write(0x01); 
    ack();
    
    min=read();
    nack();
    
    stop_bit();
    
    //char msb = (((min >> 4 ) & 0x07 ) + 48) ;
    //char lsb =  (( min & 0x07 ) + 48) ;    
    clcd_putch( min , LINE2(2));
   // clcd_putch( lsb, LINE2(3)); 
    }
    return;
}


