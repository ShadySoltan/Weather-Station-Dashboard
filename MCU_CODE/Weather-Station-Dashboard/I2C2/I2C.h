/*
 * I2C.h
 *
 *  Created on: Jun 10, 2025
 *      Author: oelwy
 */

#ifndef I2C2_I2C_H_
#define I2C2_I2C_H_

#include "../std_types.h"
#include "../tm4c123gh6pm.h"

void I2C1_Init(void);
uint8 I2C1_ReadByte(uint8 slaveAddr, uint8 regAddr);
void I2C1_WriteByte(uint8 slaveAddr, uint8 regAddr, uint8 data);
void I2C1_ReadBytes(uint8 slaveAddr, uint8 regAddr, uint8 *buf, uint8 len);


#endif /* I2C2_I2C_H_ */
