/**
 * @file    Lcd.c
 * @brief   Library for interfacing with a 16x2 character LCD.
 *
 * This file contains functions for initializing and controlling a 16x2 character LCD
 * using the MSP430G2553 microcontroller.
 *
 * @date    25.05.2024
 * @version 1.0
 */



#include <msp430g2553.h>
#include <stdint.h>
#include "../inc/Lcd.h"

#define LCD_NUM_COLS      16

#define LCD_CTRL_DIR      P2DIR
#define LCD_CTRL          P2OUT

#define LCD_DATA_DIR      P3DIR
#define LCD_DATA_OUT      P3OUT
#define LCD_DATA_IN       P3IN

#define LCD_RS_BIT        BIT0
#define LCD_RW_BIT        BIT1
#define LCD_EN_BIT        BIT2

#define LCD_CMD_CLEAR_DISPLAY  0x01
#define LCD_CMD_ENTRY_MODE_SET 0x04
#define LCD_CMD_DISPLAY_CONTROL 0x08
#define LCD_CMD_FUNCTION_SET  0x20
#define LCD_CMD_DDRAM_WRITE   0x80

#define LCD_ENTRY_MODE_INCREMENT BIT1

#define LCD_FUNCTION_SET_F       BIT2
#define LCD_FUNCTION_SET_N       BIT3
#define LCD_FUNCTION_SET_DL      BIT4

#define LCD_DISPLAY_BLINK_ON     BIT0
#define LCD_DISPLAY_CURSOR_ON    BIT1
#define LCD_DISPLAY_ON           BIT2

#define LCD_STATUS_BUSY          BIT7

#define LCD_DATA_MASK            0x0f
#define LCD_DATA_OFFSET          4

static uint8_t displayControl;

/**
 * @brief Read 8-bit from the LCD.
 * 
 * @param rs Register select value.
 * @return 8-bit data read from the LCD.
 */
static uint8_t read8Bit(char rs)
{
    uint8_t tmp;

    LCD_DATA_DIR &= ~(LCD_DATA_MASK << LCD_DATA_OFFSET);
    LCD_CTRL |= LCD_RW_BIT;

    if (rs)
        LCD_CTRL |= LCD_RS_BIT;
    else
        LCD_CTRL &= ~LCD_RS_BIT;

    LCD_CTRL |= LCD_EN_BIT;
    __delay_cycles(32);
    tmp = (LCD_DATA_IN & (LCD_DATA_MASK << LCD_DATA_OFFSET)) >> LCD_DATA_OFFSET;
    LCD_CTRL &= ~LCD_EN_BIT;

    LCD_CTRL |= LCD_EN_BIT;
    __delay_cycles(32);
    tmp = (tmp << LCD_DATA_OFFSET) | (LCD_DATA_IN >> LCD_DATA_OFFSET);
    LCD_CTRL &= ~LCD_EN_BIT;

    return tmp;
}

/**
 * @brief Write 4-bit to the LCD.
 * 
 * @param rs Register select value.
 * @param data 4-bit data to write to the LCD.
 */
static void write4Bit(char rs, uint8_t data)
{
    LCD_DATA_DIR |= (LCD_DATA_MASK << LCD_DATA_OFFSET);
    LCD_CTRL &= ~LCD_RW_BIT;

    if (rs)
        LCD_CTRL |= LCD_RS_BIT;
    else
        LCD_CTRL &= ~LCD_RS_BIT;

    LCD_CTRL |= LCD_EN_BIT;
    __delay_cycles(32);
    LCD_DATA_OUT = (data << LCD_DATA_OFFSET);
    LCD_CTRL &= ~LCD_EN_BIT;
}

/**
 * @brief Write 8-bit to the LCD.
 * 
 * @param rs Register select value.
 * @param rw Read/Write value.
 * @param data 8-bit data to write to the LCD.
 */
static void write8Bit(char rs, char rw, uint8_t data)
{
    write4Bit(rs, data >> LCD_DATA_OFFSET);
    write4Bit(rs, data & LCD_DATA_MASK);
}

/**
 * @brief Send a command to the LCD. This function blocks until the LCD is ready.
 * 
 * @param rs Register select value.
 * @param data Command data to send to the LCD.
 */
static void sendCmd(char rs, uint8_t data)
{
    while (read8Bit(0) & LCD_STATUS_BUSY);

    write8Bit(rs, 0, data);
}

/**
 * @brief Initialize the LCD.
 */
void lcdInit(void)
{
    LCD_CTRL_DIR = LCD_RS_BIT | LCD_RW_BIT | LCD_EN_BIT;
    LCD_CTRL &= ~(LCD_RS_BIT | LCD_RW_BIT | LCD_EN_BIT);

    __delay_cycles(800000);

    write4Bit(0, 0x3);
    __delay_cycles(67200);

    write4Bit(0, 0x3);
    __delay_cycles(2400);

    write4Bit(0, 0x3);
    __delay_cycles(2400);

    write4Bit(0, 0x2);
    __delay_cycles(2400);

    write8Bit(0, 0, LCD_CMD_FUNCTION_SET | LCD_FUNCTION_SET_N);

    displayControl = LCD_DISPLAY_ON;
    sendCmd(0, LCD_CMD_DISPLAY_CONTROL | displayControl);

    sendCmd(0, LCD_CMD_CLEAR_DISPLAY);

    sendCmd(0, LCD_CMD_ENTRY_MODE_SET | LCD_ENTRY_MODE_INCREMENT);
}

/**
 * @brief Enable or disable the LCD display.
 * 
 * @param on 1 to enable the display, 0 to disable.
 */
void lcdEnable(unsigned char on)
{
    if (on)
        displayControl |= LCD_DISPLAY_ON;
    else
        displayControl &= ~LCD_DISPLAY_ON;

    sendCmd(0, LCD_CMD_DISPLAY_CONTROL | displayControl);
}

/**
 * @brief Set the cursor position on the LCD.
 * 
 * @param x The x-coordinate (column) of the cursor position.
 * @param y The y-coordinate (row) of the cursor position.
 */
void lcdCursorSet(unsigned char x, unsigned char y)
{
    if (x >= LCD_NUM_COLS)
        return;

    if (!y)
        sendCmd(0, LCD_CMD_DDRAM_WRITE | x);
    else if (y == 1)
        sendCmd(0, LCD_CMD_DDRAM_WRITE | 0x40 | x);
}

/**
 * @brief Show or hide the cursor on the LCD.
 * 
 * @param on 1 to show the cursor, 0 to hide.
 */
void lcdCursorShow(unsigned char on)
{
    if (on)
        displayControl |= LCD_DISPLAY_CURSOR_ON;
    else
        displayControl &= ~LCD_DISPLAY_CURSOR_ON;

    sendCmd(0, LCD_CMD_DISPLAY_CONTROL | displayControl);
}

/**
 * @brief Enable or disable cursor blinking on the LCD.
 * 
 * @param on 1 to enable blinking, 0 to disable.
 */
void lcdCursorBlink(unsigned char on)
{
    if (on)
        displayControl |= LCD_DISPLAY_BLINK_ON;
    else
        displayControl &= ~LCD_DISPLAY_BLINK_ON;

    sendCmd(0, LCD_CMD_DISPLAY_CONTROL | displayControl);
}

/**
 * @brief Clear the LCD display.
 */
void lcdClear(void)
{
    sendCmd(0, LCD_CMD_CLEAR_DISPLAY);
}

/**
 * @brief Display a single character on the LCD.
 * 
 * @param character The character to display.
 */
void lcdPutChar(char character)
{
    sendCmd(1, character);
}

/**
 * @brief Display a string of text on the LCD.
 * 
 * @param text The null-terminated string to display.
 */
void lcdPutText(const char *text)
{
    unsigned char i;

    i = 0;

    while (*text && i < LCD_NUM_COLS) {
        lcdPutChar(*text++);
        i++;
    }
}

/**
 * @brief Display a number on the LCD.
 * 
 * @param number The number to display.
 */
void lcdPutNumber(int number)
{
    unsigned char i;
    unsigned char tmp[5];

    if (number < 0) {
        lcdPutChar('-');
        number = -number;
    }

    i = 0;

    do {
        tmp[i] = '0' + (number % 10);
        number = number / 10;
        i++;
    } while (number > 0);

    while (i > 0) {
        i--;
        lcdPutChar(tmp[i]);
    }
}

void lcdSendString(uint8_t row, uint8_t col, const char *string) {
    lcdCursorSet(col, row); // Set the cursor position
    
    lcdPutText(string);
}