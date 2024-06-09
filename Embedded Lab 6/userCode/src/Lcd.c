#include <stdint.h>
#include <msp430.h>

#include "../inc/Lcd.h"

#define LCD_NUM_COLS	16

#define LCD_CTRL_DIR    P2DIR
#define LCD_CTRL        P2OUT

#define LCD_DATA_DIR    P3DIR
#define LCD_DATA_OUT    P3OUT
#define LCD_DATA_IN     P3IN

#define LCD_RS          BIT0
#define LCD_RW          BIT1
#define LCD_EN          BIT2

#define CMD_DISP_CLEAR  0x01
#define CMD_EM_SET      0x04
#define CMD_DISP_CTRL   0x08
#define CMD_FUNC_SET    0x20
#define CMD_DDRAM_WRITE 0x80

#define ID_FLAG         BIT1

#define F_FLAG          BIT2
#define N_FLAG          BIT3
#define DL_FLAG         BIT4

#define BLINK_FLAG      BIT0
#define CURSOR_FLAG     BIT1
#define DISP_FLAG       BIT2

#define BUSY_FLAG       BIT7

#define DATA_MASK       0x0f
#define DATA_OFFSET     4

static uint8_t displayCtrl;

/* Read 8-bit from the LCD. */
static uint8_t read8Bit(char rs)
{
    uint8_t tmp;

    LCD_DATA_DIR &= ~(DATA_MASK << DATA_OFFSET);
    LCD_CTRL |= LCD_RW;

    if (rs)
        LCD_CTRL |= LCD_RS;
    else
        LCD_CTRL &= ~LCD_RS;

    LCD_CTRL |= LCD_EN;
    /* Wait for 2 µs to satisfy t_cycE and t_DDR. */
    __delay_cycles(32);
    tmp = (LCD_DATA_IN & (DATA_MASK << DATA_OFFSET)) >> DATA_OFFSET;
    LCD_CTRL &= ~LCD_EN;

    LCD_CTRL |= LCD_EN;
    /* Wait for 2 µs to satisfy t_cycE and t_DDR. */
    __delay_cycles(32);
    tmp = (tmp << DATA_OFFSET) | (LCD_DATA_IN >> DATA_OFFSET);
    LCD_CTRL &= ~LCD_EN;

    return tmp;
}

/* Write 4-bit to the LCD. */
static void write4Bit(char rs, uint8_t data)
{
    LCD_DATA_DIR |= (DATA_MASK << DATA_OFFSET);
    LCD_CTRL &= ~LCD_RW;

    if (rs)
        LCD_CTRL |= LCD_RS;
    else
        LCD_CTRL &= ~LCD_RS;

    LCD_CTRL |= LCD_EN;
    /* Wait for 2 µs to satisfy t_cycE. */
    __delay_cycles(32);
    LCD_DATA_OUT = (data << DATA_OFFSET);
    LCD_CTRL &= ~LCD_EN;
}

/* Write 8-bit to the LCD. */
static void write8Bit(char rs, char rw, uint8_t data)
{
    write4Bit(rs, data >> DATA_OFFSET);
    write4Bit(rs, data & DATA_MASK);
}

/*
 * Send a command to the LCD.
 *
 * This function blocks until the LCD is ready.
 */
static void sendCmd(char rs, uint8_t data)
{
    /* Wait until the LCD is ready. */
    while (read8Bit(0) & BUSY_FLAG);

    write8Bit(rs, 0, data);
}

void lcdInit(void)
{
    LCD_CTRL_DIR = LCD_RS | LCD_RW | LCD_EN;
    LCD_CTRL &= ~(LCD_RS | LCD_RW | LCD_EN);

    /*
     * The initialization sequence stated in LCD_MODUL_16X2.pdf and especially
     * in http://lcd-linux.sourceforge.net/pdfdocs/hd44780.pdf is used.
     */

    /* Wait for 50 ms to ensure the LCD is powered up. */
    __delay_cycles(800000);

    write4Bit(0, 0x3);
    __delay_cycles(67200);

    write4Bit(0, 0x3);
    __delay_cycles(2400);

    write4Bit(0, 0x3);
    __delay_cycles(2400);

    write4Bit(0, 0x2);
    __delay_cycles(2400);

    /* Set interface and LCD parameter (4-bit, 5x8 dots, 2 lines). */
    write8Bit(0, 0, CMD_FUNC_SET | N_FLAG);

    /* Enable display. */
    displayCtrl = DISP_FLAG;
    sendCmd(0, CMD_DISP_CTRL | displayCtrl);

    /* Clear display. */
    sendCmd(0, CMD_DISP_CLEAR);

    /* Set cursor move direction (increment). */
    sendCmd(0, CMD_EM_SET | ID_FLAG);
}

void lcdEnable(unsigned char on)
{
    if (on)
        displayCtrl |= DISP_FLAG;
    else
        displayCtrl &= ~DISP_FLAG;

    sendCmd(0, CMD_DISP_CTRL | displayCtrl);
}

void lcdCursorSet(unsigned char x, unsigned char y)
{
    if (x >= LCD_NUM_COLS)
        return;

    if (!y)
        sendCmd(0, CMD_DDRAM_WRITE | x);
    else if (y == 1)
        sendCmd(0, CMD_DDRAM_WRITE | 0x40 | x);
}

void lcdCursorShow(unsigned char on)
{
    if (on)
        displayCtrl |= CURSOR_FLAG;
    else
        displayCtrl &= ~CURSOR_FLAG;

    sendCmd(0, CMD_DISP_CTRL | displayCtrl);
}

void lcdCursorBlink(unsigned char on)
{
    if (on)
        displayCtrl |= BLINK_FLAG;
    else
        displayCtrl &= ~BLINK_FLAG;

    sendCmd(0, CMD_DISP_CTRL | displayCtrl);
}

void lcdClear(void)
{
    sendCmd(0, CMD_DISP_CLEAR);
}

void lcdPutChar(char character)
{
    sendCmd(1, character);
}

void lcdPutText(const char *text)
{
    unsigned char i = 0;

    while (*text && i < LCD_NUM_COLS) {
        lcdPutChar(*text++);
        i++;
    }
}

void lcdPutNumber(int number)
{
    unsigned char i;
    unsigned char tmp[5];

    if (number < 0) {
        lcdPutChar('-');
        number = -number;
    }

    i = 0;

    /*
     * Extract decimal places in reverse order and convert them to
     * characters.
     */
    do {
        tmp[i] = '0' + (number % 10);
        number = number / 10;
        i++;
    } while (number > 0);

    /* Put converted characters in reverse order to restore original order. */
    while (i > 0) {
        i--;
        lcdPutChar(tmp[i]);
    }
}