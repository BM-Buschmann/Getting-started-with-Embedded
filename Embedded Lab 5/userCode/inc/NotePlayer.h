/**
 * @file    Notes.h
 * @brief   Header file for the Notes library.
 *
 * This file contains enumeration and function declarations related to musical notes.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#ifndef NOTEPLAYER_H
#define NOTEPLAYER_H

#include <stdint.h>
#include "Notes.h"


/**
 * @brief Initializes the note player.
 * 
 * This function initializes the note player by configuring the necessary hardware
 * and initializing any required modules.
 */
extern void initNotePlayer();

/**
 * @brief Plays a single note for a specified duration.
 * 
 * @param note The note to be played.
 * @param duration The duration for which the note should be played.
 * 
 * This function plays a single note with the specified duration.
 */
extern void playNote(NOTE note, unsigned int duration);

/**
 * @brief Plays a series of notes with corresponding durations.
 * 
 * @param notes Pointer to an array of notes to be played.
 * @param durations Pointer to an array of durations for each note.
 * @param numNotes The number of notes to be played.
 * 
 * This function plays a series of notes with corresponding durations.
 */
extern void playNotes(NOTE *notes, unsigned int *durations, unsigned int numNotes);

#endif /* NOTEPLAYER_H */
