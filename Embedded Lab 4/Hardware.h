/**
 * @file    Hardware.h
 * @brief   Header file for Hardware.c
 *
 * This file contains declarations of functions and enums used for hardware initialization
 * and control in Hardware.c.
 *
 * @date    25.05.2024
 * @author  Bjoern Metzger & Daniel Korobow
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdint.h>

#define ADC_CHANNELS 8 /** Number of ADC channels */

// Enum for ADC channels, stored in reverse order due to hardware constraints
typedef enum
{
    CHANNEL_7,
    CHANNEL_6,
    CHANNEL_5,
    CHANNEL_4,
    CHANNEL_3,
    CHANNEL_2,
    CHANNEL_1,
    CHANNEL_0
} ADC_Channel;

// Enum for LED states
typedef enum
{
    LED_OFF, /** LED is turned off */
    LED_ON   /** LED is turned on */
} LED_STATE;

// Enum for button states
typedef enum
{
    BUTTON_NONE, /** No button is pressed */
    BUTTON_1,    /** Button 1 is pressed */
    BUTTON_2     /** Button 2 is pressed */
} Button;

/**
 * @brief Initializes the LEDs.
 *
 * This function initializes pins connected to LEDs as outputs.
 * It also ensures that all LEDs are initially turned off.
 */
void initLEDs(void);

/**
 * @brief Sets the state of the LEDs.
 *
 * This function controls the state of the LEDs connected to the pins.
 *
 * @param state The state to set the LEDs to (LED_OFF or LED_ON).
 */
void setLEDState(uint8_t state);

/**
 * @brief Initializes the Analog to Digital Converter (ADC).
 *
 * This function configures the ADC settings to enable analog input conversion.
 * It sets up the ADC channels, reference voltage, sample-and-hold time, and ADC conversion
 * sequence. Additionally, it enables the ADC and sets up the data transfer control for the ADC.
 */
void initADC(void);

/**
 * @brief Reads analog values from the ADC channels.
 *
 * This function initiates the ADC conversion process and stores the results in the provided
 * array of ADC channel values.
 *
 * @param adcChannelValues Pointer to an array where the ADC channel values will be stored.
 */
void readADC(uint16_t *adcChannelValues);

/**
 * @brief Initializes the buttons.
 *
 * This function configures pins connected to buttons as inputs and enables pull-up resistors.
 * It ensures stable input readings.
 */
void initButtons(void);

/**
 * @brief Returns the currently pressed button.
 *
 * This function checks the input states of pins connected to buttons to determine if a button is pressed.
 *
 * @return The currently pressed button (BUTTON_1, BUTTON_2, or BUTTON_NONE).
 */
uint8_t getPressedButton(void);

#endif /* HARDWARE_H_ */
