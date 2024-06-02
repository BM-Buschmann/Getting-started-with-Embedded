#include <LCD.h>


void initSerialDisplay(){
    lcd_init();                              // Initialize LCD 
    lcd_enable(1);                            
    lcd_clear();
}