
#include <stdint.h>
#include "../inc/Lcd.h"

// Function to write a nibble (4 bits) to the LCD
void lcdWriteNibble(uint8_t nibble) {
    LCD_DATA_OUT = (LCD_DATA_OUT & 0x0F) | (nibble << 4); // Place nibble on data pins
    LCD_CTRL |= LCD_EN; // Enable pulse
    __delay_cycles(10); // Short delay
    LCD_CTRL &= ~LCD_EN; // Disable pulse
}

// Function to write a byte (8 bits) to the LCD in two nibbles (4 bits each)
void lcdWriteByte(uint8_t byte) {
    lcdWriteNibble(byte >> 4); // Write the upper nibble
    lcdWriteNibble(byte & 0x0F); // Write the lower nibble
}

// Function to wait until the LCD is not busy
void lcdWaitNotBusy(void) {
    LCD_DATA_DIR &= ~0xF0; // Set data pins as input
    LCD_CTRL &= ~LCD_RS; // Set RS to 0 for command mode
    LCD_CTRL |= LCD_RW; // Set RW to 1 for read mode
    LCD_CTRL |= LCD_EN; // Enable pulse

    while (LCD_DATA_IN & BUSY_FLAG); // Wait while busy flag is set

    LCD_CTRL &= ~LCD_EN; // Disable pulse
    LCD_DATA_DIR |= 0xF0; // Set data pins as output
    LCD_CTRL &= ~LCD_RW; // Set RW to 0 for write mode
}

// Function to initialize the LCD
void lcdInit(void) {
    LCD_CTRL_DIR |= LCD_RS + LCD_RW + LCD_EN; // Set control pins as outputs
    LCD_DATA_DIR |= 0xF0; // Set data pins (P3.4 - P3.7) as outputs
    
    __delay_cycles(15000); // Wait for more than 15 ms after VCC rises to 4.5V

    lcdSendCommand(CMD_FUNC_SET | N_FLAG); // Function set: 4-bit, 2-line
    lcdSendCommand(CMD_DISP_CTRL | DISP_FLAG); // Display ON, Cursor OFF, Blink OFF
    lcdClear(); // Clear display
    lcdSendCommand(CMD_EM_SET | ID_FLAG); // Entry mode set: Increment, No shift
}

// Function to send a command to the LCD
void lcdSendCommand(uint8_t cmd) {
    lcdWaitNotBusy();
    LCD_CTRL &= ~LCD_RS; // Set RS to 0 for command mode
    LCD_CTRL &= ~LCD_RW; // Set RW to 0 for write mode
    lcdWriteByte(cmd); // Write the command byte
}

// Function to send data to the LCD
void lcdSendData(uint8_t data) {
    lcdWaitNotBusy();
    LCD_CTRL |= LCD_RS; // Set RS to 1 for data mode
    LCD_CTRL &= ~LCD_RW; // Set RW to 0 for write mode
    lcdWriteByte(data); // Write the data byte
}

// Function to clear the LCD display
void lcdClear(void) {
    lcdSendCommand(CMD_DISP_CLEAR); // Send clear display command
    __delay_cycles(1600); // Wait for the command to complete
}

// Function to set the cursor position
void lcdSetCursor(uint8_t row, uint8_t col) {
    uint8_t address = col;
    if (row == 1) {
        address += 0x40; // Offset for row 1
    }
    lcdSendCommand(CMD_DDRAM_WRITE | address); // Set DDRAM address command
}
