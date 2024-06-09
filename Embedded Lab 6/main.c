#include <templateEMP.h>

#include "./userCode/inc/Scheduler.h"
#include "./userCode/inc/Hardware.h"
#include "./userCode/inc/Clock.h"
#include "./userCode/inc/StringDisplay.h"


float voltageValue = 0.0;
uint16_t adcValues = 0;

// Example task functions
void LedBlinkTask(void)
{
	// Toggle LED1
	toggleLed();
}

// Global variable to track the state of button 1
static BUTTON button1State = BUTTON_NONE;

void userInputTask(void)
{
    // Determine which button is pressed
    BUTTON pressedButton = getPressedButton();

    // Button 1 behavior
    if (pressedButton == BUTTON_1)
    {
        if (button1State == BUTTON_NONE)
        {
            startStopwatch();
            button1State = BUTTON_1;
        }
        else if (button1State == BUTTON_1)
        {
            stopStopwatch();
            button1State = BUTTON_NONE;
        }
    }
    // Button 2 behavior
    else if (pressedButton == BUTTON_2)
    {
        resetStopwatch();
        button1State = BUTTON_NONE;
    }
}


void UpadteSecondDisplay(void)
{
	updateClock();
	printTimeDisplay(getStopwatchElapsedTime());
}

void UpadteADCDisplay(void)
{
	adcValues = readADC();
	printAdcDisplay(adcValues);
}

void UpdateVoltageDisplay(void){
	voltageValue = ((float)3.3 / 1023) * adcValues;
	printVoltageDisplay(voltageValue);
	printGageDisplay(adcValues);
}



int main(void)
{
	initMSP(); // Initialize MSP

	initHardware();
	initStringDisplay();

	initScheduler();
	addTaskToScheduler(LedBlinkTask, 200);		   // Add Task1 to run every 200 ms
	addTaskToScheduler(UpadteSecondDisplay, 1000); // Add Task1 to run every 200
	addTaskToScheduler(UpadteADCDisplay, 300);	   // Add Task1 to run every 200
	addTaskToScheduler(UpdateVoltageDisplay, 500);	   // Add Task1 to run every 200
	addTaskToScheduler(userInputTask, 350);
	runScheduler();

	return 0;
}
