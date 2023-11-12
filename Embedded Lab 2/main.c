/**
 * @file    main.c
 * @brief   MSP430 Lab Embedded Programming
 * @details This program demonstrates button handling, LED control, and buzzer usage on the uC-Labor Versuchsplatine V 2.3 with MSP430G2553 microcontroller.
 *
 * @hardware uC-Labor Versuchsplatine V 2.3 msp430G2553
 * - PB5 connected to CON3:P1.5
 * - PB6 connected to CON3:P1.6
 * - PB7 connected to CON3:P1.7
 * - Red LED (JP8-LED rt) connected to CON3:P3.0
 * - Green LED (JP8-LED gn) connected to CON3:P3.1
 * - Buzzer (X1-BUZZER) connected to CON3:P3.2
 * - Blue LED (at X1: Heater) connected to CON3:P3.3
 *
 * @author  Björn Metzger
 * @date    2023-11-11
 * @version 1.0
 */


//* ---------------------------------------- Includes ---------------------------------------------*/

#include <msp430.h>
#include <templateEMP.h>


//* ----------------------------------------- Defines ---------------------------------------------*/

/**
 * @brief Button definitions.
 */
#define BUTTON_1 BIT5
#define BUTTON_2 BIT6
#define BUTTON_3 BIT7

/**
 * @brief LED definitions.
 */
#define LED_RED BIT0
#define LED_GRN BIT1
#define LED_BLU BIT3

/**
 * @brief Buzzer definition.
 */
#define BUZZER BIT2

/**
 * @brief Boolean values.
 */
#define true 1
#define false 0

/**
 * @brief Debounce delay in cycles.
 */
#define DEBOUNCE_DELAY_CYCLES 20000

/**
 * @brief Macros for LED and Buzzer control.
 */
#define LED_RED_ON P3OUT |= LED_RED;
#define LED_RED_OFF P3OUT &=~ LED_RED;
#define LED_GRN_ON P3OUT |= LED_GRN;
#define LED_GRN_OFF P3OUT &=~ LED_GRN;
#define LED_BLU_TOGGEL P3OUT ^= LED_BLU;
#define LED_BLU_ON P3OUT |= LED_BLU;
#define LED_BLU_OFF P3OUT &=~ LED_BLU;
#define BUZZER_ON P3OUT |= BUZZER;
#define BUZZER_OFF P3OUT &=~ BUZZER;
#define MS_100_DELAY __delay_cycles(100000);


//* -------------------------------------- Declarations ------------------------------------------*/

/**
 * @brief Initializes a GPIO pin as an input with optional interrupt setup.
 *
 * @param bit The bit number of the pin to be initialized.
 * @param setupInterupt Flag indicating whether to set up interrupts (1 for true, 0 for false).
 */
void initInput(int bit, int setupInterupt);

/**
 * @brief Initializes a GPIO pin as an output.
 *
 * @param bit The bit number of the pin to be initialized.
 */
void initOutput(int bit);

/**
 * @brief Plays a simple melody and blinks the LED.
 *
 */
void playSoundAndBlink();


//* ---------------------------------------- Definitions ------------------------------------------*/

int main(void) {
    volatile unsigned int buttonsPressed = 0;
    volatile unsigned int buttonTwoPressed = false;

    // Initialize MSP430 hardware
    initMSP();

    // Disable Watchdog Timer
    WDTCTL = WDTPW | WDTHOLD;

    // Initialize input and output pins
    initInput(BUTTON_1, false);
    initInput(BUTTON_2, false);
    initInput(BUTTON_3, true);

    initOutput(LED_RED);
    initOutput(LED_GRN);
    initOutput(LED_BLU);
    initOutput(BUZZER);

    while (1) {
        // Check if BUTTON_1 is pressed
        if (!(P1IN & BUTTON_1)) {
            buttonsPressed |= BUTTON_1;  // Set BUTTON_1 flag
            __delay_cycles(DEBOUNCE_DELAY_CYCLES);

            // Check if BUTTON_1 is still pressed after debounce
            if (P1IN & BUTTON_1) {
                buttonsPressed &= ~BUTTON_1;  // Clear BUTTON_1 flag
            }
        }

        // Check if BUTTON_2 is pressed
        if (!(P1IN & BUTTON_2)) {
            buttonsPressed |= BUTTON_2;  // Set BUTTON_2 flag
            __delay_cycles(DEBOUNCE_DELAY_CYCLES);

            // Check if BUTTON_2 is still pressed after debounce
            if (P1IN & BUTTON_2) {
                buttonsPressed &= ~BUTTON_2;  // Clear BUTTON_2 flag
            }
        }

        // Check if both BUTTON_1 and BUTTON_2 are pressed
        if (buttonsPressed == (BUTTON_1 | BUTTON_2)) {
            playSoundAndBlink(BUZZER, LED_RED);
            buttonsPressed = 0;  // Clear button flags
        }

        // Check if only BUTTON_1 is pressed
        if (buttonsPressed == BUTTON_1) {
            LED_RED_ON
            buttonsPressed = 0;  // Clear button flags
        } else if (!(buttonsPressed == BUTTON_1)) {
            LED_RED_OFF
        }

        // Check if BUTTON_2 is pressed and the associated action is not performed yet
        if ((buttonsPressed == BUTTON_2) && !buttonTwoPressed) {
            LED_GRN_ON
            buttonTwoPressed = true;

            // Delay for 250,000 cycles
            __delay_cycles(250000);

            LED_GRN_OFF
            buttonsPressed = 0;  // Clear button flags
        } else if (!(buttonsPressed == BUTTON_2)) {
            buttonTwoPressed = false;  // Clear the action flag
        }
    }
}

/**
 * @brief Initializes a GPIO pin as an input with optional interrupt setup.
 *
 * @param bit The bit number of the pin to be initialized.
 * @param setupInterupt Flag indicating whether to set up interrupts (1 for true, 0 for false).
 */
void initInput(int bit, int setupInterupt) {
    P1DIR &= ~bit;      // Set as input
    P1REN |= bit;       // Enable pull-resistors
    P1OUT |= bit;       // Set to pull-up
    if (setupInterupt == 1) {
        P1IES |= bit;    // High/Low-Edge
        P1IFG &= ~bit;   // Clear interrupt flag
        P1IE |= bit;     // Enable interrupt
    }
}

/**
 * @brief Initializes a GPIO pin as an output.
 *
 * @param bit The bit number of the pin to be initialized.
 */
void initOutput(int bit) {
    P3DIR |= bit;       // Set as output
    P3OUT &= ~bit;      // Set to low initially
}

/**
 * @brief Plays a simple melody and blinks the LED.
 *
 * @param buzzer The buzzer pin.
 * @param led The LED pin.
 */
void playSoundAndBlink() {
    int i;

    // Play G note
    LED_RED_ON
    for (i = 356; i > 0; i--) {
        BUZZER_ON
        __delay_cycles(1262);
        BUZZER_OFF
        __delay_cycles(1262);
    }
    LED_RED_OFF
    MS_100_DELAY
    LED_RED_ON

    // Play E note
    for (i = 297; i > 0; i--) {
        BUZZER_ON
        __delay_cycles(1515);
        BUZZER_OFF
        __delay_cycles(1515);
    }
    LED_RED_OFF
    MS_100_DELAY
    LED_RED_ON

    // Play C note
    for (i = 238; i > 0; i--) {
        BUZZER_ON
        __delay_cycles(1894);
        BUZZER_OFF
        __delay_cycles(1894);
    }
    LED_RED_OFF
    MS_100_DELAY
}


//* ----------------------------------------- Interrupts -------------------------------------------*/

/**
 * @brief Port 1 interrupt service routine.
 */
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void) {
    if (P1IFG & BIT7) {
        LED_BLU_TOGGEL
        P1IFG &= ~BIT7;
    }
}
