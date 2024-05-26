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
