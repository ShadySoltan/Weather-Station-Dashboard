/*
 * LDR.h
 *
 *  Created on: Jun 8, 2025
 *      Author: oelwy
 */

#ifndef LDR_LDR_H_
#define LDR_LDR_H_


#include "../std_types.h"
#include "../tm4c123gh6pm.h"
#include "LDR.h"

void ADC0_Init(void);

uint16 ADC0_Read(void);

float ADC0_ReadVoltage(void);

uint8_t LDR_ReadPercentage(void);



#endif /* LDR_LDR_H_ */

