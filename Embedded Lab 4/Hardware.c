/**
 * @file    Hardware.c
 * @brief   Functions for hardware initialization and control.
 *
 * This file contains implementations of functions for initializing and controlling hardware
 * components such as LEDs, buttons, and the Analog to Digital Converter (ADC).
 *
 * @date    10.05.2024
 * @author  Bjoern Metzger
 * @version 1.0
 */

#include <stdint.h>
#include <msp430.h>

#include "Hardware.h"

#define ZERO 0 /**< Zero value */

/**
 * @brief Initializes the buttons.
 *
 * This function configures the specified pins as inputs and enables pull-up resistors for them
 * to ensure stable input readings. The buttons are connected to pins P3.0 and P3.1.
 */
void initButtons()
{
	// Configure P3.0 and P3.1 as inputs
	P3DIR &= ~(BIT0 | BIT1);

	// Enable pull-up resistors for P3.0 and P3.1
	P3REN |= BIT0 | BIT1;
	P3OUT |= BIT0 | BIT1;
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
uint8_t getPressedButton()
{
	if ((P3IN & BIT0) == ZERO)
	{
		return BUTTON_1;
	}
	else if ((P3IN & BIT1) == ZERO)
	{
		return BUTTON_2;
	}
	else
	{
		return BUTTON_NONE;
	}
}

/**
 * @brief Initializes the LEDs.
 *
 * This function configures pins P3.2 and P3.3 as outputs to drive LEDs.
 * It also ensures that both LEDs are initially turned off.
 */
void initLEDs()
{
	// Configure P3.2 and P3.3 as outputs
	P3DIR |= BIT2 + BIT3;

	// Turn off both LEDs (P3.2 and P3.3)
	P3OUT &= ~(BIT2 + BIT3);
}

/**
 * @brief Sets the state of the LEDs.
 *
 * This function controls the state of the LEDs connected to pins P3.2 and P3.3.
 *
 * @param state The state to set the LEDs to. Use LED_ON to turn the LEDs on,
 *              and any other value to turn them off.
 */
void setLEDState(uint8_t state)
{
	if (state == LED_ON)
	{
		P3OUT |= BIT2 + BIT3; // Enable both LEDs (P3.2 and P3.3)
	}
	else
	{
		P3OUT &= ~(BIT2 + BIT3); // Disable both LEDs (P3.2 and P3.3)
	}
}

/**
 * @brief Initializes the Analog to Digital Converter (ADC).
 *
 * This function configures the ADC settings to enable analog input conversion.
 * It sets up the ADC channels, reference voltage, sample-and-hold time, and ADC conversion
 * sequence. Additionally, it enables the ADC and sets up the data transfer control for the ADC.
 */
void initADC(void)
{
	ADC10CTL1 = INCH_7 + ADC10DIV_0 + CONSEQ_3 + SHS_0; // Set input channel 7, ADC clock divider, conversion sequence, and sample-and-hold source
	ADC10CTL0 = SREF_0 + ADC10SHT_2 + MSC + ADC10ON;	// Set reference voltage, sample-and-hold time, multiple sample conversion, and turn on ADC
	ADC10AE0 = BIT7 + BIT6 + BIT5 + BIT4 + BIT3 + BIT0; // Enable analog input channels 0, 3, 4, 5, 6, and 7
	ADC10DTC1 = ADC_CHANNELS;							// Set the number of conversions to perform
}

/**
 * @brief Reads analog values from the ADC channels.
 *
 * This function initiates the ADC conversion process and stores the results in the provided
 * array of ADC channel values.
 *
 * @param adcChannelValues Pointer to an array where the ADC channel values will be stored.
 */
void readADC(uint16_t *adcChannelValues)
{
	ADC10CTL0 &= ~ENC; // Disable ADC conversion
	while (ADC10CTL1 & BUSY)
		;						// Wait until ADC is finished with the conversion
	ADC10CTL0 |= ENC + ADC10SC; // Start ADC conversion

	// Set ADC buffer register address to store conversion results
	ADC10SA = (uint16_t)adcChannelValues;
}
