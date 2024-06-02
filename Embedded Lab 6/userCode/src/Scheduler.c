#include <stdint.h>
#include <msp430.h>

#include "../inc/Scheduler.h"

#define TIMER_COUNT 1000 // Timer count for 1 ms interval
#define GLOBAL_MAX 2048  // Maximum value for globalCounter
#define TASK00_INTERVAL_MASK (1 - 1)
#define TASK01_INTERVAL_MASK (2 - 1)
#define TASK02_INTERVAL_MASK (4 - 1)
#define TASK03_INTERVAL_MASK (8 - 1)
#define TASK04_INTERVAL_MASK (16 - 1)
#define TASK05_INTERVAL_MASK (32 - 1)
#define TASK06_INTERVAL_MASK (64 - 1)
#define TASK07_INTERVAL_MASK (128 - 1)
#define TASK08_INTERVAL_MASK (256 - 1)
#define TASK09_INTERVAL_MASK (512 - 1)
#define TASK10_INTERVAL_MASK (1024 - 1)
#define TASK11_INTERVAL_MASK (2048 - 1)

// Global counter
volatile uint16_t globalCounter = 0;

/**
 * @brief Matches the current time slot with the tasks to be executed
 */
void slotMatch(void);

/**
 * @brief Increments the global counter and resets it if it exceeds the maximum value
 */
void incrementCounter(void);

/**
 * @brief Initializes the scheduler by configuring the timer
 */
void initScheduler(void);

/**
 * @brief Runs the scheduler loop
 */
void runScheduler(void);

/**
 * @brief Timer A0 interrupt service routine
 */
__interrupt void Timer1_A0_ISR(void);

// Task functions declared as weak and extern

/**
 * @brief Task function for task 0, executed every 1 ms
 */
void __attribute__((weak)) Task00(void) {};

/**
 * @brief Task function for task 1, executed every 2 ms
 */
void __attribute__((weak)) Task01(void) {};

/**
 * @brief Task function for task 2, executed every 4 ms
 */
void __attribute__((weak)) Task02(void) {};

/**
 * @brief Task function for task 3, executed every 8 ms
 */
void __attribute__((weak)) Task03(void) {};

/**
 * @brief Task function for task 4, executed every 16 ms
 */
void __attribute__((weak)) Task04(void) {};

/**
 * @brief Task function for task 5, executed every 32 ms
 */
void __attribute__((weak)) Task05(void) {};

/**
 * @brief Task function for task 6, executed every 64 ms
 */
void __attribute__((weak)) Task06(void) {};

/**
 * @brief Task function for task 7, executed every 128 ms
 */
void __attribute__((weak)) Task07(void) {};

/**
 * @brief Task function for task 8, executed every 256 ms
 */
void __attribute__((weak)) Task08(void) {};

/**
 * @brief Task function for task 9, executed every 512 ms
 */
void __attribute__((weak)) Task09(void) {};

/**
 * @brief Task function for task 10, executed every 1024 ms (1 second)
 */
void __attribute__((weak)) Task10(void) {};

/**
 * @brief Task function for task 11, executed every 2048 ms (2 seconds)
 */
void __attribute__((weak)) Task11(void) {};

// Inline function to match task slots
inline void slotMatch(void)
{
    if ((globalCounter & TASK00_INTERVAL_MASK) == 0) Task00();
    if ((globalCounter & TASK01_INTERVAL_MASK) == 0) Task01();
    if ((globalCounter & TASK02_INTERVAL_MASK) == 0) Task02();
    if ((globalCounter & TASK03_INTERVAL_MASK) == 0) Task03();
    if ((globalCounter & TASK04_INTERVAL_MASK) == 0) Task04();
    if ((globalCounter & TASK05_INTERVAL_MASK) == 0) Task05();
    if ((globalCounter & TASK06_INTERVAL_MASK) == 0) Task06();
    if ((globalCounter & TASK07_INTERVAL_MASK) == 0) Task07();
    if ((globalCounter & TASK08_INTERVAL_MASK) == 0) Task08();
    if ((globalCounter & TASK09_INTERVAL_MASK) == 0) Task09();
    if ((globalCounter & TASK10_INTERVAL_MASK) == 0) Task10();
    if ((globalCounter & TASK11_INTERVAL_MASK) == 0) Task11();
}

// Inline function to increment and reset the counter
inline void incrementCounter(void)
{
    globalCounter++;
    if (globalCounter >= GLOBAL_MAX)
    {
        globalCounter = 0;
    }
}

/**
 * @brief Initializes the scheduler by configuring Timer A0
 */
void initScheduler(void)
{
    TA1CCTL0 = CCIE;                  // Enable Timer A0 interrupt
    TA1CCR0 = TIMER_COUNT - 1;        // Set timer count for 1 ms interval (1 MHz clock)
    TA1CTL = TASSEL_2 + MC_1 + TACLR; // SMCLK, up mode, clear TAR
}

/**
 * @brief Runs the scheduler loop, enabling interrupts and entering low power mode
 */
void runScheduler(void)
{
    __enable_interrupt();
    while (1)
    {
        // Low power mode with interrupts enabled
        __bis_SR_register(LPM0_bits + GIE);
    }
}

/**
 * @brief Timer A0 interrupt service routine, increments the counter and matches tasks
 */
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
{
    incrementCounter();
    slotMatch();

    // Exit low power mode on return from interrupt
    __bic_SR_register_on_exit(LPM0_bits);
}
