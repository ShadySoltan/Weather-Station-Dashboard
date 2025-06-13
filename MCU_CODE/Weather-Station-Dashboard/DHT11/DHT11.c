/*
 * DHT11.c
 *
 *  Created on: Jun 8, 2025
 *      Author: oelwy
 */


#include "../tm4c123gh6pm.h"
#include "../std_types.h"
#include "../Delay/delay.h"
#include "DHT11.h"

// --- DHT Functions (unchanged) ---
void DHT_PinOutput()
{
    GPIO_PORTA_DIR_R |= DHT_PIN;
    GPIO_PORTA_DEN_R |= DHT_PIN;
    GPIO_PORTA_DATA_R |= DHT_PIN;
}

void DHT_PinInput()
{
    GPIO_PORTA_DIR_R &= ~DHT_PIN;
    GPIO_PORTA_DEN_R |= DHT_PIN;
}

void DHT_Start()
{
    DHT_PinOutput();
    GPIO_PORTA_DATA_R &= ~DHT_PIN;
    delay_ms(25);
    GPIO_PORTA_DATA_R |= DHT_PIN;
    delay_us(50);
    DHT_PinInput();
}

uint8 DHT_CheckResponse()
{
    uint32 timeout = 20000;
    delay_us(50);
    while ((GPIO_PORTA_DATA_R & DHT_PIN) && timeout--)
        ;
    if (timeout == 0)
        return 0;
    timeout = 20000;
    while (!(GPIO_PORTA_DATA_R & DHT_PIN) && timeout--)
        ;
    if (timeout == 0)
        return 0;
    return 1;
}

uint8 DHT_ReadBit()
{
    uint32 timeout = 10000;

    // Wait for DHT to pull line HIGH
    while (!(GPIO_PORTA_DATA_R & DHT_PIN) && timeout--)
        ;
    if (timeout == 0)
        return 0xFF;

    delay_us(30); // Sample at ~30us (DHT spec: 26-28us for 0, 70us for 1)

    uint8 bit = (GPIO_PORTA_DATA_R & DHT_PIN) ? 1 : 0;

    timeout = 10000;
    while ((GPIO_PORTA_DATA_R & DHT_PIN) && timeout--)
        ; // Wait for LOW again
    if (timeout == 0)
        return 0xFF;

    return bit;
}

uint8 DHT_ReadByte()
{
    uint8 i, result = 0;
    for (i = 0; i < 8; i++)
    {
        uint8 bit = DHT_ReadBit();
        if (bit == 0xFF)
            return 0xFF; // Propagate failure
        result <<= 1;
        result |= bit;

        uint32 timeout = 10000;
        while ((GPIO_PORTA_DATA_R & DHT_PIN) && timeout--)
            ;
    }
    return result;
}

uint8 DHT_ReadData(uint8 *humidity, uint8 *temperature)
{
    uint8 hum_int, hum_dec, temp_int, temp_dec, checksum;
    DHT_Start();
    if (!DHT_CheckResponse())
        return 0;
    hum_int = DHT_ReadByte();
    hum_dec = DHT_ReadByte();
    temp_int = DHT_ReadByte();
    temp_dec = DHT_ReadByte();
    checksum = DHT_ReadByte();
    if ((uint8) (hum_int + hum_dec + temp_int + temp_dec) != checksum)
        return 0;
    *humidity = hum_int;
    *temperature = temp_int;
    return 1;
}
