#define times 50
#include <xc.h>
#define _XTAL_FREQ 16000000 //define crystal frequency to 20MHz
unsigned char i;
int main(void) {
    TRISD = 0x00; //set RC0 pin as a digital output pin
    PORTD = 0x00;
    TRISC0 = 1; // Set RB1 as input for the push button
    TRISC1 = 1; // Set RB1 as input for the push button
    unsigned int buttonState;
    unsigned int prevButtonState = 1; 
    
    while (1) {
             buttonState = RC0;
      if (buttonState)
      {
          
            PORTD =  0x00;
            
     

        
        }
       if(buttonState == 0)
      {
          PORTD =  0xff;
      }
            
        
        return 0;
    }
}