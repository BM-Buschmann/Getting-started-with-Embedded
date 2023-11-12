/**
 * @file    main.c
 * @brief   MSP430 Timer-Based LED Control Sample Code
 * @details This code controls LEDs on a uC-Labor Versuchsplatine V 2.3 (msp430G2553) using timers.
 *
 * @hardware uC-Labor Versuchsplatine V 2.3 msp430G2553
 *
 * @author  Björn Metzger
 * @date    2023-10-29
 * @version 1.0
 */


#include <msp430.h>
#include <templateEMP.h>

#define GRN_LED BIT0 // Green LED bitmask
#define RED_LED BIT7 // Red LED bitmask

int main(void) {
    initMSP(); // Initialize MSP430

    P3DIR |= (GRN_LED | RED_LED); // Set P3 pins as outputs for green and red LEDs
    P3OUT |= (GRN_LED | RED_LED); // Set P3 pins as outputs for green and red LEDs

    while (1) {
        P3OUT |= GRN_LED; // Turn on the green LED
        __delay_cycles(150000); // Delay for some time
        P3OUT &= ~GRN_LED; // Turn off the green LED
        __delay_cycles(50000); // Delay for some time
        P3OUT |= GRN_LED; // Turn on the green LED again
        __delay_cycles(150000); // Delay for some time
        P3OUT &= ~GRN_LED; // Turn off the green LED
        __delay_cycles(50000); // Delay for some time
        P3OUT |= (GRN_LED | RED_LED); // Turn on both green and red LEDs
        __delay_cycles(150000); // Delay for some time
        P3OUT &= ~GRN_LED; // Turn off the green LED
        __delay_cycles(50000); // Delay for some time
        P3OUT |= GRN_LED; // Turn on the green LED again
        __delay_cycles(150000); // Delay for some time
        P3OUT &= ~GRN_LED; // Turn off the green LED
        __delay_cycles(50000); // Delay for some time
        P3OUT &= ~RED_LED; // Turn off the red LED

        if (serialAvailable()) {
            if (P3OUT & GRN_LED) {
                serialPrintln(" Aktueller Zustand der roten LED: AN"); // Print the state of the red LED as ON
                serialFlush(); // Flush the serial buffer
            } else {
                serialPrintln("Aktueller Zustand der roten LED: AUS"); // Print the state of the red LED as OFF
                serialFlush(); // Flush the serial buffer
            }
        }
    }
}




/*
 **Note:**
 I've noticed that the approach of controlling an LED in this code sample is not suitable for a real-world application,
 I've due to its inefficient and blocking nature. There are several reasons for this:

 1. **Blocking Delay:**
 In my code, I used busy-wait loops with `__delay_cycles` to control the LED blinking.
 This means that the microcontroller is not performing any other tasks during these delays,
 which can be highly inefficient and wasteful of processing power.
 In a real-world application, I would want my microcontroller to be able to perform other tasks concurrently.

 2. **Serial Output Status:**
 As I correctly pointed out, the serial output status check is always executed at the end of the loop.
 Since the LEDs are turned off at the end of each loop iteration, the status check will always report that the LED is off.
 This makes it ineffective for monitoring the LED status in real-time.

 To mitigate these issues and make the LED control more practical for a real-world application,
 I can implement a non-blocking approach using interrupts and a finite state machine (FSM).
 Here's a brief description of how I can achieve this:

 1. **Use Timer Interrupts:**
 Instead of busy-wait delays, I can use timer interrupts to control the LED blinking.
 I can set up a timer to generate periodic interrupts, and in the interrupt service routine, toggle the LED state.
 This way, my microcontroller can perform other tasks while waiting for the timer interrupt.

 2. **Finite State Machine (FSM):**
 I can implement a simple FSM to manage the LED states.
 The FSM can have states like LED_ON, LED_OFF, and possibly other states if I need more complex behavior.
 The timer interrupt can trigger state transitions.

 3. **Serial Output Status:**
 To monitor the LED status, I can update a global variable within the FSM to reflect the current LED state.
 This variable can be checked in the main loop or via another interrupt to send the LED status through the serial output.

 By implementing a non-blocking approach with interrupts and an FSM, I can efficiently control the LEDs and monitor their status
 in real-time while allowing my microcontroller to perform other tasks concurrently.
 This is a more practical and efficient way to control LEDs in a real-world application.
 */

