/**
 * @file    Clock.c
 * @brief   Functions for stopwatch control.
 *
 * This file contains implementations of functions related to stopwatch control.
 * It includes functions to initialize the stopwatch, update the stopwatch time,
 * start, stop, and reset the stopwatch, and retrieve the current stopwatch time.
 *
 * @date    25.05.2024
 * @authors 
 * - Bjoern Metzger
 * - Daniel Korobow
 * @version 1.0
 */

#include <stdint.h>
#include "../inc/Clock.h"

// Global variables to hold stopwatch time and status
static Time stopwatchTime = {0, 0, 0};
static uint8_t stopwatchRunning = 0;

/**
 * @brief Initializes the stopwatch by setting the time to 0 and stopping it.
 */
void init(void) {
    stopwatchTime.hours = 0;
    stopwatchTime.minutes = 0;
    stopwatchTime.seconds = 0;
    stopwatchRunning = 0;
}

/**
 * @brief Updates the stopwatch time if it is running.
 *
 * This function increments the seconds, and if an overflow occurs,
 * it updates the minutes and hours accordingly.
 */
void updateStopwatch(void) {
    if (stopwatchRunning) {
        stopwatchTime.seconds++;
        if (stopwatchTime.seconds >= 60) {
            stopwatchTime.seconds = 0;
            stopwatchTime.minutes++;
            if (stopwatchTime.minutes >= 60) {
                stopwatchTime.minutes = 0;
                stopwatchTime.hours++;
                if (stopwatchTime.hours >= 24) {
                    stopwatchTime.hours = 0;
                }
            }
        }
    }
}

/**
 * @brief Starts the stopwatch.
 */
void startStopwatch(void) {
    stopwatchRunning = 1;
}

/**
 * @brief Stops the stopwatch.
 */
void stopStopwatch(void) {
    stopwatchRunning = 0;
}

/**
 * @brief Resets the stopwatch time to 0 and stops it.
 */
void resetStopwatch(void) {
    stopwatchRunning = 0;
    stopwatchTime.hours = 0;
    stopwatchTime.minutes = 0;
    stopwatchTime.seconds = 0;
}

/**
 * @brief Gets the current stopwatch time.
 *
 * @return The current time of the stopwatch.
 */
Time getStopwatchTime(void) {
    return stopwatchTime;
}
