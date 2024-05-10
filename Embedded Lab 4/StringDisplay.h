/*
 * @file    StringDisplay.h
 * @brief   Header file for StringDisplay.c
 *
 * This file contains declarations of functions and enums used for formatting and displaying
 * ADC values in StringDisplay.c.
 *
 * @date    10.05.2024
 * @author  Bjoern Metzger & Daniel Korobow
 */

#ifndef STRINGDISPLAY_H_
#define STRINGDISPLAY_H_

// Enum for printing modes
typedef enum
{
    MODE_A, // Left-aligned percentage display
    MODE_B, // Gage display
    MODE_C  // Cap type display
} PRINT_MODE;

/**
 * @brief Prints ADC values based on the specified mode.
 *
 * This function formats and prints ADC values according to the specified mode.
 * It converts the ADC value to different formats based on the mode.
 *
 * @param mode The mode indicating how to format the ADC value.
 * @param value The ADC value to be printed.
 */
void printAdcValues(uint8_t mode, uint16_t value);

#endif /* STRINGDISPLAY_H_ */
