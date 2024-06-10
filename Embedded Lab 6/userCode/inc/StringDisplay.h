#ifndef STRING_DISPLAY_H
#define STRING_DISPLAY_H

#include <stdint.h>


extern void initStringDisplay();
extern void printTimeDisplay(Time current);
extern void printAdcDisplay(uint16_t adcValue);
extern void printVoltageDisplay(float value);
extern void printGageDisplay(uint16_t adcValue);

#endif /* LCD_H */
