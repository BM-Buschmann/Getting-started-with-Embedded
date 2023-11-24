/**
 * @file    main.c
 * @brief   MSP430 Lab Embedded Programming
 * @details Implementation of a Simon Says game on uC-Labor Versuchsplatine V 2.3 msp430G2553. For further description if the task please read the README folder in the root of this directory.
 *
 * @hardware uC-Labor Versuchsplatine V 2.3 msp430G2553
 * - PB5 connected to CON3:P1.3
 * - PB6 connected to CON3:P1.4
 * - PB7 connected to CON3:P1.5
 * - Red LED (JP8-LED_RT) connected to CON3:P3.0
 * - Green LED (JP8-LED_GN) connected to CON3:P3.1
 * - Blue LED (JP8-LED_BL) connected to CON3:P3.2
 *
 * @task part a 
 * - The alternate function of the pinn is a heating element, aka if the jumper for the transistor is set to heater the signal on the pinn will trigger a transistor to swich to the on state and therfor zurrent will flow through the associated heating resistor.
 * - But in our case the jumper pinn is set to only toggel the led as we dont need the heater functionality.
 * 
 * @author  Bjï¿½rn Metzger, Melvin Willburger
 * @date    2023-11-16
 * @version 3.0
 */

//* ---------------------------------------- Includes ---------------------------------------------*/

// Toolchain includes
#include <msp430.h>
#include <templateEMP.h>

// Standard includes
#include <stdint.h>
#include <stdbool.h>

//* ----------------------------------------- Defines ---------------------------------------------*/

#define LED_RED (BIT0)
#define LED_GRN (BIT1)
#define LED_BLU (BIT2)

#define BUTTON_ONE (BIT3)
#define BUTTON_TWO (BIT4)
#define BUTTON_THREE (BIT5)

#define PORT_ONE (P1IN)
#define PORT_THREE (P3OUT)

#define SEQUENCE_COUNT ((uint8_t)5)
#define NUMBER_OF_LEDS ((uint8_t)3)
#define DEBOUNCE_DELAY_CYCLES ((uint16_t)20000)

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
    uint8_t u8_LedOrder[NUMBER_OF_LEDS];
    uint16_t u8_OnTime;
    uint16_t u8_OffTime;
} ledSequence;

//* -------------------------------------- Variable Declarations ------------------------------------------*/

// LED sequences
ledSequence sequences[SEQUENCE_COUNT] = {
    {{LED_RED, LED_GRN, LED_BLU}, (uint16_t)2000, (uint16_t)2000},
    {{LED_GRN, LED_BLU, LED_RED}, (uint16_t)1000, (uint16_t)1000},
    {{LED_BLU, LED_RED, LED_GRN}, (uint16_t)500, (uint16_t)500},
    {{LED_RED, LED_BLU, LED_GRN}, (uint16_t)300, (uint16_t)300},
    {{LED_GRN, LED_RED, LED_BLU}, (uint16_t)100, (uint16_t)100}};

// User input array and control variables
uint8_t u8_LedOrderUserInput[NUMBER_OF_LEDS];
uint8_t u8_CurrentSequenceIndex = ((uint8_t)0);
uint8_t u8_CurrentExecutionState = stateNull;

// Timer-related variables
volatile uint16_t u16_TimerCount = ((uint16_t)0);
volatile uint8_t u16_LedIndex = ((uint16_t)0);

//* -------------------------------------- Method Declarations ------------------------------------------*/

// Initialization method
void initialize(void);

// Animation methods
void playStartAnimation(void);
void playCurrentSequenceAnimation(ledSequence *p_CurrentSequence);
void playRoundWonAnimation(void);  // renamed the roundwon() method to playRoundWonAnimation() to fit the naming sheme
void playRoundLostAnimation(void); // renamed the roundlost() method to playRoundLostAnimation() to fit the naming sheme
void playGameWonAnimation(void);   // renamed the  gamewon() method to playGameWonAnimation() to fit the naming sheme

// Timer control methods
void startTimer(void);
void stopTimer(void);

// User input methods
void collectUserInput(uint8_t *p_LedOrderUserInput);

// Helper methods
bool compareArrays(uint8_t *p_Array1, uint8_t *p_Array2);
bool debounceInput(uint8_t u8_Port, uint8_t u8_Pin);

//* ---------------------------------------- Definitions ------------------------------------------*/
int main(void)
{
    bool b_Result = false;

    initialize();

    while (1)
    {
        switch (u8_CurrentExecutionState)
        {
        case stateNull: // Play starting animation
            if (debounceInput(PORT_ONE, BUTTON_ONE) || debounceInput(PORT_ONE, BUTTON_TWO) || debounceInput(PORT_ONE, BUTTON_THREE))
            {
                playStartAnimation();
                u8_CurrentExecutionState = stateOne; // Move to the next execution state
            }
            break;

        case stateOne: // Play LED sequence for the user
            playCurrentSequenceAnimation(&sequences[u8_CurrentSequenceIndex]);
            u8_CurrentExecutionState = stateTwo;
            break;

        case stateTwo: // Collect user input
            collectUserInput(&u8_LedOrderUserInput);
            u8_CurrentExecutionState = stateThree;
            break;

        case stateThree: // Compare user input and played sequence

            b_Result = compareArrays(&sequences[u8_CurrentSequenceIndex].u8_LedOrder, &u8_LedOrderUserInput);

            if (b_Result && u8_CurrentSequenceIndex == ((uint8_t)5)) // User input identical to displayed sequence for the last round
            {
                playGameWonAnimation();
                playStartAnimation();
                u8_CurrentSequenceIndex = ((uint8_t)0);
                u8_CurrentExecutionState = stateNull;
            }
            else if (b_Result) // User input identical to displayed sequence
            {
                playRoundWonAnimation();
                u8_CurrentSequenceIndex++; // Increment the current sequence index
                u8_CurrentExecutionState = stateOne;
            }
            else // User input not identical to displayed sequence
            {
                playRoundLostAnimation();
                u8_CurrentSequenceIndex = ((uint8_t)0); // Reset the current sequence index
                u8_CurrentExecutionState = stateNull;
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
    TA0CCR0 = ((uint8_t)1000);          // Set CCR0 to produce 100 ms per count
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
    __delay_cycles((uint32_t)2000000);            // Delay for 2 seconds
    PORT_THREE &= ~(LED_RED + LED_GRN + LED_BLU); // Turn off all LEDs
}

/**
 * @brief Plays the animation based on the provided LED sequence.
 *
 * This function plays the animation based on the LED sequence provided.
 * It toggles LEDs according to the sequence's on and off times using a timer.
 *
 * @param p_CurrentSequence Pointer to the LED sequence structure.
 */
void playCurrentSequenceAnimation(ledSequence *p_CurrentSequence)
{
    startTimer(); // Start the timer for animation

    uint8_t u8_CurentLedIndex = 0; // Declare u8_CurentLedIndex within the method

    while (u8_CurentLedIndex < NUMBER_OF_LEDS)
    {
        uint8_t u8_CurrentLedPin = p_CurrentSequence->u8_LedOrder[u8_CurentLedIndex];
        uint16_t u16_CurrentPeriodTime = p_CurrentSequence->u8_OffTime + p_CurrentSequence->u8_OnTime;

        if (u16_TimerCount >= u16_CurrentPeriodTime)
        {
            u16_TimerCount = 0;
            u8_CurentLedIndex++;
        }
        else
        {
            // If the current time is within the "on" time and the LEDs are off, turn them on
            if ((u16_TimerCount <= p_CurrentSequence->u8_OnTime) && !(PORT_THREE & u8_CurrentLedPin))
            {
                PORT_THREE |= u8_CurrentLedPin; // Turn on all LEDs
            }
            // If the current time is greater than 250 and the LEDs are on, turn them off
            else if ((u16_TimerCount > p_CurrentSequence->u8_OnTime) && (PORT_THREE & u8_CurrentLedPin))
            {
                PORT_THREE &= ~u8_CurrentLedPin;
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

    uint8_t u8_CurentLedIndex = ((uint8_t)0);

    uint16_t u16_CurrentPeriodTime = ((uint16_t)500); // 500ms for each state (on and off)

    while (u8_CurentLedIndex < ((uint8_t)4)) // Repeat the sequence four times
    {
        if (u16_TimerCount >= u16_CurrentPeriodTime)
        {
            u16_TimerCount = ((uint8_t)0);
            u8_CurentLedIndex++;
        }
        else
        {
            // If the current time is within the "on" time and the LEDs are off, turn them on
            if ((u16_TimerCount <= ((uint8_t)250)) && !(PORT_THREE & (LED_RED + LED_GRN + LED_BLU)))
            {
                PORT_THREE |= (LED_RED + LED_GRN + LED_BLU); // Turn on all LEDs
            }
            // If the current time is greater than 250 and the LEDs are on, turn them off
            else if ((u16_TimerCount > ((uint8_t)250)) && (PORT_THREE & (LED_RED + LED_GRN + LED_BLU)))
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

    uint8_t u8_CurentLedIndex = 0;

    while (u8_CurentLedIndex < ((uint8_t)3)) // Repeat the sequence three times
    {
        uint16_t u16_CurrentPeriodTime = ((uint16_t)1000); // 1000ms for each cycle

        if (u16_TimerCount >= u16_CurrentPeriodTime) // If the loop reached the desired period rool over to 0 and start again
        {
            u16_TimerCount = ((uint16_t)0);
            u8_CurentLedIndex++;
        }
        else
        {
            // If the current time is within the "on" time and the LEDs are off, turn them on
            if ((u16_TimerCount <= ((uint16_t)333)))
            {
                PORT_THREE |= LED_BLU; // Turn on all LEDs
                PORT_THREE &= ~(LED_RED + LED_GRN);
            }
            // If the current time is greater than 333 and less than or equal to 666, turn on the red LED
            else if ((u16_TimerCount > ((uint16_t)333)) && (u16_TimerCount <= ((uint16_t)666)))
            {
                PORT_THREE |= LED_RED; // Turn on the red LED
                PORT_THREE &= ~(LED_GRN + LED_BLU);
            }
            // If the current time is greater than 666, turn on the green LED
            else if ((u16_TimerCount > ((uint16_t)666)))
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

    uint8_t u8_CurentLedIndex = 0;

    while (u8_CurentLedIndex < ((uint8_t)3)) // Repeat the sequence three times
    {
        uint16_t u16_CurrentPeriodTime = ((uint16_t)1000); // 1000ms for each cycle

        if (u16_TimerCount >= u16_CurrentPeriodTime)
        {
            u16_TimerCount = ((uint16_t)0);
            u8_CurentLedIndex++;
        }
        else
        {
            // If the current time is within the "on" time and the LEDs are off, turn them on
            if ((u16_TimerCount <= ((uint16_t)333)))
            {
                PORT_THREE |= LED_BLU; // Turn on all LEDs
                PORT_THREE &= ~(LED_GRN + LED_RED);
            }
            // If the current time is greater than 333 and less than or equal to 666, turn on the green LED
            else if ((u16_TimerCount > ((uint16_t)333)) && (u16_TimerCount <= ((uint16_t)666)))
            {
                PORT_THREE |= LED_GRN; // Turn on the green LED
                PORT_THREE &= ~(LED_RED + LED_BLU);
            }
            // If the current time is greater than 666, turn on the red LED
            else if ((u16_TimerCount > ((uint16_t)666)))
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
 * @param p_LedOrderUserInput Pointer to the array where the user input will be stored.
 */
void collectUserInput(uint8_t *p_LedOrderUserInput)
{
    uint8_t u8_CurrentIndex = ((uint8_t)0);

    // Continue polling buttons until all three buttons are pressed
    while (u8_CurrentIndex < NUMBER_OF_LEDS)
    {
        if (debounceInput(PORT_ONE, BUTTON_ONE))
        {
            // Button one is pressed, store it in the array
            p_LedOrderUserInput[u8_CurrentIndex] = LED_RED;
            u8_CurrentIndex++;
            while (debounceInput(PORT_ONE, BUTTON_ONE))
            {
                // Wait for button release to avoid multiple presses being registered
            }
        }
        else if (debounceInput(PORT_ONE, BUTTON_TWO))
        {
            // Button two is pressed, store it in the array
            p_LedOrderUserInput[u8_CurrentIndex] = LED_GRN;
            u8_CurrentIndex++;
            while (debounceInput(PORT_ONE, BUTTON_TWO))
            {
                // Wait for button release to avoid multiple presses being registered
            }
        }
        else if (debounceInput(PORT_ONE, BUTTON_THREE))
        {
            // Button three is pressed, store it in the array
            p_LedOrderUserInput[u8_CurrentIndex] = LED_BLU;
            u8_CurrentIndex++;
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
 * @param p_Array1 Pointer to the first array.
 * @param p_Array2 Pointer to the second array.
 * @return true if the arrays are identical, false otherwise.
 */
bool compareArrays(uint8_t *p_Array1, uint8_t *p_Array2)
{
    uint8_t u8_ArrayIndex = ((uint8_t)0);

    for (u8_ArrayIndex = NUMBER_OF_LEDS; u8_ArrayIndex > ((uint8_t)0); u8_ArrayIndex--)
    {
        if (p_Array1[u8_ArrayIndex] != p_Array2[u8_ArrayIndex])
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
 * @param u8_Port Port register of the button pin.
 * @param u8_Pin Pin number of the button.
 * @return 1 if the button is pressed after debouncing, 0 otherwise.
 */
bool debounceInput(uint8_t u8_Port, uint8_t u8_Pin)
{
    bool b_ButtonState = false; // Default to not pressed

    if (!(u8_Port & u8_Pin))
    {
        b_ButtonState = true; // Set state to pressed

        __delay_cycles(DEBOUNCE_DELAY_CYCLES); // Wait for a time gap to debounce the button input signal

        if (u8_Port & u8_Pin)
        {
            b_ButtonState = false; // It turned out the signal was a fluke; reset the state
        }
    }

    return b_ButtonState;
}

//* ----------------------------------------- Interrupts -------------------------------------------*/

// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    CCTL0 &= ~CCIE; // Disable interrupt vectors for CCR0
    u16_TimerCount++;
    CCTL0 |= CCIE; // Enable interrupt vectors for CCR0

    TACTL &= ~TAIFG; // clear interrupt Flag
}
