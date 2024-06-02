#include <stdint.h>

#include "../inc/Clock.h"

// Global variables to hold time and stopwatch status
static Time currentTime = {0, 0, 0};
static Time stopwatchStartTime = {0, 0, 0};
static uint8_t stopwatchRunning = 0;

void init(void) {
    // Initialize RTC module or any other necessary initialization
}

void updateClock(void) {
    // Increment seconds and handle overflow
    currentTime.seconds++;
    if (currentTime.seconds >= 60) {
        currentTime.seconds = 0;
        currentTime.minutes++;
        if (currentTime.minutes >= 60) {
            currentTime.minutes = 0;
            currentTime.hours++;
            if (currentTime.hours >= 24) {
                currentTime.hours = 0;
            }
        }
    }
}

Time getTime(void) {
    // Return the current time
    return currentTime;
}

void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    // Set the current time to a predefined value
    currentTime.hours = hours;
    currentTime.minutes = minutes;
    currentTime.seconds = seconds;
}

void startStopwatch(void) {
    // Start the stopwatch
    stopwatchStartTime = currentTime;
    stopwatchRunning = 1;
}

void stopStopwatch(void) {
    // Stop the stopwatch
    stopwatchRunning = 0;
}

void resetStopwatch(void) {
    // Reset the stopwatch
    stopwatchStartTime = currentTime;
}

Time getStopwatchElapsedTime(void) {
    // Calculate elapsed time if stopwatch is running
    Time elapsedTime = {0, 0, 0};
    if (stopwatchRunning) {
        // Calculate elapsed time since stopwatch start time
        uint8_t secondsElapsed = currentTime.seconds - stopwatchStartTime.seconds;
        uint8_t minutesElapsed = currentTime.minutes - stopwatchStartTime.minutes;
        uint8_t hoursElapsed = currentTime.hours - stopwatchStartTime.hours;
        
        // Handle borrow
        if (secondsElapsed < 0) {
            secondsElapsed += 60;
            minutesElapsed--;
        }
        if (minutesElapsed < 0) {
            minutesElapsed += 60;
            hoursElapsed--;
        }
        if (hoursElapsed < 0) {
            hoursElapsed += 24;
        }
        
        elapsedTime.hours = hoursElapsed;
        elapsedTime.minutes = minutesElapsed;
        elapsedTime.seconds = secondsElapsed;
    }
    return elapsedTime;
}