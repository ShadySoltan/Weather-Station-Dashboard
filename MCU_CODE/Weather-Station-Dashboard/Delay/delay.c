/*
 * delay.c
 *
 *  Created on: Jun 7, 2025
 *      Author: Omar Elwy
 */


#include "../tm4c123gh6pm.h"
#include "../std_types.h"
#include "delay.h"

// --- Delay using SysTick ---
void delay_us(uint32 us)
{
    // Disable SysTick
    SYSTICK_CTRL_R = 0;

    // Set reload value (adjust for clock speed)
    uint32 reload = (SYSTEM_CLOCK_FREQ / 1000000UL) * us - 1;
    SYSTICK_RELOAD_R = reload;

    // Clear current value
    SYSTICK_CURRENT_R = 0;

    // Enable SysTick (no interrupts, use processor clock)
    SYSTICK_CTRL_R = (1 << 2) | (1 << 0);  // 0x05

    // Wait for COUNTFLAG
    while ((SYSTICK_CTRL_R & (1 << 16)) == 0)
        ;

    // Disable SysTick
    SYSTICK_CTRL_R = 0;
}

void delay_ms(uint32 ms)
{
    while (ms--)
        delay_us(1000);
}
