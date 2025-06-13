/*
 * LDR.c
 *
 *  Created on: Jun 8, 2025
 *      Author: oelwy
 */

#include "../std_types.h"
#include "../tm4c123gh6pm.h"
#include "../Delay/delay.h"


void ADC0_Init(void) {
    // 1. Enable clock to GPIO Port E and ADC0
    SYSCTL_RCGCGPIO_R |= (1 << 4);      // Enable Port E clock
    SYSCTL_RCGCADC_R |= (1 << 0);       // Enable ADC0 clock
    while ((SYSCTL_PRGPIO_R & (1 << 4)) == 0); // Wait for Port E

    // 2. Configure PE3 (AIN0) as analog input
    GPIO_PORTE_AFSEL_R |= (1 << 3);     // Enable alternate function
    GPIO_PORTE_DEN_R &= ~(1 << 3);      // Disable digital I/O
    GPIO_PORTE_AMSEL_R |= (1 << 3);     // Enable analog mode

    // 3. Configure ADC0
    ADC0_ACTSS_R &= ~(1 << 3);          // Disable SS3 during config
    ADC0_EMUX_R &= ~0xF000;             // Software trigger (default)
    ADC0_SSMUX3_R = 0;                  // Sample AIN0 (PE3)
    ADC0_SSCTL3_R |= (1 << 1) | (1 << 2); // Single sample, no interrupts
    ADC0_ACTSS_R |= (1 << 3);           // Enable SS3
}


uint16 ADC0_Read(void) {
    ADC0_PSSI_R |= (1 << 3);            // Start sampling (SS3)
    while ((ADC0_RIS_R & (1 << 3)) == 0); // Wait for conversion
    uint32 result = ADC0_SSFIFO3_R;   // Read result
    ADC0_ISC_R |= (1 << 3);             // Clear interrupt flag
    return result;
}

float ADC0_ReadVoltage(void) {
    uint32 adcValue = ADC0_Read();
    return (adcValue * 3.3) / 4095.0;   // Convert to voltage
}


uint8 LDR_ReadPercentage(void) {
    uint32 adcValue = ADC0_Read();

    // --- MODIFICATION START ---
    // Invert the ADC value to get a direct representation of light intensity.
    // Max ADC value (4095) represents darkest (lowest light for your module).
    // Min ADC value (0) represents brightest (highest light for your module).
    uint32 invertedAdcValue = 4095 - adcValue;

    // Convert the inverted value to a percentage (0-100%).
    // Now, higher invertedAdcValue (which means lower original adcValue) will result in a higher percentage.
    uint8 percentage = (invertedAdcValue * 100) / 4095;

    // Optional: Add a small safeguard if floating point math or edge cases lead to >100%
    if (percentage > 100) {
        percentage = 100;
    }
    // As uint8, it cannot be less than 0, but if it were int, you'd add:
    // if (percentage < 0) percentage = 0;

    return percentage;
    // --- MODIFICATION END ---
}

