#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../inc/Clock.h"
#include "../inc/Lcd.h"
#include "../inc/StringDisplay.h"

#define MAX_FLOAT_DIGITS 10 // Maximum number of digits for the float part

// Lookup table for powers of 10
static const float pow10[] = {1, 10, 100, 1000, 10000, 100000, 1000000};

typedef enum
{
    false,
    true,
} BOOL;

void initStringDisplay()
{
    lcdInit(); // Initialize LCD
    lcdEnable(1);
    lcdClear();
    lcdCursorBlink(0);
    lcdCursorShow(0);

    lcdSendString(0, 0, "Labor 6");
}

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

    // Convert hours, minutes, and seconds to digit strings
    convertToDigitString(current.hours, hoursStr);
    convertToDigitString(current.minutes, minutesStr);
    convertToDigitString(current.seconds, secondsStr);

    sprintf(timeStr, "%s:%s:%s", hoursStr, minutesStr, secondsStr);

    // Display "timestring" on the first row
    lcdSendString(0, 8, timeStr);
}

/**
 * @brief Returns a right-aligned string.
 *
 * @param text The null-terminated string to right-align.
 * @param width The width of the area to right-align the string within.
 * @param alignedText Buffer to store the right-aligned string. It should be large enough to hold the result.
 */
void getRightAlignedString(const char *text, unsigned char width, char *alignedText)
{
    unsigned char len = strlen(text);

    if (len >= width)
    {
        strcpy(alignedText, text);
        return;
    }

    unsigned char start = width - len;
    memset(alignedText, ' ', start);
    strcpy(alignedText + start, text);
    alignedText[width] = '\0';
}

/**
 * @brief Converts an integer to a string.
 * 
 * @param num The integer value to convert.
 * @param str The buffer to store the resulting string. It should be large enough to hold the result.
 */
void intToStr(int num, char *str)
{
    uint16_t i = 0;
    BOOL isNegative = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0) {
        isNegative = true;
        num = -num;
    }

    while (num != 0) {
        uint16_t rem = num % 10;
        str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num = num / 10;
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    uint16_t start = 0;
    uint16_t end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}


/**
 * @brief Converts a float value to a string.
 * 
 * @param value The float value to convert.
 * @param buffer The buffer to store the resulting string. It should be large enough to hold the result.
 * @param precision The number of decimal places to include in the result.
 */
void floatToString(float value, char *buffer, unsigned char precision)
{
    int intPart = (int)value;
    float floatPart = value - (float)intPart;
    char intStr[20];
    char floatStr[MAX_FLOAT_DIGITS + 1];

    intToStr(intPart, intStr);

    if (precision != 0) {
        int floatPartInt = (int)(floatPart * pow10[precision]);
        intToStr(floatPartInt, floatStr);

        sprintf(buffer, "%s.%s", intStr, floatStr);
    } else {
        sprintf(buffer, "%s", intStr);
    }
}

void printAdcDisplay(uint16_t adcValue)
{

    char alignedText[4 + 1];
    char adcString[25];

    sprintf(adcString, "%d", adcValue);
    getRightAlignedString(adcString, 4, alignedText);

    lcdSendString(1, 8, "    ");
    // Display the right-aligned ADC value on the LCD
    lcdSendString(1, 8, alignedText);
}

void printVoltageDisplay(float voltageValue)
{
    char alignedText[4 + 1];
    char floatString[20];

    // Convert float to string with 2 decimal places
    floatToString(voltageValue, floatString, 1);

    // Get the right-aligned string
    getRightAlignedString(floatString, 4, alignedText);

    lcdSendString(1, 12, "    ");
    // Display the right-aligned ADC value on the LCD
    lcdSendString(1, 12, alignedText);
}
#define OMEGA 244

// Gage values and their corresponding strings
static const uint16_t gageValues[7] = {125, 250, 375, 500, 625, 750, 875, 1023};

/**
 * @brief Converts a value to a gage string.
 * 
 * @param value The input value to convert.
 * @return char* The gage string.
 */
void  convertToGage(uint16_t value, char* gageStr)
{
    char omega = 0xF4; // Omega symbol character code

    // Find the appropriate column position based on the value
    uint8_t col = 0; // Default column position
    uint16_t i = 0;

    for (i = 0; i < 7; i++)
    {
        if (value >= gageValues[i])
        {
            col++; // Increment column position based on the index
        }
    }

    // Create the gage string
    memset(gageStr, ' ', 8); // Fill with spaces
    gageStr[col] = omega; // Place the omega symbol at the calculated column

}

void printGageDisplay(uint16_t adcValue){
    char gageString[8+1] = {};

    convertToGage(adcValue, gageString);

    // Display the right-aligned ADC value on the LCD
    lcdSendString(1, 0, gageString);

    //lcdCursorSet(1, 1);
    //lcdPutChar(OMEGA);
}
