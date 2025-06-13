/*
 * UART.c
 *
 *  Created on: Mar 4, 2024
 *      Author: shady
 */
#include "UART.h"
#include "../tm4c123gh6pm.h"
#include "../std_types.h"
#include "../Delay/delay.h"

//Private Function Definitions
static void GPIO_SetupUART1Pins(void)
{
    // Enable Clock for GPIO Port B (not Port A!)
    SetBit(SYSCTL_RCGCGPIO_R, 1);  // Bit 1 for PORTB (not Bit 0 for PORTA)
    while (!(SYSCTL_PRGPIO_R & (1 << 1)));  // Wait for PORTB to be ready

    // Disable analog mode (AMSEL)
    ClearBit(GPIO_PORTB_AMSEL_R, 0);  // PB0
    ClearBit(GPIO_PORTB_AMSEL_R, 1);  // PB1

    // Configure directions:
    // PB0 (U1RX) = Input, PB1 (U1TX) = Output
    ClearBit(GPIO_PORTB_DIR_R, 0);  // PB0 = Input
    SetBit(GPIO_PORTB_DIR_R, 1);    // PB1 = Output

    // Enable alternate function (AFSEL)
    SetBit(GPIO_PORTB_AFSEL_R, 0);  // PB0 = U1RX
    SetBit(GPIO_PORTB_AFSEL_R, 1);  // PB1 = U1TX

    // Configure PCTL for UART (0x00000011 for PB0/PB1)
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFFFF00) | 0x00000011;

    // Enable digital I/O (DEN)
    SetBit(GPIO_PORTB_DEN_R, 0);  // PB0
    SetBit(GPIO_PORTB_DEN_R, 1);  // PB1
}

void UART1_Init(void) {
    // Enable clocks for UART1 and GPIOB
    SetBit(SYSCTL_RCGCUART_R, 1);  // UART1
    SetBit(SYSCTL_RCGCGPIO_R, 1);  // GPIOB
    while (!(SYSCTL_PRUART_R & (1 << 1)));  // Wait for UART1 ready

    // Disable UART during config
    UART1_CTL_R &= ~(1 << 0);  // UARTEN = 0

    // Configure baud rate (16 MHz, 115200 baud)
    UART1_IBRD_R = 8;   // Integer part
    UART1_FBRD_R = 44;  // Fractional part

    // Line control (8N1, FIFO enabled)
    UART1_LCRH_R = (0x3 << 5) | (1 << 4);  // WLEN=8 bits, FEN=1

    // Configure GPIOB for UART1 (PB0=RX, PB1=TX)
    ClearBit(GPIO_PORTB_AMSEL_R, 0);  // Disable analog
    ClearBit(GPIO_PORTB_AMSEL_R, 1);
    ClearBit(GPIO_PORTB_DIR_R, 0);    // PB0 = Input
    SetBit(GPIO_PORTB_DIR_R, 1);      // PB1 = Output
    SetBit(GPIO_PORTB_AFSEL_R, 0);    // PB0 = Alt func
    SetBit(GPIO_PORTB_AFSEL_R, 1);    // PB1 = Alt func
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFFFF00) | 0x00000011;
    SetBit(GPIO_PORTB_DEN_R, 0);      // Enable digital
    SetBit(GPIO_PORTB_DEN_R, 1);

    // Enable UART1
    UART1_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9);  // UARTEN, TXE, RXE
}

//Public Functions Definitions
//void UART1_Init(void)
//{
//    GPIO_SetupUART1Pins();
//
//    // Enable clock for UART1
//    SYSCTL_RCGCUART_R |= 0x01;
//    while (!(SYSCTL_PRUART_R & 0x01))
//        ;
//
//    //Disable UART1 while initializing
//    UART1_CTL_R = 0;
//
//    //Use system clock
//    UART1_CC_R = 0;
//
//    //Configure the baud rate of UART1 to be 9600
//    UART1_IBRD_R = 104;
//    UART1_FBRD_R = 11;
//
//    /* UART Line Control Rister Settings
//     * BRK = 0 Normal Use
//     * PEN = 0 Disable Parity
//     * EPS = 0 No affect as the parity is disabled
//     * STP2 = 0 1-stop bit at end of the frame
//     * FEN = 0 FIFOs are disabled
//     * WLEN = 0x3 8-bits data frame
//     * SPS = 0 no stick parity
//     */
//    UART1_LCRH_R = (UART_DATA_8BITS << UART_LCRH_WLEN_BITS_POS);
//
//    /* UART Control Rister Settings
//     * RXE = 1 Enable UART Receive
//     * TXE = 1 Enable UART Transmit
//     * HSE = 0 The UART is clocked using the system clock divided by 16
//     * UARTEN = 1 Enable UART
//     */
//    UART1_CTL_R =
//            UART_CTL_UARTEN_MASK | UART_CTL_TXE_MASK | UART_CTL_RXE_MASK;
//}

void UART1_SendByte(uint8 data)
{
    while (!(UART1_FR_R & UART_FR_TXFE_MASK))
        ;
    UART1_DR_R = data;
}

uint8 UART1_ReceiveByte(void)
{
    while (UART1_FR_R & UART_FR_RXFE_MASK)
        ;
    return UART1_DR_R;
}

void UART1_SendData(const uint8 *pData, uint32 uSize)
{
    while (uSize--)
    {
        UART1_SendByte(*pData);
        pData++;
    }
}

void UART1_ReceiveData(uint8 *pData, uint32 uSize)
{
    while (uSize--)
    {
        *pData = UART1_ReceiveByte();
        pData++;
    }
}

void UART1_SendString(const uint8 *pData)
{
    uint32 uCounter = 0;

    while (pData[uCounter] != '\0')
    {
        UART1_SendByte(pData[uCounter]);
        uCounter++;
    }
}

void UART1_ReceiveString(uint8 *pData)
{
    uint32 uCounter = 0;

    pData[uCounter] = UART1_ReceiveByte();

    while (pData[uCounter] != '#')
    {
        uCounter++;
        pData[uCounter] = UART1_ReceiveByte();
    }
    pData[uCounter] = '\0';
}

void UART1_PrintNum(uint8 num) {
    char buffer[4];
    int i = 0;

    if (num == 0) {
        UART1_SendByte('0');
        return;
    }

    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    // print digits in reverse
    while (i--) {
        UART1_SendByte(buffer[i]);
    }
}

