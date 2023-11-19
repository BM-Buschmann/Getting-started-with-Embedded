/**
 * @file    main.c
 * @brief   MSP430 Lab Embedded Programming
 * @details
 *
 * @hardware uC-Labor Versuchsplatine V 2.3 msp430G2553
 *
 *
 * @author  Bj�rn Metzger
 * @date    2023-11-11
 * @version 1.0
 */

//* ---------------------------------------- Includes ---------------------------------------------*/

// Toolchain includes
#include <msp430.h>
#include <templateEMP.h>

// Standard includes
#include <stdint.h>
#include <stdbool.h>

//* ----------------------------------------- Defines ---------------------------------------------*/

#define LED_RED BIT0
#define LED_GRN BIT1
#define LED_BLU BIT2

#define BUTTON_ONE BIT3
#define BUTTON_TWO BIT4
#define BUTTON_THREE BIT5

#define PORT_ONE P1IN
#define PORT_THREE P3OUT

#define SEQUENCE_COUNT 5
#define NUMBER_OF_LEDS 3
#define DEBOUNCE_DELAY_CYCLES 20000

//* --------------------------------------- Typedefines --------------------------------------------*/

enum ExecutionState
{
    stateNull,
    stateOne,
    stateTwo,
    stateThree,
    stateFour,
    stateFive,
};

typedef struct ledSequence
{
    uint8_t ledOrder[NUMBER_OF_LEDS];
    uint16_t onTime;
    uint16_t offTime;
} ledSequence;

//* -------------------------------------- Variable Declarations ------------------------------------------*/

// LED sequences
ledSequence sequences[SEQUENCE_COUNT] = {
    {{LED_RED, LED_GRN, LED_BLU}, 2000, 2000},
    {{LED_GRN, LED_BLU, LED_RED}, 1000, 1000},
    {{LED_BLU, LED_RED, LED_GRN}, 500, 500},
    {{LED_RED, LED_BLU, LED_GRN}, 300, 300},
    {{LED_GRN, LED_RED, LED_BLU}, 100, 100}};

// User input array and control variables
uint8_t ledOrderUserInput[NUMBER_OF_LEDS];
uint8_t currentSequenceIndex = 0;
uint8_t currentExecutionState = stateNull;

// Flag to check current LED status
uint8_t curentLedStatus = 0;

// Timer-related variables
volatile uint16_t timerCount = 0;
volatile uint8_t ledIndex = 0;

// Flags for control flow
bool userInputFalse = false;
bool lastSequence = false;

//* -------------------------------------- Method Declarations ------------------------------------------*/

// Initialization method
void initialize(void);

// Animation methods
void playStartAnimation(void);
void playCurrentSequenceAnimation(ledSequence *currentSequence);
void playRoundWonAnimation(void);  // renamed the roundwon() method to playRoundWonAnimation() to fit the naming sheme
void playRoundLostAnimation(void); // renamed the roundlost() method to playRoundLostAnimation() to fit the naming sheme
void playGameWonAnimation(void);   // renamed the  gamewon() method to playGameWonAnimation() to fit the naming sheme

// Timer control methods
void startTimer(void);
void stopTimer(void);

// User input methods
void collectUserInput(uint8_t *ledOrderUserInput);

// Helper methods
bool compareArrays(uint8_t *array1, uint8_t *array2);
bool debounceInput(uint8_t port, uint8_t pin);

//* ---------------------------------------- Definitions ------------------------------------------*/
int main(void)
{
    bool result = false;

    initialize();

    while (1)
    {
        switch (currentExecutionState)
        {
        case stateNull: // Play starting animation
            if (debounceInput(PORT_ONE, BUTTON_ONE) || debounceInput(PORT_ONE, BUTTON_TWO) || debounceInput(PORT_ONE, BUTTON_THREE))
            {
                playStartAnimation();
                currentExecutionState = stateOne; // Move to the next execution state
            }
            break;

        case stateOne: // Play LED sequence for the user
            playCurrentSequenceAnimation(&sequences[currentSequenceIndex]);
            currentExecutionState = stateTwo;
            break;

        case stateTwo: // Collect user input
            collectUserInput(&ledOrderUserInput);
            currentExecutionState = stateThree;
            break;

        case stateThree: // Compare user input and played sequence

            result = compareArrays(&sequences[currentSequenceIndex].ledOrder, &ledOrderUserInput);

            if (result && currentSequenceIndex == 5) // User input identical to displayed sequence for the last round
            {
                playGameWonAnimation();
                playStartAnimation();
                currentSequenceIndex = 0;
                currentExecutionState = stateNull;
            }
            else if (result) // User input identical to displayed sequence
            {
                playRoundWonAnimation();
                currentSequenceIndex++; // Increment the current sequence index
                currentExecutionState = stateOne;
            }
            else // User input not identical to displayed sequence
            {
                playRoundLostAnimation();
                currentSequenceIndex = 0; // Reset the current sequence index
                currentExecutionState = stateNull;
            }
            break;

        default:
            break;
        }
    }
}

/**
 * @brief Initializes the system.
 *
 * This function performs the necessary initialization steps to set up the system.
 * It initializes the MSP, disables interrupts, stops the watchdog timer, configures
 * LED pins, sets button pins as input with pull-up resistors, and configures Timer A.
 */
void initialize()
{
    initMSP(); // Initialize the MSP

    __disable_interrupt(); // Disable interrupts globally

    WDTCTL = WDTPW + WDTHOLD; // Stop the watchdog timer

    P3DIR |= (LED_RED + LED_GRN + LED_BLU);  // Set LED pins as output
    P3OUT &= ~(LED_RED + LED_GRN + LED_BLU); // Turn off all LEDs initially

    P1DIR &= ~(BUTTON_ONE + BUTTON_TWO + BUTTON_THREE); // Set button pins as input
    P1REN |= (BUTTON_ONE + BUTTON_TWO + BUTTON_THREE);  // Enable pull-up resistor for buttons
    P1OUT |= (BUTTON_ONE + BUTTON_TWO + BUTTON_THREE);  // Set pull-up resistor for buttons

    // Configure Timer A
    TA0CTL = TASSEL_2 + MC_0 + TACLR; // Use the SMCLK as the clock source, disable timer for now, clear timer
    TA0CCR0 = 1000;                   // Set CCR0 to produce 100 ms per count
    TA0CCTL0 &= ~CCIE;                // Enable the interrupt for CCR0

    __enable_interrupt(); // Enable global interrupts
}

/**
 * @brief Plays the start animation.
 *
 * This function turns on all LEDs, introduces a delay of 2 seconds, and then turns off all LEDs.
 * The delay is achieved using the __delay_cycles function.
 */
void playStartAnimation()
{
    PORT_THREE |= (LED_RED + LED_GRN + LED_BLU);  // Turn on all LEDs
    __delay_cycles(2000000);                      // Delay for 2 seconds
    PORT_THREE &= ~(LED_RED + LED_GRN + LED_BLU); // Turn off all LEDs
}

/**
 * @brief Plays the animation based on the provided LED sequence.
 *
 * This function plays the animation based on the LED sequence provided.
 * It toggles LEDs according to the sequence's on and off times using a timer.
 *
 * @param currentSequence Pointer to the LED sequence structure.
 */
void playCurrentSequenceAnimation(ledSequence *currentSequence)
{
    startTimer(); // Start the timer for animation

    uint8_t curentLedIndex = 0; // Declare curentLedIndex within the method

    while (curentLedIndex < NUMBER_OF_LEDS)
    {
        uint8_t currentLedPin = currentSequence->ledOrder[curentLedIndex];
        uint16_t currentPeriodTime = currentSequence->offTime + currentSequence->onTime;

        if (timerCount >= currentPeriodTime)
        {
            timerCount = 0;
            curentLedIndex++;
        }
        else
        {
            // If the current time is within the "on" time and the LEDs are off, turn them on
            if ((timerCount <= currentSequence->onTime) && !(PORT_THREE & currentLedPin))
            {
                PORT_THREE |= currentLedPin; // Turn on all LEDs
            }
            // If the current time is greater than 250 and the LEDs are on, turn them off
            else if ((timerCount > currentSequence->onTime) && (PORT_THREE & currentLedPin))
            {
                PORT_THREE &= ~currentLedPin;
            }
            else
            {
                // Errors do nothing for now
            }
        }
    }

    stopTimer(); // Stop the timer after animation
}

/**
 * @brief Plays the animation indicating a won round.
 *
 * This function plays the animation indicating a won round.
 * It alternates between turning on and off all LEDs for a specified period.
 */
void playRoundWonAnimation()
{
    startTimer(); // Start the timer for animation

    uint8_t curentLedIndex = 0;

    uint16_t currentPeriodTime = 500; // 500ms for each state (on and off)

    while (curentLedIndex < 4) // Repeat the sequence twice
    {
        if (timerCount >= currentPeriodTime)
        {
            timerCount = 0;
            curentLedIndex++;
        }
        else
        {
            // If the current time is within the "on" time and the LEDs are off, turn them on
            if ((timerCount <= 250) && !(PORT_THREE & (LED_RED + LED_GRN + LED_BLU)))
            {
                PORT_THREE |= (LED_RED + LED_GRN + LED_BLU); // Turn on all LEDs
            }
            // If the current time is greater than 250 and the LEDs are on, turn them off
            else if ((timerCount > 250) && (PORT_THREE & (LED_RED + LED_GRN + LED_BLU)))
            {
                PORT_THREE &= ~(LED_RED + LED_GRN + LED_BLU);
            }
            else
            {
                // Errors do nothing for now
            }
        }
    }
    PORT_THREE &= ~(LED_RED + LED_GRN + LED_BLU); // Turn off all LEDs
    stopTimer();                                  // Stop the timer after animation
}

/**
 * @brief Plays the animation indicating a lost round.
 *
 * This function plays the animation indicating a lost round.
 * It cycles through different LED states for a specified period.
 */
void playRoundLostAnimation()
{
    startTimer(); // Start the timer for animation

    uint8_t curentLedIndex = 0;

    while (curentLedIndex < 3) // Repeat the sequence three times
    {
        uint16_t currentPeriodTime = 1000; // 1000ms for each cycle

        if (timerCount >= currentPeriodTime) // If the loop reached the desired period rool over to 0 and start again
        {
            timerCount = 0;
            curentLedIndex++;
        }
        else
        {
            // If the current time is within the "on" time and the LEDs are off, turn them on
            if ((timerCount <= 333))
            {
                PORT_THREE |= LED_BLU; // Turn on all LEDs
                PORT_THREE &= ~(LED_RED + LED_GRN);
            }
            // If the current time is greater than 333 and less than or equal to 666, turn on the red LED
            else if ((timerCount > 333) && (timerCount <= 666))
            {
                PORT_THREE |= LED_RED; // Turn on the red LED
                PORT_THREE &= ~(LED_GRN + LED_BLU);
            }
            // If the current time is greater than 666, turn on the green LED
            else if ((timerCount > 666))
            {
                PORT_THREE |= LED_GRN; // Turn on the green LED
                PORT_THREE &= ~(LED_RED + LED_BLU);
            }
            else
            {
                // Errors do nothing for now
            }
        }
    }
    PORT_THREE &= ~(LED_RED + LED_GRN + LED_BLU); // Turn off all LEDs
    stopTimer();                                  // Stop the timer after animation
}

/**
 * @brief Plays the animation indicating a won game.
 *
 * This function plays the animation indicating a won game.
 * It cycles through different LED states for a specified period.
 */
void playGameWonAnimation()
{
    startTimer(); // Start the timer for animation

    uint8_t curentLedIndex = 0;

    while (curentLedIndex < 3) // Repeat the sequence three times
    {
        uint16_t currentPeriodTime = 1000; // 1000ms for each cycle

        if (timerCount >= currentPeriodTime)
        {
            timerCount = 0;
            curentLedIndex++;
        }
        else
        {
            // If the current time is within the "on" time and the LEDs are off, turn them on
            if ((timerCount <= 333))
            {
                PORT_THREE |= LED_BLU; // Turn on all LEDs
                PORT_THREE &= ~(LED_GRN + LED_RED);
            }
            // If the current time is greater than 333 and less than or equal to 666, turn on the green LED
            else if ((timerCount > 333) && (timerCount <= 666))
            {
                PORT_THREE |= LED_GRN; // Turn on the green LED
                PORT_THREE &= ~(LED_RED + LED_BLU);
            }
            // If the current time is greater than 666, turn on the red LED
            else if ((timerCount > 666))
            {
                PORT_THREE |= LED_RED; // Turn on the red LED
                PORT_THREE &= ~(LED_BLU + LED_GRN);
            }
            else
            {
                // Errors do nothing for now
            }
        }
    }

    PORT_THREE &= ~(LED_RED + LED_GRN + LED_BLU); // Turn off all LEDs
    stopTimer();                                  // Stop the timer after animation
}

/**
 * @brief Starts the Timer A0 in up mode and enables interrupt vectors.
 *
 * This function configures Timer A0 in up mode and enables the interrupt vectors for CCR0.
 * It allows the timer to start counting and trigger interrupts at the specified interval.
 */
void startTimer()
{
    TA0CTL |= MC_1; // Set timer to up mode
    CCTL0 |= CCIE;  // Enable interrupt vectors for CCR0
}

/**
 * @brief Stops Timer A0 and disables interrupt vectors.
 *
 * This function stops Timer A0 and disables the interrupt vectors for CCR0.
 * It halts the timer from counting and prevents further interrupts.
 */
void stopTimer()
{
    TA0CTL |= MC_0; // Set timer to off state
    CCTL0 &= ~CCIE; // Disable interrupt vectors for CCR0
}

/**
 * @brief Collects user input by polling buttons one to three with debouncing.
 *
 * This function polls buttons one to three until all of them have been pressed.
 * The order in which the buttons have been pressed is stored in the array ledOrderUserInput.
 * Debouncing is applied to the button inputs using the debounceInput function.
 *
 * @param ledOrderUserInput Pointer to the array where the user input will be stored.
 */
void collectUserInput(uint8_t *ledOrderUserInput)
{
    uint8_t currentIndex = 0;

    // Continue polling buttons until all three buttons are pressed
    while (currentIndex < NUMBER_OF_LEDS)
    {
        if (debounceInput(PORT_ONE, BUTTON_ONE))
        {
            // Button one is pressed, store it in the array
            ledOrderUserInput[currentIndex] = LED_RED;
            currentIndex++;
            while (debounceInput(PORT_ONE, BUTTON_ONE))
            {
                // Wait for button release to avoid multiple presses being registered
            }
        }
        else if (debounceInput(PORT_ONE, BUTTON_TWO))
        {
            // Button two is pressed, store it in the array
            ledOrderUserInput[currentIndex] = LED_GRN;
            currentIndex++;
            while (debounceInput(PORT_ONE, BUTTON_TWO))
            {
                // Wait for button release to avoid multiple presses being registered
            }
        }
        else if (debounceInput(PORT_ONE, BUTTON_THREE))
        {
            // Button three is pressed, store it in the array
            ledOrderUserInput[currentIndex] = LED_BLU;
            currentIndex++;
            while (debounceInput(PORT_ONE, BUTTON_THREE))
            {
                // Wait for button release to avoid multiple presses being registered
            }
        }
    }
}

/**
 * @brief Compares two arrays to check if they are identical.
 *
 * This function compares two arrays to check if they are identical.
 *
 * @param array1 Pointer to the first array.
 * @param array2 Pointer to the second array.
 * @param size Size of the arrays.
 * @return true if the arrays are identical, false otherwise.
 */
bool compareArrays(uint8_t *array1, uint8_t *array2)
{
    uint8_t arrayIndex;
    for (arrayIndex = NUMBER_OF_LEDS; arrayIndex > 0; arrayIndex--)
    {
        if (array1[arrayIndex] != array2[arrayIndex])
        {
            // Arrays are not identical
            return false;
        }
    }

    // Arrays are identical
    return true;
}

/**
 * @brief Debounces a button input using a time delay.
 *
 * This function debounces a button input by checking its state over time.
 * It uses a time delay to filter out spurious signals and provide a stable button state.
 *
 * @param port Port register of the button pin.
 * @param pin Pin number of the button.
 * @return 1 if the button is pressed after debouncing, 0 otherwise.
 */
bool debounceInput(uint8_t port, uint8_t pin)
{
    bool buttonState = false; // Default to not pressed

    if (!(port & pin))
    {
        buttonState = true; // Set state to pressed

        __delay_cycles(DEBOUNCE_DELAY_CYCLES); // Wait for a time gap to debounce the button input signal

        if (port & pin)
        {
            buttonState = false; // It turned out the signal was a fluke; reset the state
        }
    }

    return buttonState;
}

//* ----------------------------------------- Interrupts -------------------------------------------*/

// Timer A0 interrupt service routine
// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    timerCount++;
    TACTL &= ~TAIFG; // clear interrupt Flag
}
