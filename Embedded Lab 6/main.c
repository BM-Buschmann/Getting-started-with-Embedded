#include <templateEMP.h>
#include <stdint.h>
#include <lcd.h>

#include <LCD.h>
#include "./userCode/inc/Scheduler.h"
#include "./userCode/inc/Clock.h"	 // Include the clock module header
#include "./userCode/inc/Hardware.h" // Include the clock module header

// Define a counter to keep track of milliseconds
static uint16_t millisecondCounter = 0;

void Task00()
{
	// Increment the millisecond counter
	millisecondCounter++;

	// If 1 second has elapsed (1000 milliseconds), call updateClock and reset the counter
	if ((millisecondCounter % 1000) == 0)
	{
		updateClock();
		millisecondCounter = 0;
	}

	if ((millisecondCounter % 200) == 0)
	{
		toggleLed(PIN_5);
	}
}

void Task08()
{
	
}

void Task11()
{
	// serialPrintInt(getTime().hours);
	// serialPrint(":");
	// serialPrintInt(getTime().minutes);
	// serialPrint(":");
	// serialPrintInt(getTime().seconds);
	// serialPrintln(",");

	// serialPrintInt((int)getPressedButton());
	serialPrintInt((int)readADC());
}

// main routine
void main(void)
{
	initMSP(); // Initialize MSP

	initScheduler();

	initHardware();

	 // Initialize the LCD
    lcdInit();

    // Clear the LCD display
    lcdClear();

    // Set the cursor to the beginning of the first row
    lcdSetCursor(0, 0);

    // Display "Hello, World!" on the first row
    const char *message = "Hello, World!";
    while (*message) {
        lcdSendData(*message++);
    }

    // Set the cursor to the beginning of the second row
    lcdSetCursor(1, 0);

    // Display "MSP430 Rocks!" on the second row
    message = "MSP430 Rocks!";
    while (*message) {
        lcdSendData(*message++);
    }

}
