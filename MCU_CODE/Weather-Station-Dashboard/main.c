// Rewritten version: output DHT11 readings to 16x4 HD44780 LCD (parallel mode, 4-bit)

#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "std_types.h"
#include "UART/UART.h"
#include "Delay/delay.h"
#include "LCD/LCD.h"
#include "DHT11/DHT11.h"
#include "LDR/LDR.h"
//#include "I2C2/I2C.h"

uint8 temperature = 0;
uint8 humidity = 0;
uint8 light = 0 ;



int main(void)
{
    // 1. Enable GPIOA (LCD)
    SYSCTL_RCGCGPIO_R |= (1 << 0); // PORTA
    while (!(SYSCTL_PRGPIO_R & (1 << 0)));

    // 2. Init peripherals
    UART1_Init();
    ADC0_Init();       // PE3/AIN0 connected to LDR
    LCD_Init();


    LCD_SetCursor(0, 0);
    LCD_String((uint8 *)"HELLO TIVA!");
    delay_ms(2000);

    while (1)
    {
        // 3. Read LDR light percentage (0–100%)
        light = LDR_ReadPercentage();

        // 4. Read DHT11 values
        if (DHT_ReadData(&humidity, &temperature))
        {
            LCD_Clear();

            LCD_SetCursor(0, 0);
            LCD_String("Temp: ");
            LCD_PrintNum(temperature);
            LCD_Char('C');
            delay_ms(200);

            LCD_SetCursor(1, 0);
            LCD_String("Humi: ");
            LCD_PrintNum(humidity);
            LCD_Char('%');
            delay_ms(200);

            LCD_SetCursor(2, 0);
            LCD_String("Light: ");
            LCD_PrintNum(light);
            LCD_Char('%');
            delay_ms(200);

            // Send via UART1
            UART1_SendString("Temp=");
            UART1_PrintNum(temperature);
            UART1_SendString("C  Humi=");
            UART1_PrintNum(humidity);
            UART1_SendString("%  Light=");
            UART1_PrintNum(light);
            UART1_SendString("%\r\n");

            if (temperature == 255 || humidity == 255)
            {
                LCD_SetCursor(3, 0);
                LCD_String("Sensor error!");
            }
        }
        else
        {
            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_String("DHT11 Error");
            UART1_SendString("DHT11 Error\r\n");
        }

        delay_ms(60000);
    }
}



//int main(void)
//{
//    LCD_Init();
//    I2C1_Init();
//    BMP180_Init();
//
//    NVIC_SYSTEM_SYSHNDCTRL |= (1 << 16) | (1 << 17) | (1 << 18); // Enable fault handlers
//
//    uint8 id = I2C1_ReadByte(0x77, 0xD0); // BMP180 default I2C address, ID register
//
//    if (id == 0x55)
//    {
//        LCD_SetCursor(0, 0);
//        LCD_String("BMP180 detected\r\n");
//    }
//    else
//    {
//        LCD_SetCursor(0, 0);
//        LCD_String("BMP180 not detected, ID = ");
//        LCD_PrintNum(id);  // your custom UART print function
//    }
//
//    while (1)
//        ;
//}
