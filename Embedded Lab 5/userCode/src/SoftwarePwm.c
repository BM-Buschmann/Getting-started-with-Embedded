/**
 * @file    SoftwarePwm.c
 * @brief   Functions for software-based PWM generation.
 *
 * This file contains implementations of functions related to software-based PWM generation.
 * It includes functions to initialize the PWM, set frequency and duty cycle, start and stop PWM.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#include <stdint.h>
#include <msp430.h>
#include "../inc/SoftwarePwm.h"

// Global variables to store port and pin
volatile unsigned char *pwmPort;
unsigned char pwmPin;

void softwarePwmInit(volatile unsigned char *port, unsigned char pin);
void softwarePwmSetFrequency(uint16_t freq);
void softwarePwmSetDutyCycle(uint16_t dutyCycle);
void softwarePwmStart();
void softwarePwmStop();

/**
 * @brief Initializes Timer1 for PWM operation.
 * 
 * @param port Pointer to the port register.
 * @param pin The pin number for PWM output.
 * 
 * This function initializes Timer1 for PWM operation. It configures the specified pin
 * on the specified port as an output and sets Timer1 to Up Mode with Capture/Compare
 * interrupt enabled.
 */
void softwarePwmInit(volatile unsigned char *port, unsigned char pin)
{
    // Save port and pin information
    pwmPort = port;
    pwmPin = pin;

    // Set pin as output
    *port |= (1 << pin);

    // Stop Timer1
    TA1CTL = 0;

    // Set Timer1 to Up Mode
    TA1CTL |= TASSEL_2 | MC_1;

    // Set the output mode to Reset/Set
    TA1CCTL1 |= OUTMOD_7;

    // Enable Timer1 Capture/Compare interrupt
    TA1CCTL1 |= CCIE;
}

/**
 * @brief Sets the PWM frequency.
 * 
 * @param freq The desired frequency in Hz.
 * 
 * This function sets the PWM frequency by calculating the period based on the
 * desired frequency and setting the Timer1 Capture/Compare register accordingly.
 */
void softwarePwmSetFrequency(uint16_t freq)
{
    // Calculate the period based on frequency
    uint16_t period = 1000000 / freq; // 1 MHz clock source

    // Set the period
    TA1CCR0 = period - 1;
}

/**
 * @brief Sets the PWM duty cycle.
 * 
 * @param dutyCycle The desired duty cycle as a percentage (0-100).
 * 
 * This function sets the PWM duty cycle by calculating the duty cycle value
 * and setting the Timer1 Capture/Compare register accordingly.
 */
void softwarePwmSetDutyCycle(uint16_t dutyCycle)
{
    // Calculate the duty cycle value
    uint16_t dutyValue = (TA1CCR0 + 1) * dutyCycle / 100;

    // Set the duty cycle
    TA1CCR1 = dutyValue;
}

/**
 * @brief Starts the PWM operation.
 * 
 * This function starts Timer1 for PWM operation.
 */
void softwarePwmStart()
{
    // Start Timer1
    TA1CTL |= MC_1;
}

/**
 * @brief Stops the PWM operation.
 * 
 * This function stops Timer1 to halt PWM operation.
 */
void softwarePwmStop()
{
    // Stop Timer1
    TA1CTL &= ~MC_3;
}

/**
 * @brief Timer1 Capture/Compare interrupt service routine.
 * 
 * This ISR toggles the specified pin on the specified port when Timer1 Capture/Compare
 * interrupt occurs.
 */
#pragma vector = TIMER1_A1_VECTOR
__interrupt void timer1ISR(void)
{
    // Toggle specified pin on specified port
    *pwmPort ^= (1 << pwmPin);

    // Clear the interrupt flag
    TA1CCTL1 &= ~CCIFG;
}
