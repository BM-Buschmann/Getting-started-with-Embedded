#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "../inc/Scheduler.h"

#define LCD_CTRL_DIR P2DIR
#define LCD_CTRL_OUT P2OUT
#define LCD_DATA_DIR P3DIR
#define LCD_DATA_OUT P3OUT

#define LCD_RS BIT0
#define LCD_RW BIT1
#define LCD_EN BIT2

#define LCD_ROWS 2
#define LCD_COLS 16

#define CMD_CLEAR_DISPLAY 0x01
#define CMD_RETURN_HOME 0x02
#define CMD_ENTRY_MODE 0x06
#define CMD_DISPLAY_CTRL 0x0C
#define CMD_FUNCTION_SET 0x28
#define CMD_SET_DDRAM_ADDR 0x80

void lcdEnablePulse(void);
void lcdSendNibble(uint8_t nibble);
void lcdWriteByte(uint8_t byte, uint8_t mode);
void lcdBusyWait(void);

void lcdInit(void) {
    LCD_CTRL_DIR |= LCD_RS | LCD_EN;
    LCD_DATA_DIR = 0xFF;

    // Initialization sequence
    __delay_cycles(15000);  // Delay 15ms after power is applied
    lcdSendCommand(0x30);    // Function set
    __delay_cycles(5000);   // Delay 5ms
    lcdSendCommand(0x30);    // Function set
    __delay_cycles(120);     // Delay 120us
    lcdSendCommand(0x30);    // Function set
    __delay_cycles(120);     // Delay 120us

    lcdSendCommand(CMD_FUNCTION_SET);   // Set 4-bit mode
    lcdSendCommand(CMD_ENTRY_MODE);     // Set entry mode
    lcdSendCommand(CMD_CLEAR_DISPLAY);  // Clear display
    lcdSendCommand(CMD_DISPLAY_CTRL);   // Turn on display
}

void lcdSendCommand(uint8_t cmd) {
    lcdWriteByte(cmd, 0);  // Set RS low for command mode
}

void lcdSendData(uint8_t data) {
    lcdWriteByte(data, 1);  // Set RS high for data mode
}

void lcdWriteByte(uint8_t byte, uint8_t mode) {
    // Set RS and R/W according to mode
    LCD_CTRL_OUT = (mode ? LCD_RS : 0) | LCD_EN;

    // Send higher nibble first
    lcdSendNibble(byte >> 4);
    lcdEnablePulse();

    // Send lower nibble
    lcdSendNibble(byte & 0x0F);
    lcdEnablePulse();

    lcdBusyWait();
}

void lcdSendNibble(uint8_t nibble) {
    // Clear data pins
    LCD_DATA_OUT &= ~0x0F;

    // Set data pins according to the nibble
    LCD_DATA_OUT |= (nibble & 0x0F);
}

void lcdEnablePulse(void) {
    // Generate enable pulse
    LCD_CTRL_OUT |= LCD_EN;
    __delay_cycles(40);  // Pulse width > 37us
    LCD_CTRL_OUT &= ~LCD_EN;
}

void lcdSetCursor(uint8_t row, uint8_t col) {
    uint8_t address = col & 0x0F;
    if (row == 1) {
        address += 0x40;  // Address for second row
    }
    lcdSendCommand(CMD_SET_DDRAM_ADDR | address);
}

void lcdClear(void) {
    lcdSendCommand(CMD_CLEAR_DISPLAY);
    __delay_cycles(3000);  // Delay 3ms after clearing display
}

void lcdBusyWait(void) {
    // Wait until LCD is not busy
    while (LCD_DATA_IN & BIT7) {
        // Optionally yield to other tasks in the scheduler
        runScheduler();
    }
}
