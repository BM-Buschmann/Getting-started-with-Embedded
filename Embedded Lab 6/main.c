#include <templateEMP.h>
#include <stdint.h>
#include <stdio.h>

#include "./userCode/inc/Scheduler.h"
#include "./userCode/inc/Hardware.h"
#include "./userCode/inc/Clock.h"
#include "./userCode/inc/Lcd.h"
#include "./userCode/inc/StringDisplay.h"

// #include <LCD.h>

// Example task functions
void LedBlinkTask(void)
{
	// Toggle LED1
	toggleLed();
}

void UpadteSecondDisplay(void)
{
	updateClock();
	printTimeDisplay(getTime());
}

int main(void)
{
	initMSP(); // Initialize MSP

	initHardware();
	lcdInit(); // Initialize LCD
	lcdEnable(1);
	lcdCursorBlink(0);
	lcdCursorShow(0);

	initScheduler();

	addTaskToScheduler(LedBlinkTask, 200);		   // Add Task1 to run every 200 ms
	addTaskToScheduler(UpadteSecondDisplay, 1000); // Add Task1 to run every 200

	runScheduler();

	return 0;
}
