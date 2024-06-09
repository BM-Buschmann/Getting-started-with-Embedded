#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../inc/Clock.h"
#include "../inc/Lcd.h"
#include "../inc/StringDisplay.h"


void convertToDigitString(uint8_t num, char *str)
{
    if (num < 10)
    {
        sprintf(str, "0%d", num);
    }
    else
    {
        sprintf(str, "%d", num);
    }
}

void printTimeDisplay(Time current)
{
    char timeStr[9];    // HH:MM:SS\0
    char hoursStr[3];   // HH\0
    char minutesStr[3]; // MM\0
    char secondsStr[3]; // SS\0
    const char *timeStrPtr = &timeStr[0];

    // Convert hours, minutes, and seconds to digit strings
    convertToDigitString(current.hours, hoursStr);
    convertToDigitString(current.minutes, minutesStr);
    convertToDigitString(current.seconds, secondsStr);

    sprintf(timeStr, "%s:%s:%s", hoursStr, minutesStr, secondsStr);
	lcdClear();
	lcdCursorSet(1, 8); // Set Cursor Pos. to 0,0
	lcdPutText(timeStr);
}

void rightAlignIntToCharArray(uint16_t num, char *charArray, uint16_t arrayLength, char paddingChar)
{
    // Convert integer to string
    char numStr[4];
    snprintf(numStr, sizeof(numStr), "%d", num);

    // Get the length of the integer string
    uint16_t numStrLen = strlen(numStr);

    // Calculate the number of padding characters needed
    uint16_t numPaddingChars = 3 - numStrLen;

    // Fill the character array with padding characters
    uint16_t i = 0;
    while (i < numPaddingChars)
    {
        charArray[i++] = paddingChar;
    }

    // Add the integer string to the character array
    uint16_t j = 0;
    while (j < numStrLen)
    {
        charArray[i++] = numStr[j++];
    }

    // Add null terminator
    charArray[3] = '\0';
}

void printAdcDisplay(uint16_t adcValue)
{
    char adcString[4]; // Max 4 digits

    // Right-align the ADC value string within 4 characters
    rightAlignIntToCharArray(adcValue, adcString, 4, "x");

    char valueString[8]; // Assuming a max length of 15 characters
    sprintf(valueString, "%s", adcString);

    // Print the concatenated string on the LCD
    lcdSetCursor(1, 8); // Assuming you want to print on the second row, starting from column 8
    const char *ptr = valueString;
    while (*ptr)
    {
        lcdSendData(*ptr++);
    }
}
