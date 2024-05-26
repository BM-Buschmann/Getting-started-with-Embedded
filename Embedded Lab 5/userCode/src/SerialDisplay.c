/**
 * @file    SerialDisplay.c
 * @brief   Functions for serial display operations.
 *
 * This file contains implementations of functions related to serial display operations.
 * It includes functions to clear the terminal screen, display notes, tone selection, and playing tone.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */


#include <stdio.h>
#include <stdint.h>

#include "../inc/Notes.h"

#include "../inc/SerialDisplay.h"

void displayNotes(NOTE currentNote);
void displayToneSelection(TONE currentTone);
void displayPlayingTone(TONE currentTone);
void clearScreen();

/**
 * @brief Clears the terminal screen.
 * 
 * This function sends an escape sequence to the terminal to clear the screen
 * and move the cursor to the home position.
 */
void clearScreen()
{
    serialPrintln("\033[2J\033[H");
}

/**
 * @brief Displays the notes with the currently selected note indicated by a '#'.
 * 
 * @param currentNote The currently selected note to be indicated.
 * 
 * This function retrieves the list of notes and their names, and then constructs
 * a string where the currently selected note is indicated by a '#'. The string
 * is then printed to the terminal.
 */
void displayNotes(NOTE currentNote)
{
    uint16_t i = 0;
    const char **noteNames = getNoteNames();
    const NOTE *notes = getNotes();
    uint16_t numNotes = getNumberOfNotes();

    char buffer[128] = ""; // Increase size if necessary
    char temp[16];

    for (i = 0; i < numNotes; i++)
    {
        if (notes[i] == currentNote)
        {
            snprintf(temp, sizeof(temp), "# ");
        }
        else
        {
            snprintf(temp, sizeof(temp), "%s ", noteNames[i]);
        }
        strncat(buffer, temp, sizeof(buffer) - strlen(buffer) - 1);
    }

    serialPrintln(buffer);
}

/**
 * @brief Displays the currently selected tone.
 * 
 * @param currentTone The currently selected tone.
 * 
 * This function prints a message to the terminal indicating the currently
 * selected tone number.
 */
void displayToneSelection(TONE currentTone)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Auswahl von Ton # %d", currentTone + 1);
    serialPrintln(buffer);
}

/**
 * @brief Displays a message indicating the playing note.
 * 
 * @param currentTone The tone currently being played.
 * 
 * This function prints a message to the terminal indicating the note number
 * currently being played.
 */
void displayPlayingTone(TONE currentTone)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Playing Note # %d", currentTone + 1);
    serialPrintln(buffer);
}
