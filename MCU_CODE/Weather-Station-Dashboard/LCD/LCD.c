/*
 * LCD.c
 *
 *  Created on: Jun 8, 2025
 *      Author: oelwy
 */

#include "../tm4c123gh6pm.h"
#include "../std_types.h"
#include "../Delay/delay.h"
#include "LCD.h"



// --- LCD functions (GPIO - 4-bit parallel mode) ---
void LCD_EnablePulse(void)
{
    LCD_PORT_DATA |= (1 << EN);
    delay_ms(1);
    LCD_PORT_DATA &= ~(1 << EN);
    delay_ms(1);
}

void LCD_Send4Bits(uint8 nibble)
{
    LCD_PORT_DATA &= ~((1 << D4) | (1 << D5) | (1 << D6) | (1 << D7)); // Clear

    if (nibble & 0x01)
        LCD_PORT_DATA |= (1 << D4);
    if (nibble & 0x02)
        LCD_PORT_DATA |= (1 << D5);
    if (nibble & 0x04)
        LCD_PORT_DATA |= (1 << D6);
    if (nibble & 0x08)
        LCD_PORT_DATA |= (1 << D7);
}

void LCD_Command(uint8 cmd)
{
    LCD_PORT_DATA &= ~(1 << RS); // RS = 0
    LCD_Send4Bits(cmd >> 4);
    LCD_EnablePulse();
    LCD_Send4Bits(cmd & 0x0F);
    LCD_EnablePulse();
    delay_ms(2);
}

void LCD_Char(uint8 data)
{
    LCD_PORT_DATA |= (1 << RS); // RS = 1
    LCD_Send4Bits(data >> 4);
    LCD_EnablePulse();
    LCD_Send4Bits(data & 0x0F);
    LCD_EnablePulse();
    delay_ms(2);
}

void LCD_Init(void)
{
    SYSCTL_RCGCGPIO_R |= (1 << 1); // Enable Port B clock
    volatile uint8 delay = SYSCTL_RCGCGPIO_R;

    // PB2 (RS), PB3 (EN), PB4-PB7 (D4-D7) as output
    GPIO_PORTB_DIR_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6)
            | (1 << 7);
    GPIO_PORTB_DEN_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6)
            | (1 << 7);

    delay_ms(50);  // Wait LCD power on

    LCD_Command(0x33);  // 4-bit init sequence
    LCD_Command(0x32);
    LCD_Command(LCD_FUNCTION_SET);      // 4-bit, 2-line, 5x8 font
    LCD_Command(LCD_DISPLAY_ON);        // Display ON, cursor OFF
    LCD_Command(LCD_CLEAR);             // Clear display
    LCD_Command(LCD_ENTRY_MODE);        // Entry mode set
}

void LCD_Clear(void)
{
    LCD_Command(LCD_CLEAR);
    delay_ms(2);
}

void LCD_SetCursor(uint8 row, uint8 col)
{

    uint8 addr;

    switch (row)
    {
    case 0:
        addr = 0x00 + col;
        break;  // Line 1
    case 1:
        addr = 0x40 + col;
        break;  // Line 2
    case 2:
        addr = 0x10 + col;
        break;  // Line 3
    case 3:
        addr = 0x50 + col;
        break;  // Line 4
    default:
        addr = 0x00 + col;        // Default to Line 1
    }
    LCD_Command(0x80 | addr);

//    uint8 address = (row == 0) ? 0x00 : 0x40;
//    LCD_Command(LCD_SET_CURSOR + address + col);
}

void LCD_String(uint8 *str)
{
    while (*str)
    {
        LCD_Char(*str++);
    }
}

void LCD_PrintNum(uint8 num)
{
    char buf[4];
    int i = 0;
    if (num == 0)
    {
        LCD_Char('0');
        return;
    }
    while (num > 0)
    {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    }
    while (i--)
        LCD_Char(buf[i]);
}
