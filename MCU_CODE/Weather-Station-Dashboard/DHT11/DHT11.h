/*
 * DHT11.h
 *
 *  Created on: Jun 8, 2025
 *      Author: oelwy
 */

#ifndef DHT11_H_
#define DHT11_H_


#include "../tm4c123gh6pm.h"
#include "../std_types.h"


#define DHT_PIN (1 << 2)  // PA2


// --- DHT Functions (unchanged) ---
void DHT_PinOutput();

void DHT_PinInput();

void DHT_Start();

uint8 DHT_CheckResponse();

uint8 DHT_ReadBit();

uint8 DHT_ReadByte();

uint8 DHT_ReadData(uint8 *humidity, uint8 *temperature);

#endif /* DHT11_H_ */
