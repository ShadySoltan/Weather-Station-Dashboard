/*
 * UART.h
 *
 *  Created on: Jun 7, 2025
 *      Author: Omar Elwy
 */

#ifndef UART_H_
#define UART_H_

#include "../std_types.h"
#include "../tm4c123gh6pm.h"

#define UART_DATA_5BITS          0x0
#define UART_DATA_6BITS          0x1
#define UART_DATA_7BITS          0x2
#define UART_DATA_8BITS          0x3
#define UART_LCRH_WLEN_BITS_POS  5
#define UART_CTL_UARTEN_MASK     0x00000001
#define UART_CTL_TXE_MASK        0x00000100
#define UART_CTL_RXE_MASK        0x00000200
#define UART_FR_TXFE_MASK        0x00000080
#define UART_FR_RXFE_MASK        0x00000010
#define UART_LCRH_WLEN_8         0x00000060  // 8-bit word length
#define UART_LCRH_FEN            0x00000010  // Enable FIFOs

//// UART Flag Register (FR) bit masks
//#define UART_FR_RXFE      0x00000010  // Receive FIFO Empty
//#define UART_FR_TXFF      0x00000020  // Transmit FIFO Full
//#define UART_FR_RXFF      0x00000040  // Receive FIFO Full
//#define UART_FR_TXFE      0x00000080  // Transmit FIFO Empty
//#define UART_FR_BUSY      0x00000008  // UART Busy


void UART1_Init(void);

void UART1_SendByte(uint8 data);

uint8 UART1_ReceiveByte(void);

void UART1_SendData(const uint8 *pData, uint32 uSize);

void UART1_ReceiveData(uint8 *pData, uint32 uSize);

void UART1_SendString(const uint8 *pData);

void UART1_ReceiveString(uint8 *pData);

void UART1_PrintNum(uint8 num);

#endif /* UART_H_ */
