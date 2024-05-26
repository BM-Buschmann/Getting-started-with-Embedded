/**
 * @file    Userinterface.h
 * @brief   Header file for user interface functions.
 *
 * This file contains function declarations for user interface interactions.
 * It includes functions to initialize the user interface components, get user-selected tones,
 * and play the selected tones.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <stdint.h>


/**
 * @brief Initializes the user interface components.
 * 
 * This function initializes the note player and joystick hardware components
 * necessary for the user interface.
 */
extern void initUi();

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
extern void getUserSelectedTones(NOTE *selectedTones, uint16_t maxTones);

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
extern void playUserNotes(NOTE *selectedTones, uint16_t numTones, uint16_t duration);

#endif /* USERINTERFACE_H */
