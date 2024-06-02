/**
 * @file    main.c
 * @brief   Main entry point of the program.
 *
 * This file contains the main function, which serves as the entry point of the program.
 * It initializes the microcontroller, peripherals, and enters an infinite loop to read
 * ADC values and process button presses, printing ADC values on the console in different modes.
 *
 * Hardware Description:
 * - Connect the expansion board (CON1) to the experimentation board (CON1) using the provided ribbon cable.
 * On the expansion board:
 *   - Connect CON6:I2C_/SPI to CON2:3V3
 *   - Connect CON6:XSDA to CON2:P1.7
 *   - Connect CON6:XSCL to CON2:P1.6
 * Connect X1:Buzzer to CON3:P1.4 on the main board.
 *
 * How to operate:
 * 1. Ensure the connection between the expansion board and the experimentation board is secure.
 * 2. Connect the necessary components according to the hardware description.
 * 3. Initialize the program by running the main function.
 * 4. Follow the on-screen instructions or serial interface prompts to interact with the program.
 * 5. Use the joystick to select tones or navigate through the menu options.
 * 6. Press the joystick button to confirm selections or trigger specific actions.
 * 7. Follow any additional prompts or instructions displayed on the serial interface for advanced functionalities.
 * 
 * NOTE:
 * the ADC Needs to be read twice as the value returend is the value of the pervious conversion cycle stored in the register therfo to get an up to date reading the adc needs to be read twice.
 *
 * @date    25.05.2024
 * @author  Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */



#include <templateEMP.h>

#include "./userCode/inc/Notes.h"
#include "./userCode/inc/UserInterFace.h"

/**
 * @brief The main function of the program.
 * 
 * This function is the entry point of the program. It initializes the microcontroller
 * and the user interface, then enters an infinite loop to continuously get user-selected
 * tones and play them.
 * 
 * @return This function does not return.
 */
int main(void)
{
    initMSP(); // Initialize microcontroller
    initUi();  // Initialize user interface

    while (1)
    {
        NOTE selectedTones[NUM_TONES];
        getUserSelectedTones(selectedTones, NUM_TONES);
        playUserNotes(selectedTones, NUM_TONES, 1); // Play each note for 1 second
    }
}
