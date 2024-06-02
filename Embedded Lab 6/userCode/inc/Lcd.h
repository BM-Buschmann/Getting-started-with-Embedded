#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// Function prototypes
void lcdInit(void);
void lcdSendCommand(uint8_t cmd);
void lcdSendData(uint8_t data);
void lcdSetCursor(uint8_t row, uint8_t col);
void lcdClear(void);

#endif // LCD_H
