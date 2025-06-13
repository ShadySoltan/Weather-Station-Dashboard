/*
 * I2C.c
 *
 *  Created on: Jun 10, 2025
 *      Author: oelwy
 */

#include "../std_types.h"
#include "../tm4c123gh6pm.h"
#include "I2C.h"


#define SYSCTL_RCGCI2C_R       (*((volatile uint32 *)0x400FE620))
#define SYSCTL_RCGCGPIO_R      (*((volatile uint32 *)0x400FE608))
#define SYSCTL_PRGPIO_R        (*((volatile uint32 *)0x400FEA08))

#define GPIO_PORTA_AFSEL_R     (*((volatile uint32 *)0x40004420))
#define GPIO_PORTA_DEN_R       (*((volatile uint32 *)0x4000451C))
#define GPIO_PORTA_ODR_R       (*((volatile uint32 *)0x4000450C))
#define GPIO_PORTA_PCTL_R      (*((volatile uint32 *)0x4000452C))

#define I2C1_MSA_R             (*((volatile uint32 *)0x40021000))
#define I2C1_MCS_R             (*((volatile uint32 *)0x40021004))
#define I2C1_MDR_R             (*((volatile uint32 *)0x40021008))
#define I2C1_MTPR_R            (*((volatile uint32 *)0x4002100C))
#define I2C1_MCR_R             (*((volatile uint32 *)0x40021020))

#define I2C_RUN                0x01
#define I2C_START              0x02
#define I2C_STOP               0x04
#define I2C_ACK                0x08
#define I2C_BUSY               0x01
#define I2C_ERROR              0x02

#define I2C1_SCL_PIN (1 << 6)  // PA6
#define I2C1_SDA_PIN (1 << 7)  // PA7

void I2C1_Init(void) {
    SYSCTL_RCGCGPIO_R |= (1 << 0);     // Enable Port A
    SYSCTL_RCGCI2C_R |= (1 << 1);      // Enable I2C1
    while (!(SYSCTL_PRGPIO_R & (1 << 0))); // Wait for Port A ready

    // Configure PA6 (SCL) and PA7 (SDA)
    GPIO_PORTA_AFSEL_R |= (1 << 6) | (1 << 7);     // Enable alternate function
    GPIO_PORTA_DEN_R   |= (1 << 6) | (1 << 7);     // Digital enable
    GPIO_PORTA_ODR_R   |= (1 << 7);                // Open-drain SDA
    GPIO_PORTA_PCTL_R  &= ~0xFF000000;             // Clear PCTL bits
    GPIO_PORTA_PCTL_R  |=  0x33000000;             // PA6/7 I2C1

    I2C1_MCR_R = 0x10;               // Master mode
    I2C1_MTPR_R = 0x27;              // ~100 kHz (TPR = (16MHz/(2*100kHz)) - 1 = 79)
}

void I2C1_WriteByte(uint8 slaveAddr, uint8 regAddr, uint8 data) {
    I2C1_MSA_R = (slaveAddr << 1) & ~1;
    I2C1_MDR_R = regAddr;
    I2C1_MCS_R = I2C_START | I2C_RUN;
    while (I2C1_MCS_R & I2C_BUSY);
    if (I2C1_MCS_R & I2C_ERROR) return;

    I2C1_MDR_R = data;
    I2C1_MCS_R = I2C_RUN | I2C_STOP;
    while (I2C1_MCS_R & I2C_BUSY);
}

uint8 I2C1_ReadByte(uint8 slaveAddr, uint8 regAddr) {
    I2C1_MSA_R = (slaveAddr << 1) & ~1;
    I2C1_MDR_R = regAddr;
    I2C1_MCS_R = I2C_START | I2C_RUN;
    while (I2C1_MCS_R & I2C_BUSY);
    if (I2C1_MCS_R & I2C_ERROR) return 0xFF;

    I2C1_MSA_R = (slaveAddr << 1) | 1;
    I2C1_MCS_R = I2C_RUN | I2C_STOP;
    while (I2C1_MCS_R & I2C_BUSY);
    if (I2C1_MCS_R & I2C_ERROR) return 0xFF;

    return (uint8)I2C1_MDR_R;
}

void I2C1_ReadBytes(uint8 slaveAddr, uint8 regAddr, uint8 *buf, uint8 len) {
    I2C1_MSA_R = (slaveAddr << 1);
    I2C1_MDR_R = regAddr;
    I2C1_MCS_R = I2C_START | I2C_RUN;
    while (I2C1_MCS_R & I2C_BUSY);
    if (I2C1_MCS_R & I2C_ERROR) return;

    I2C1_MSA_R = (slaveAddr << 1) | 1;
    int i ;
    for ( i = 0; i < len; i++) {
        if (i == 0)
            I2C1_MCS_R = I2C_START | I2C_RUN | (len == 1 ? I2C_STOP : I2C_ACK);
        else if (i == len - 1)
            I2C1_MCS_R = I2C_RUN | I2C_STOP;
        else
            I2C1_MCS_R = I2C_RUN | I2C_ACK;

        while (I2C1_MCS_R & I2C_BUSY);
        if (I2C1_MCS_R & I2C_ERROR) break;

        buf[i] = I2C1_MDR_R;
    }
}
