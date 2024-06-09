#include <stdint.h>
#include <msp430g2553.h>

#include "../inc/Scheduler.h"

Task taskList[MaxTasks];
uint8_t taskCount = 0;

void initScheduler(void);
void addTaskToScheduler(TaskFunction function, uint16_t interval);
void runScheduler(void);

void initScheduler(void) {
    TA1CCTL0 = CCIE;                    // Enable Timer A2 interrupt
    TA1CCR0 = 1000 - 1;                 // Set timer count for 1 ms interval (assuming 1 MHz clock)
    TA1CTL = TASSEL_2 + MC_1 + TACLR;   // SMCLK, up mode, clear TAR
}

void addTaskToScheduler(TaskFunction function, uint16_t interval) {
    if (taskCount < MaxTasks) {
        taskList[taskCount].function = function;
        taskList[taskCount].interval = interval;
        taskList[taskCount].counter = interval;
        taskCount++;
    }
}

void runScheduler(void) {
    __enable_interrupt();
    while (1) {
        // Low power mode with interrupts enabled
        __bis_SR_register(LPM0_bits + GIE);
    }
}

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