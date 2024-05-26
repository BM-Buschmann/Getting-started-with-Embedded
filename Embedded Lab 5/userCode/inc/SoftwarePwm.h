/**
 * @file    SoftwarePwm.h
 * @brief   Header file for the Software PWM library.
 *
 * This file contains function declarations for the software PWM library.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#ifndef SOFTWAREPWM_H
#define SOFTWAREPWM_H

#include <stdint.h>
/**
 * @brief Initializes the software PWM.
 * 
 * @param port Pointer to the port register.
 * @param pin The pin number for PWM output.
 * 
 * This function initializes the software PWM by configuring the specified pin
 * on the specified port for PWM output.
 */
extern void softwarePwmInit(volatile unsigned char *port, unsigned char pin);

/**
 * @brief Sets the frequency of the software PWM.
 * 
 * @param freq The desired frequency in Hz.
 * 
 * This function sets the frequency of the software PWM by adjusting the period
 * of the PWM waveform.
 */
extern void softwarePwmSetFrequency(uint16_t freq);

/**
 * @brief Sets the duty cycle of the software PWM.
 * 
 * @param dutyCycle The desired duty cycle as a percentage (0-100).
 * 
 * This function sets the duty cycle of the software PWM, controlling the ratio
 * of high time to low time in each period of the PWM waveform.
 */
extern void softwarePwmSetDutyCycle(uint16_t dutyCycle);

/**
 * @brief Starts the software PWM operation.
 * 
 * This function starts the software PWM, allowing the PWM waveform to generate
 * output on the configured pin.
 */
extern void softwarePwmStart();

/**
 * @brief Stops the software PWM operation.
 * 
 * This function stops the software PWM, halting the generation of the PWM waveform
 * on the configured pin.
 */
extern void softwarePwmStop();
 
#endif /* SOFTWAREPWM_H */
