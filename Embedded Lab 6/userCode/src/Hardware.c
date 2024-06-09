#include <stdint.h>
#include <msp430.h>

#include "../inc/Hardware.h"

void initHardware()
{
    initButtons();

    initLed();

    initADC();
}

void initLed()
{
    P1DIR |= BIT5;  // Set Pin 5 on Port 1 as output
    P1OUT &= ~BIT5; // Initially turn off the LED
}

void toggleLed()
{
    // Toggle the state of the LED (XOR operation with the current state)
    P1OUT ^= BIT5;
}

void setLedState(LED_STATE state)
{
    // Determine the bit position based on the GPIO_PIN enum value

    if (state == LED_ON)
    {
        // Turn on the LED
        P1OUT |= BIT5;
    }
    else
    {
        // Turn off the LED
        P1OUT &= ~BIT5;
    }
}

/**
 * @brief Initializes the buttons.
 *
 * This function configures the specified pins as inputs and enables pull-up resistors for them
 * to ensure stable input readings. The buttons are connected to pins P3.0 and P3.1.
 */
void initButtons()
{

    // Configure the specified pin as an input
    P1DIR &= ~(BIT3 | BIT4);

    // Enable the pull-up resistor for the specified pin
    P1REN |= (BIT3 | BIT4);
    P1OUT |= (BIT3 | BIT4);
}

/**
 * @brief Returns the currently pressed button.
 *
 * This function checks the input states of pins P3.0 and P3.1 to determine if a button is pressed.
 * If P3.0 is low, it returns BUTTON_1, indicating that the first button is pressed.
 * If P3.1 is low, it returns BUTTON_2, indicating that the second button is pressed.
 * If neither button is pressed, it returns BUTTON_NONE.
 *
 * @return The currently pressed button (BUTTON_1, BUTTON_2, or BUTTON_NONE).
 */
BUTTON getPressedButton()
{
    if ((P1IN & BIT3) == 0)
    {
        return BUTTON_1;
    }
    else if ((P1IN & BIT4) == 0)
    {
        return BUTTON_2;
    }
    else
    {
        return BUTTON_NONE;
    }
}

// Global variable to store ADC value
volatile uint16_t ADC_Value;

/**
 * @brief Initializes the Analog to Digital Converter (ADC).
 *
 * This function configures the ADC settings to enable analog input conversion.
 * It sets up the ADC channels, reference voltage, sample-and-hold time, and ADC conversion
 * sequence. Additionally, it enables the ADC and sets up the data transfer control for the ADC.
 */
void initADC()
{
    // Configure ADC settings
    ADC10CTL0 = ADC10ON + ADC10SHT_2; // Power ADC on; use 16 clocks as sample & hold time
    ADC10AE0 |= BIT6;                 // Enable P1.6 as AD-input
    ADC10CTL1 = INCH_6;               // Select channel 6
    ADC10DTC1 = 1;                    // Enable 1 conversion
    ADC10SA = (uint16_t)&ADC_Value;   // Set DMA destination address
    ADC10CTL0 |= ENC;                 // Enable conversion
    ADC10CTL0 |= ADC10SC;             // Start conversion
}

/**
 * @brief Reads an analog value from the ADC channel.
 *
 * This function reads the analog value stored in memory by the DMA controller.
 *
 * @return The analog value read from the ADC channel.
 */
uint16_t readADC()
{
    while (ADC10CTL1 & ADC10BUSY) // Wait until result is ready
    {
    }

    return ADC_Value;
}