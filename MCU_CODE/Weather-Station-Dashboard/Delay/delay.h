/*
 * delay.h
 *
 *  Created on: Jun 7, 2025
 *      Author: Omar Elwy
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "../std_types.h"

#define SYSTEM_CLOCK_FREQ 16000000UL  // 16 MHz

// --- Delay using SysTick ---
void delay_us(uint32 us);

void delay_ms(uint32 ms);



#endif /* DELAY_H_ */
