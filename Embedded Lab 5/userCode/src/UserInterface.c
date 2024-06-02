/**
 * @file    Userinterface.c
 * @brief   Functions for user interface interactions.
 *
 * This file contains implementations of functions related to user interface interactions.
 * It includes functions to initialize the user interface components, update note selection
 * based on joystick input, get user-selected tones, and play the selected tones.
 *
 * @date    10.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#include <stdint.h>

#include "../inc/Notes.h"
#include "../inc/Hardware.h"
#include "../inc/NotePlayer.h"
#include "../inc/SerialDisplay.h"

#include "../inc/Userinterface.h"

#define DELAY_ONE_SEC 100000
#define DELAY_HALF_SEC 50000

#define DEBOUNCE_DELAY DELAY_HALF_SEC // Adjust as necessary for your hardware

/**
 * @brief Initializes the user interface components.
 * 
 * This function initializes the note player and joystick hardware components
 * necessary for the user interface.
 */
void initUi()
{
    initNotePlayer();
    initJoystick();
}

/**
 * @brief Updates the current note based on joystick direction.
 * 
 * @param currentNote Pointer to the current note to be updated.
 * @param direction The direction of the joystick (JOYSTICK_LEFT or JOYSTICK_RIGHT).
 * 
 * This function updates the current note based on the direction of the joystick.
 * It changes the note to the next or previous note in the note list and updates
 * the display to reflect the new note.
 */
void updateNoteSelection(NOTE *currentNote, JOYSTICK_DIRECTION direction)
{
    uint16_t i = 0;
    const NOTE *notes = getNotes();
    uint16_t numNotes = getNumberOfNotes();

    // Update the current note based on joystick direction
    if (direction == JOYSTICK_RIGHT)
    {
        // Move to the next note
        for (i = 0; i < numNotes; i++)
        {
            if (notes[i] == *currentNote)
            {
                *currentNote = notes[(i + 1) % numNotes];
                break;
            }
        }
    }
    else if (direction == JOYSTICK_LEFT)
    {
        // Move to the previous note
        for (i = 0; i < numNotes; i++)
        {
            if (notes[i] == *currentNote)
            {
                *currentNote = notes[(i - 1 + numNotes) % numNotes];
                break;
            }
        }
    }

    // Refresh the display with the updated note
    displayNotes(*currentNote);
}

/**
 * @brief Gets user-selected tones using the joystick interface.
 * 
 * @param selectedTones Pointer to an array where the selected tones will be stored.
 * @param maxTones The maximum number of tones that can be selected.
 * 
 * This function allows the user to select tones using the joystick. The selected tones
 * are stored in the provided array. The selection process continues until the user 
 * selects the maximum number of tones specified by maxTones.
 */
void getUserSelectedTones(NOTE *selectedTones, uint16_t maxTones)
{
    NOTE currentNote = NOTE_C;  // Start with the first note
    NOTE previousNote = NOTE_C; // Keep track of the previous note to detect changes
    uint16_t toneIndex = 0;
    uint16_t previousToneIndex = -1; // Track previous tone index to detect changes

    displayToneSelection(0);
    displayNotes(currentNote);

    while (toneIndex < maxTones)
    {
        // Display tone selection only if the tone index has changed
        if (toneIndex != previousToneIndex || currentNote != previousNote)
        {
            clearScreen(); // Clear the screen if there's any change
            displayToneSelection(toneIndex);
            displayNotes(currentNote);
            previousToneIndex = toneIndex;
            previousNote = currentNote;
        }

        JOYSTICK_DIRECTION currentDirection = getJoystickDirection();

        switch (currentDirection)
        {
        case JOYSTICK_LEFT:
        case JOYSTICK_RIGHT:
            __delay_cycles(DEBOUNCE_DELAY); // Debounce delay
            // Check direction again to confirm change
            if (getJoystickDirection() == currentDirection)
            {
                updateNoteSelection(&currentNote, currentDirection);
            }
            break;
        default:
            break;
        }

        if (isJoystickPressed() == JOYSTICK_PRESSED)
        {
            __delay_cycles(DEBOUNCE_DELAY); // Debounce delay
            // Check if joystick is still pressed
            if (isJoystickPressed() == JOYSTICK_PRESSED)
            {
                selectedTones[toneIndex++] = currentNote;
                //playNote(currentNote, 1); // NOT TO shure if the note needs to be played after selection therfor i uncommented it here.
                clearScreen(); // Clear the screen after selecting a tone
                if (toneIndex < maxTones)
                {
                    displayToneSelection(toneIndex);
                    displayNotes(currentNote);
                }
            }
        }

        // Small delay to avoid bouncing issues
        __delay_cycles(DELAY_HALF_SEC);
    }
    __delay_cycles(DELAY_ONE_SEC);
}

/**
 * @brief Plays the user-selected tones.
 * 
 * @param selectedTones Pointer to an array of selected tones to be played.
 * @param numTones The number of tones to play.
 * @param duration The duration each tone should be played for.
 * 
 * This function plays the user-selected tones in sequence. Each tone is played for
 * the specified duration, and the display is updated to show the currently playing tone.
 */
void playUserNotes(NOTE *selectedTones, uint16_t numTones, uint16_t duration)
{
    uint16_t i = 0;
    for (i = 0; i < numTones; i++)
    {
        __delay_cycles(DELAY_ONE_SEC);
        displayPlayingTone(i);
        playNote(selectedTones[i], duration);
    }
}
