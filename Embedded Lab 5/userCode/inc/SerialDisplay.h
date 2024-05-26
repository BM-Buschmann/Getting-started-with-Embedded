/**
 * @file    SerialDisplay.h
 * @brief   Header file for the Serial Display library.
 *
 * This file contains function declarations for the Serial Display library.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#ifndef SERIALDISPLAY_H
#define SERIALDISPLAY_H

/**
 * @brief Displays the notes with the currently selected note indicated by a '#'.
 * 
 * @param currentNote The currently selected note to be indicated.
 * 
 * This function retrieves the list of notes and their names, and then constructs
 * a string where the currently selected note is indicated by a '#'. The string
 * is then printed to the terminal.
 */
extern void displayNotes(NOTE currentNote);

/**
 * @brief Displays the currently selected tone.
 * 
 * @param currentTone The currently selected tone.
 * 
 * This function prints a message to the terminal indicating the currently
 * selected tone number.
 */
extern void displayToneSelection(TONE currentTone);

/**
 * @brief Displays a message indicating the playing note.
 * 
 * @param currentTone The tone currently being played.
 * 
 * This function prints a message to the terminal indicating the note number
 * currently being played.
 */
extern void displayPlayingTone(TONE currentTone);

/**
 * @brief Clears the terminal screen.
 * 
 * This function sends an escape sequence to the terminal to clear the screen
 * and move the cursor to the home position.
 */
extern void clearScreen();

#endif /* USERINTERFACE_H */
