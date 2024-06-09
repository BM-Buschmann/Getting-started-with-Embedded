#ifndef LCD_H
#define LCD_H

// Function prototypes
void lcdInit(void);
void lcdEnable(unsigned char on);
void lcdCursorSet(unsigned char x, unsigned char y);
void lcdCursorShow(unsigned char on);
void lcdCursorBlink(unsigned char on);
void lcdClear(void);
void lcdPutChar(char character);
void lcdPutText(const char *text);
void lcdPutNumber(int number);
#endif // LCD_H
