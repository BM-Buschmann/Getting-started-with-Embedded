#ifndef CLOCK_H
#define CLOCK_H

// Structure to hold time information
typedef struct {
    uint8_t hours;   // Hours (0-23)
    uint8_t minutes; // Minutes (0-59)
    uint8_t seconds; // Seconds (0-59)
} Time;

/**
 * @brief Updates the clock every 1 second
 * 
 * @note this methode needs to be called every second otherwise the clock is not in sync
 */
extern void updateClock(void);

/**
 * @brief Gets the current time as a Time structure
 * 
 * @return Time structure containing current time
 */
extern Time getTime(void);

/**
 * @brief Sets the current time to a predefined value
 * 
 * @param hours Hours (0-23)
 * @param minutes Minutes (0-59)
 * @param seconds Seconds (0-59)
 */
extern void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

/**
 * @brief Starts the stopwatch
 */
extern void startStopwatch(void);

/**
 * @brief Stops the stopwatch
 */
extern void stopStopwatch(void);

/**
 * @brief Resets the stopwatch
 */
extern void resetStopwatch(void);

/**
 * @brief Gets the elapsed time of the stopwatch
 * 
 * @return Time structure containing elapsed time
 */
extern Time getStopwatchElapsedTime(void);

#endif // CLOCK_H
