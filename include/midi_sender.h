#ifndef MIDI_SENDER_H
#define MIDI_SENDER_H

#include <Arduino.h>
#include <queue.h> // For FreeRTOS queue
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include "midi_logic.h" // For MidiMessage struct

// Function to initialize the USB MIDI interface
void initMidiSender();

// FreeRTOS task for sending MIDI messages
void midi_sender_task(void *pvParameters);

#endif // MIDI_SENDER_H
