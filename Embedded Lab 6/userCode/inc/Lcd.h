#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/**
 * @brief Initialize the LCD.
 */
void lcdInit(void);

/**
 * @brief Enable or disable the LCD display.
 * 
 * @param on 1 to enable the display, 0 to disable.
 */
void lcdEnable(unsigned char on);

/**
 * @brief Show or hide the cursor on the LCD.
 * 
 * @param on 1 to show the cursor, 0 to hide.
 */
void lcdCursorShow(unsigned char on);

/**
 * @brief Enable or disable cursor blinking on the LCD.
 * 
 * @param on 1 to enable blinking, 0 to disable.
 */
void lcdCursorBlink(unsigned char on);

/**
 * @brief Clear the LCD display.
 */
void lcdClear(void);

/**
 * @brief Display a single character on the LCD.
 * 
 * @param character The character to display.
 */
void lcdPutChar(char character);

/**
 * @brief Display a number on the LCD.
 * 
 * @param number The number to display.
 */
void lcdPutNumber(int number);

void lcdSendString(uint8_t row, uint8_t col, const char *string);
void lcdCursorSet(unsigned char x, unsigned char y);
#endif // LCD_H