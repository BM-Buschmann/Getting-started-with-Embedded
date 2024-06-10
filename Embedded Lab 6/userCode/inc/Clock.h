#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

// Define the Time structure
typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} Time;

// Function to initialize the stopwatch
extern void init(void);

// Function to update the stopwatch (to be called periodically)
extern void updateStopwatch(void);

// Function to start the stopwatch
extern void startStopwatch(void);

// Function to stop the stopwatch
extern void stopStopwatch(void);

// Function to reset the stopwatch
extern void resetStopwatch(void);

// Function to get the current stopwatch time
extern Time getStopwatchTime(void);



#endif // CLOCK_H
