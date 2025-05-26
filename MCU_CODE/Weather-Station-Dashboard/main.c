

/*******************************************************/
/*************************main.c************************/
/*******************************************************/


#include "std_types.h"
#include "tm4c123gh6pm.h"


void SystemInit(void) {

    // Enable clock to all GPIO ports
    SYSCTL_RCGCGPIO_R |= 0x3F; // Ports A-F
    while((SYSCTL_PRGPIO_R & 0x3F) == 0); // Wait for clock stabilization

    // Enable clock to peripherals we'll use
    SYSCTL_RCGCUART_R |= 0x02;  // UART1
    SYSCTL_RCGCI2C_R |= 0x01;   // I2C0
    SYSCTL_RCGCADC_R |= 0x01;   // ADC0
    SYSCTL_RCGCTIMER_R |= 0x01; // Timer0
}


int main(void) {
    SystemInit();
    while(1);
}
