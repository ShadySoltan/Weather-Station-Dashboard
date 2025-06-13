/*
 * LCD.h
 *
 *  Created on: Jun 8, 2025
 *      Author: oelwy
 */

#ifndef LCD_H_
#define LCD_H_

#include "../tm4c123gh6pm.h"
#include "../std_types.h"

#define LCD_PORT_DATA       GPIO_PORTB_DATA_R

// LCD connected to Port B
#define RS              2   // PB2
#define EN              3   // PB3
#define D4              4
#define D5              5
#define D6              6
#define D7              7
#define LCD_CTRL_MASK (RS | EN | D4 | D5 | D6 | D7)

#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_ENTRY_MODE      0x06
#define LCD_DISPLAY_ON      0x0C
#define LCD_CURSOR_ON_BLINK 0x0F
#define LCD_FUNCTION_SET    0x28
#define LCD_SET_CURSOR      0x80


// --- LCD functions (GPIO - 4-bit parallel mode) ---
void LCD_EnablePulse(void);

void LCD_Send4Bits(uint8 nibble);

void LCD_Command(uint8 cmd);

void LCD_Char(uint8 data);

void LCD_Init(void);

void LCD_Clear(void);

void LCD_SetCursor(uint8 row, uint8 col);

void LCD_String(uint8 *str);

void LCD_PrintNum(uint8 num);

#endif /* LCD_H_ */
