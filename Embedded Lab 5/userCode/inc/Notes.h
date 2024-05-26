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

#ifndef NOTES_H
#define NOTES_H

/**
 * @brief Enumeration for musical notes with their corresponding frequencies in Hz.
 * 
 * This enum defines musical notes with their respective frequencies. The frequencies
 * are represented as integer values.
 */
typedef enum
{
    NOTE_C = 261,  /**< Frequency of note C (261 Hz) */
    NOTE_D = 294,  /**< Frequency of note D (294 Hz) */
    NOTE_E = 329,  /**< Frequency of note E (329 Hz) */
    NOTE_F = 349,  /**< Frequency of note F (349 Hz) */
    NOTE_G = 391,  /**< Frequency of note G (391 Hz) */
    NOTE_A = 440,  /**< Frequency of note A (440 Hz) */
    NOTE_H = 493,  /**< Frequency of note H (493 Hz) */
    NOTE_C2 = 523  /**< Frequency of note C2 (523 Hz) */
} NOTE;

/**
 * @brief Enumeration for tone selection.
 * 
 * This enum defines different tones that can be selected. The NUM_TONES value
 * can be used to count the number of tones available.
 */
typedef enum
{
    TONE_1,     /**< Tone 1 */
    TONE_2,     /**< Tone 2 */
    TONE_3,     /**< Tone 3 */
    TONE_4,     /**< Tone 4 */
    TONE_5,     /**< Tone 5 */
    NUM_TONES   /**< Number of tones available */
} TONE;

/**
 * @brief Returns a pointer to the array of note names.
 * 
 * This function provides access to the array of note names used in the application.
 * 
 * @return A pointer to the array of note names.
 */
extern const char **getNoteNames();

/**
 * @brief Returns a pointer to the array of notes.
 * 
 * This function provides access to the array of notes used in the application.
 * 
 * @return A pointer to the array of notes.
 */
extern const NOTE *getNotes();

/**
 * @brief Returns the number of notes.
 * 
 * This function calculates the number of notes in the notes array.
 * 
 * @return The number of notes.
 */
extern const int getNumberOfNotes();
#endif /* NOTES_H */
