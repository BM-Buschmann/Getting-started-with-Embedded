/**
 * @file    main.c
 * @brief   Main entry point of the program.
 *
 * This file contains the main function, which serves as the entry point of the program.
 * It initializes the microcontroller, peripherals, and enters an infinite loop to read
 * ADC values and process button presses, printing ADC values on the console in different modes.
 *
 * Hardware Description:
 * - Connect the potentiometer (X1:U_POT) to pin CON3:P1.7.
 * - Connect the button (X1:PB5) to pin CON3:P3.0.
 * - Additionally, connect the following components:
 *   - Button (PB6) to pin CON3:P3.1.
 *   - Light Dependent Resistor (LDR) (JP1:LDR) to pin CON3:P1.6.
 *   - Red LED (JP8:LED_RT) to pin CON3:P3.2.
 *   - Green LED (JP8:LED_GN) to pin CON3:P3.3.
 *   - Place the provided black plastic sleeve on the board as sketched.
 *
 * Tasks (Part d):
 * Ensure that PB6 (button) is pressed to activate this part of the task.
 * Utilize the analog signal from JP8:LDR to detect the color of a poker chip placed on the black plastic sleeve.
 * Extend your program to evaluate the intensity sums of the red and green LEDs and output the detected chip color
 * (either "White", "Black", or "No chip") formatted with four-digit numbers, right-aligned, to the computer via the
 * serial interface. Note: The LDR requires a few milliseconds to adjust to abrupt changes in light values.
 *
 * @date    10.05.2024
 * @author  Bjoern Metzger 
 * @version 1.0
 */


#include <stdint.h>
#include <templateEMP.h>

#include "Hardware.h"
#include "StringDisplay.h"

/**
 * @brief Main function of the program.
 *
 * This function initializes the microcontroller, LEDs, ADC, and buttons. It enters an infinite loop
 * where it reads ADC values and processes button presses. Depending on the pressed button, it prints
 * ADC values on the console in different modes.
 *
 * @return This function does not return.
 */
int main(void)
{
    uint16_t adcChannelValues[ADC_CHANNELS];

    initMSP();       // Initialize microcontroller
    initLEDs();      // Initialize LEDs
    initADC();       // Initialize ADC
    initButtons();   // Initialize buttons

    while (1)
    {
        readADC(adcChannelValues); // Read ADC values

        // Process button presses
        switch (getPressedButton())
        {
            case BUTTON_2:
                setLEDState(LED_ON);
                printAdcValues(MODE_C, adcChannelValues[CHANNEL_6]);
                setLEDState(LED_OFF);
                break;
            case BUTTON_1:
                printAdcValues(MODE_B, adcChannelValues[CHANNEL_7]);
                break;
            case BUTTON_NONE:
                printAdcValues(MODE_A, adcChannelValues[CHANNEL_7]);
                break;
            default:
                break;
        }
    }
}
