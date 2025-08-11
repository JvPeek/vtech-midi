#ifndef MIDI_LOGIC_H
#define MIDI_LOGIC_H

#include <Arduino.h>
#include <queue.h> // For FreeRTOS queue
#include "button_reader.h" // For ButtonEvent struct

// Define a struct for MIDI messages to be sent
typedef struct {
    uint8_t type; // e.g., MIDI_CONTROL_CHANGE
    uint8_t data1; // CC number or Note number
    uint8_t data2; // Value or Velocity
    uint8_t channel; // MIDI channel
} MidiMessage;

// Function to initialize MIDI logic (e.g., bank switch pins)
void initMidiLogic();

// FreeRTOS task for processing button events and bank switches
void midi_logic_task(void *pvParameters);

// External declaration for the MIDI message queue handle
extern QueueHandle_t xMidiMessageQueue;

// External declaration for the current bank variable (shared with button_reader)
extern int currentBank;

#endif // MIDI_LOGIC_H
