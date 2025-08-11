#ifndef MIDI_CONFIG_H
#define MIDI_CONFIG_H

#include <Arduino.h>

struct ButtonConfig {
    uint8_t pin;
    uint8_t cc_number;
};

// Define your buttons here
// Example:
// {PIN_NUMBER, CC_NUMBER},
const ButtonConfig buttons[] = {
    {1, 10},  // Example: Button on GPIO 1, sends CC 10
    {2, 11},  // Example: Button on GPIO 2, sends CC 11
    {3, 12},  // Example: Button on GPIO 3, sends CC 12
};

const int NUM_BUTTONS = sizeof(buttons) / sizeof(buttons[0]);

#endif // MIDI_CONFIG_H
