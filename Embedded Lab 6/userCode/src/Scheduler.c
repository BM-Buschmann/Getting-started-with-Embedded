/**
 * @file    Scheduler.c
 * @brief   Simple cooperative scheduler for managing periodic tasks.
 *
 * This file contains the implementation of a simple cooperative scheduler
 * that allows adding tasks with specified intervals and running them based
 * on a timer interrupt.
 *
 * @date    25.05.2024
 * @authors 
 * - Bjoern Metzger
 * - Daniel Korobow
 * @version 1.0
 */

#include <stdint.h>
#include <msp430g2553.h>
#include "../inc/Scheduler.h"

// Array to hold the scheduled tasks
Task taskList[MaxTasks];

// Counter to keep track of the number of tasks
uint8_t taskCount = 0;

/**
 * @brief Initializes the scheduler and sets up the timer interrupt.
 *
 * This function configures Timer A1 to generate an interrupt every 1 ms,
 * which will be used to trigger the task scheduler.
 */
void initScheduler(void) {
    TA1CCTL0 = CCIE;                    // Enable Timer A1 interrupt
    TA1CCR0 = 1000 - 1;                 // Set timer count for 1 ms interval (assuming 1 MHz clock)
    TA1CTL = TASSEL_2 + MC_1 + TACLR;   // SMCLK, up mode, clear TAR
}

/**
 * @brief Adds a task to the scheduler.
 *
 * @param function The function pointer to the task to be scheduled.
 * @param interval The interval in milliseconds at which the task should run.
 *
 * This function adds a new task to the scheduler with the specified interval.
 * If the maximum number of tasks is reached, the task will not be added.
 */
void addTaskToScheduler(TaskFunction function, uint16_t interval) {
    if (taskCount < MaxTasks) {
        taskList[taskCount].function = function;
        taskList[taskCount].interval = interval;
        taskList[taskCount].counter = interval;
        taskCount++;
    }
}

/**
 * @brief Starts the scheduler and enters low power mode.
 *
 * This function enables interrupts and enters a low power mode,
 * allowing the scheduler to run tasks based on the timer interrupts.
 */
void runScheduler(void) {
    __enable_interrupt();
    while (1) {
        // Low power mode with interrupts enabled
        __bis_SR_register(LPM0_bits + GIE);
    }
}

/**
 * @brief Timer A1 interrupt service routine.
 *
 * This ISR is triggered every 1 ms by Timer A1. It decrements the counter
 * for each scheduled task and runs the task if its counter reaches zero.
 * After running the task, the counter is reset to the task's interval.
 */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1A0ISR(void) {
    __bic_SR_register(GIE); // Disable interrupts

    uint8_t i = 0;
    for (i = 0; i < taskCount; i++) {
        if (taskList[i].counter > 0) {
            taskList[i].counter--;
        }
        if (taskList[i].counter == 0) {
            taskList[i].function();
            taskList[i].counter = taskList[i].interval;
        }
    }

    __bis_SR_register(GIE); // Enable interrupts
}
