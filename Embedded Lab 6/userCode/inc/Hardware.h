#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>

// Enumeration for GPIO pins
typedef enum {
    PIN_0,
    PIN_1,
    PIN_2,
    PIN_3,
    PIN_4,
    PIN_5,
    PIN_6,
    PIN_7,
    PIN_8
} GPIO_PIN;

// Enumeration for LED states
typedef enum {
    LED_OFF,
    LED_ON
} LED_STATE;

// Enumeration for buttons
typedef enum {
    BUTTON_NONE,
    BUTTON_1,
    BUTTON_2
} BUTTON;

// Enumeration for ADC channels
typedef enum {
    CHANNEL_0,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
    CHANNEL_5,
    CHANNEL_6,
    CHANNEL_7
} ADC_Channel;

extern void initHardware();
extern void initLed();
extern void toggleLed();
extern void setLedState(LED_STATE state);
extern void initButtons();
extern BUTTON getPressedButton();
extern void initADC();
extern uint16_t readADC();

#endif // HARDWARE_H
