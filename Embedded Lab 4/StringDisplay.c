/**
 * @file    StringDisplay.c
 * @brief   Functions for formatting and displaying ADC values.
 *
 * This file contains functions to format and display ADC values based on different modes.
 * It includes functions to convert ADC values to graphical representations (gages) and
 * capacitor colors, as well as functions to clamp values and convert them to percentages.
 *
 * @date    10.05.2024
 * @author  Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "StringDisplay.h"

#define CHAR_ARRAY_LENGTH 5
#define NUM_SPACES_TO_PAD 4

#define GAGE_VALUE_1111 " 1111"
#define GAGE_VALUE_1110 " 1110"
#define GAGE_VALUE_1100 " 1100"
#define GAGE_VALUE_1000 " 1000"
#define GAGE_VALUE_0000 " 0000"

#define ZERO 0
#define ONE 1
#define TWENTY_FIVE 25

#define ONE_HUNDRED 100
#define TWO_HUNDRED 200
#define THREE_HUNDRED 300
#define FOUR_HUNDRED 400
#define FIVE_HUNDRED 500
#define SIX_HUNDRED 600
#define SEVEN_HUNDRED 700
#define EIGHT_HUNDRED 800
#define NINE_HUNDRED 900
#define ONE_THOUSAND 1000

void convertToGage(uint16_t value, char *gageStr);
void convertToCapColour(uint16_t value, char *colourStr);

uint16_t clampToMax(uint16_t value, uint16_t maxValue);
uint16_t convertToPercentage(uint16_t value);

/**
 * @brief Prints ADC values based on the specified mode.
 *
 * This function formats and prints ADC values according to the specified mode.
 * It converts the ADC value to different formats based on the mode.
 *
 * @param mode The mode indicating how to format the ADC value.
 * @param value The ADC value to be printed.
 */
void printAdcValues(uint8_t mode, uint16_t value)
{
    char temp[TWENTY_FIVE];
    char tempNumericStrn[TWENTY_FIVE]; // Array to store right-aligned print value
    char tempDisplayStrn[TWENTY_FIVE]; // Array to store right-aligned display value
    uint16_t numericValue = clampToMax(value, ONE_THOUSAND);
    uint16_t percentValue = convertToPercentage(numericValue);

    switch (mode)
    {
    case MODE_A:
        // Right-align numerical print value
        rightAlignIntToCharArray(numericValue, tempNumericStrn);
        // Right-align graphical display value
        rightAlignIntToCharArray(percentValue, tempDisplayStrn);
        // Format the string
        sprintf(temp, "ADC: %s --> %s%%", tempNumericStrn, tempDisplayStrn);
        // Print the formatted string
        serialPrintln(temp);
        break;
    case MODE_B:
        // Right-align print value
        rightAlignIntToCharArray(numericValue, tempNumericStrn);
        // Format the gauge display
        convertToGage(numericValue, tempDisplayStrn);
        // Format the string
        sprintf(temp, "ADC: %s --> %s", tempNumericStrn, tempDisplayStrn);
        // Print the formatted string
        serialPrintln(temp);
        break;
    case MODE_C:
        // Right-align print value
        rightAlignIntToCharArray(numericValue, tempNumericStrn);
        // Format the capacitor color display
        convertToCapColour(numericValue, tempDisplayStrn);
        // Format the string
        sprintf(temp, "ADC: %s --> %s", tempNumericStrn, tempDisplayStrn);
        // Print the formatted string
        serialPrintln(temp);
        break;
    default:
        break;
    }
}

/**
 * @brief Converts an integer to a right-aligned character array.
 *
 * This function converts the given integer to a string and right-aligns it within
 * the provided character array, padding with spaces as needed.
 *
 * @param num The integer to convert.
 * @param charArray Pointer to the character array where the result will be stored.
 */
void rightAlignIntToCharArray(uint16_t num, char *charArray)
{
    // Convert integer to string
    char numStr[CHAR_ARRAY_LENGTH]; // Need space for the null terminator
    snprintf(numStr, sizeof(numStr), "%d", num);

    // Get the length of the integer string
    uint16_t numStrLen = strlen(numStr);

    // Calculate the number of spaces to add before the integer
    uint16_t numSpaces = NUM_SPACES_TO_PAD - numStrLen;

    // Fill the character array with spaces followed by the integer string
    uint16_t i = ZERO;
    while (i < numSpaces)
    {
        charArray[i++] = ' '; // Add spaces
    }

    uint16_t j = ZERO;
    while (j < numStrLen)
    {
        charArray[i++] = numStr[j++]; // Add integer characters
    }

    // Add null terminator
    charArray[CHAR_ARRAY_LENGTH - ONE] = '\0';
}

/**
 * @brief Converts an ADC value to a graphical representation (gage).
 *
 * This function converts the given ADC value to a graphical representation (gage)
 * based on predefined thresholds. It assigns a binary representation to the `gageStr`
 * parameter based on the value range.
 *
 * @param value The ADC value to convert.
 * @param gageStr Pointer to the character array where the gage representation will be stored.
 */
void convertToGage(uint16_t value, char *gageStr)
{
    // Check the value range and assign the corresponding binary representation
    if (value > EIGHT_HUNDRED && value <= ONE_THOUSAND)
    {
        strcpy(gageStr, GAGE_VALUE_1111); // Set gageStr to 1111
    }
    else if (value > SIX_HUNDRED && value <= EIGHT_HUNDRED)
    {
        strcpy(gageStr, GAGE_VALUE_1110); // Set gageStr to 1110
    }
    else if (value > FOUR_HUNDRED && value <= SIX_HUNDRED)
    {
        strcpy(gageStr, GAGE_VALUE_1100); // Set gageStr to 1100
    }
    else if (value > TWO_HUNDRED && value <= FOUR_HUNDRED)
    {
        strcpy(gageStr, GAGE_VALUE_1000); // Set gageStr to 1000
    }
    else if (value >= ZERO && value <= TWO_HUNDRED)
    {
        strcpy(gageStr, GAGE_VALUE_0000); // Set gageStr to 0000
    }
    else
    {
        // Handle out-of-range values
        strcpy(gageStr, ""); // Empty string
    }
}

/**
 * @brief Converts an ADC value to a cap colour.
 *
 * This function converts the given ADC value to a cap colour based on predefined thresholds.
 * It assigns a colour string to the `colourStr` parameter based on the value range.
 *
 * @param value The ADC value to convert.
 * @param colourStr Pointer to the character array where the cap colour will be stored.
 */
void convertToCapColour(uint16_t value, char *colourStr)
{
    if (value > TWO_HUNDRED && value <= ONE_THOUSAND)
    {
        strcpy(colourStr, "None"); // No cap
    }
    else if (value > ONE_HUNDRED && value <= TWO_HUNDRED)
    {
        strcpy(colourStr, "White"); // White cap
    }
    else if (value >= ZERO && value <= ONE_HUNDRED)
    {
        strcpy(colourStr, "Black"); // Black cap
    }
    else // Error
    {
        strcpy(colourStr, "ERROR");
    }
}

/**
 * @brief Clamps a value to a maximum value.
 *
 * This function ensures that the given value does not exceed the specified maximum value.
 *
 * @param value The value to clamp.
 * @param maxValue The maximum value to clamp to.
 * @return The clamped value, which is the minimum of the given value and maxValue.
 */
uint16_t clampToMax(uint16_t value, uint16_t maxValue)
{
    if (value > maxValue)
    {
        return maxValue; // If value exceeds maxValue, return maxValue
    }
    return value; // Otherwise, return value unchanged
}

/**
 * @brief Converts a value to a percentage.
 *
 * This function converts the given value to a percentage based on a maximum value of 1000.
 *
 * @param value The value to convert to a percentage.
 * @return The percentage value of the given value, scaled to a maximum of 100.
 */
uint16_t convertToPercentage(uint16_t value)
{
    return (uint16_t)(((double)value / (double)ONE_THOUSAND) * (double)ONE_HUNDRED); // Convert value to percentage
}