#ifndef SCHEDULER_H
#define SCHEDULER_H

/**
 * @file Scheduler.h
 * @brief Header file for scheduler functions and task declarations
 */

/**
 * @brief Initialize the scheduler
 */
extern void initScheduler(void);

/**
 * @brief Run the scheduler loop
 */
extern void runScheduler(void);

// Declare task functions as weak and extern

/**
 * @brief Task function for task 0, executed every 1 ms
 */
extern void __attribute__((weak)) Task00(void);

/**
 * @brief Task function for task 1, executed every 2 ms
 */
extern void __attribute__((weak)) Task01(void);

/**
 * @brief Task function for task 2, executed every 4 ms
 */
extern void __attribute__((weak)) Task02(void);

/**
 * @brief Task function for task 3, executed every 8 ms
 */
extern void __attribute__((weak)) Task03(void);

/**
 * @brief Task function for task 4, executed every 16 ms
 */
extern void __attribute__((weak)) Task04(void);

/**
 * @brief Task function for task 5, executed every 32 ms
 */
extern void __attribute__((weak)) Task05(void);

/**
 * @brief Task function for task 6, executed every 64 ms
 */
extern void __attribute__((weak)) Task06(void);

/**
 * @brief Task function for task 7, executed every 128 ms
 */
extern void __attribute__((weak)) Task07(void);

/**
 * @brief Task function for task 8, executed every 256 ms
 */
extern void __attribute__((weak)) Task08(void);

/**
 * @brief Task function for task 9, executed every 512 ms
 */
extern void __attribute__((weak)) Task09(void);

/**
 * @brief Task function for task 10, executed every 1024 ms (1 second)
 */
extern void __attribute__((weak)) Task10(void);

/**
 * @brief Task function for task 11, executed every 2048 ms (2 seconds)
 */
extern void __attribute__((weak)) Task11(void);

#endif // SCHEDULER_H
