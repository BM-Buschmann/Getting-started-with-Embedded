/**
 * @file    Notes.c
 * @brief   Definitions of note-related data.
 *
 * This file contains definitions of note-related data, such as note names and note values.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#include "../inc/Notes.h"

// Array of note names
const char *noteNames[] = {"C", "D", "E", "F", "G", "A", "H", "C2"};

// Array of notes
const NOTE notes[] = {NOTE_C, NOTE_D, NOTE_E, NOTE_F, NOTE_G, NOTE_A, NOTE_H, NOTE_C2};

const char **getNoteNames();
const NOTE *getNotes();
const int getNumberOfNotes();

/**
 * @brief Returns a pointer to the array of note names.
 * 
 * This function provides access to the array of note names used in the application.
 * 
 * @return A pointer to the array of note names.
 */
const char **getNoteNames() {
    return noteNames;
}

/**
 * @brief Returns a pointer to the array of notes.
 * 
 * This function provides access to the array of notes used in the application.
 * 
 * @return A pointer to the array of notes.
 */
const NOTE *getNotes() {
    return notes;
}

/**
 * @brief Returns the number of notes.
 * 
 * This function calculates the number of notes in the notes array.
 * 
 * @return The number of notes.
 */
const int getNumberOfNotes(){
    return sizeof(notes) / sizeof(notes[0]);
}
