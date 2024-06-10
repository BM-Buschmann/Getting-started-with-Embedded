/**
 * @file    main.c
 * @brief   Main entry point of the program.
 *
 * @date    25.05.2024
 * @author  Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#include <templateEMP.h>

#include "./userCode/inc/Scheduler.h"
#include "./userCode/inc/Hardware.h"
#include "./userCode/inc/Clock.h"
#include "./userCode/inc/StringDisplay.h"

/** Global variable to store the voltage value. */
float voltageValue = 0.0;

/** Global variable to store the ADC values. */
uint16_t adcValues = 0;

/**
 * @brief Task function to toggle LED1.
 */
void LedBlinkTask(void)
{
    // Toggle LED1
    toggleLed();
}

/** Global variable to track the state of button 1. */
static BUTTON button1State = BUTTON_NONE;

/**
 * @brief Task function to handle user input via buttons.
 */
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

/**
 * @brief Task function to update the second display with stopwatch time.
 */
void UpadteSecondDisplay(void)
{
    updateStopwatch();
    printTimeDisplay(getStopwatchTime());
}

/**
 * @brief Task function to update the ADC display with the latest ADC values.
 */
void UpadteADCDisplay(void)
{
    adcValues = readADC();
    printAdcDisplay(adcValues);
}

/**
 * @brief Task function to update the voltage display calculated from ADC values.
 */
void UpdateVoltageDisplay(void)
{
    voltageValue = ((float)3.3 / 1023) * adcValues;
    printVoltageDisplay(voltageValue);
    printGageDisplay(adcValues);
}

/**
 * @brief Main function initializing hardware and scheduler, and adding tasks to the scheduler.
 * 
 * @return int Return code.
 */
int main(void)
{
    initMSP(); // Initialize MSP

    initHardware();
    initStringDisplay();

    initScheduler();
    addTaskToScheduler(userInputTask, 100);         // Add Task1 to run every 200 ms
    addTaskToScheduler(LedBlinkTask, 200);          // Add Task2 to run every 200 ms
    addTaskToScheduler(UpadteADCDisplay, 300);      // Add Task3 to run every 300 ms
    addTaskToScheduler(UpdateVoltageDisplay, 500);  // Add Task4 to run every 500 ms
    addTaskToScheduler(UpadteSecondDisplay, 1000);  // Add Task5 to run every 1000 ms

    runScheduler();

    return 0;
}
