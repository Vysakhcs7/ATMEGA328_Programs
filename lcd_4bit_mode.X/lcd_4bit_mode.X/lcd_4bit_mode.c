/*
 * File:   lcd_4bit_mode.c
 * Author: vysakh
 *
 * Created on 17 September, 2023, 3:05 PM
 */


#define F_CPU 16000000UL // 16MHz clock frequency
#include <avr/io.h>
#include<util/delay.h>

#define RS PC0
#define RW PC1
#define EN PC2
#define LCD_DATA_PORT PORTB
#define LCD_COMMAND_PORT PORTC
#define delay 100

void lcd_command(unsigned char);
void lcd_data(unsigned char);
void lcd_init(void);

int main(void) {
    DDRC = 0XFF;
    LCD_DATA_PORT = 0xFF;
    lcd_init();
    lcd_data('A');
    while (1) {

    }
    return 0;
}

void lcd_command(unsigned char cmd) {
    LCD_DATA_PORT = (LCD_DATA_PORT & 0X0F) | (cmd & 0xF0);
    LCD_COMMAND_PORT &= (~(1 << RS));
    LCD_COMMAND_PORT &= (~(1 << RW));
    LCD_COMMAND_PORT |= (1 << EN);
    _delay_us(delay);
    LCD_COMMAND_PORT &= (~(1 << EN));
    _delay_us(delay);
    
    LCD_DATA_PORT = (LCD_DATA_PORT & 0X0F) | (cmd << 4 & 0XF0);
    LCD_COMMAND_PORT |= (1 << EN);
    _delay_us(delay);
    LCD_COMMAND_PORT &= (~(1 << EN));
    _delay_us(delay);
}

void lcd_init(void) {
    lcd_command(0x02);
    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x01);
    _delay_ms(2); // Wait for clear to complete
    lcd_command(0x06);
}

void lcd_data(unsigned char text) {
    LCD_DATA_PORT = (LCD_DATA_PORT & 0X0F) | (text & 0xF0);
    LCD_COMMAND_PORT |= (1 << RS);
    LCD_COMMAND_PORT &= (~(1 << RW));
    LCD_COMMAND_PORT |= (1 << EN);
    _delay_us(delay);
    LCD_COMMAND_PORT &= (~(1 << EN));

    _delay_us(delay);

    LCD_DATA_PORT = (LCD_DATA_PORT & 0X0F) | (text << 4 & 0xF0);
    LCD_COMMAND_PORT |= (1 << EN);
    _delay_us(delay);
    LCD_COMMAND_PORT &= (~(1 << EN));
    _delay_us(delay);
}



