/**
 * @file    NotePlayer.c
 * @brief   Functions for playing musical notes.
 *
 * This file contains implementations of functions related to playing musical notes.
 * It includes functions to initialize the note player, play a single note, and play
 * a sequence of notes.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#include <stdint.h>
#include <msp430.h>

#include "../inc/Notes.h"
#include "../inc/SoftwarePwm.h"

#include "../inc/NotePlayer.h"

void initNotePlayer();
void playNote(NOTE note, unsigned int duration);
void playNotes(NOTE *notes, unsigned int *durations, unsigned int numNotes);

/**
 * @brief Initializes the note player.
 * 
 * This function initializes the note player by configuring the PWM output pin
 * and initializing the software PWM module.
 */
void initNotePlayer(){
    // Set P1.4 as output
    P1DIR |= BIT4;

    // Initialize the PWM on P1.4
    softwarePwmInit(&P1OUT, 4);
};

/**
 * @brief Plays a single note for a specified duration.
 * 
 * @param note The note to be played.
 * @param duration The duration for which the note should be played.
 * 
 * This function plays a single note by setting its frequency and starting the PWM.
 * It then waits for the specified duration before stopping the PWM.
 */
void playNote(NOTE note, unsigned int duration){
    uint16_t i;
    // Set frequency and start PWM
    softwarePwmSetFrequency(note);
    softwarePwmStart();

    // Loop through each note
    for (i = 0; i < duration; i++)
    {
        // Play the note
        __delay_cycles(1000000);
    }
    // Stop PWM
    softwarePwmStop();
}

/**
 * @brief Plays a series of notes with corresponding durations.
 * 
 * @param notes Pointer to an array of notes to be played.
 * @param durations Pointer to an array of durations for each note.
 * @param numNotes The number of notes to be played.
 * 
 * This function plays a series of notes with corresponding durations. It iterates
 * through each note, playing it for the specified duration using the playNote function.
 */
void playNotes(NOTE *notes, unsigned int *durations, unsigned int numNotes)
{
    uint16_t i;

    // Loop through each note
    for (i = 0; i < numNotes; i++)
    {
        // Play the note
        playNote(notes[i], durations[i]);
    }
}
