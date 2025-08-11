#ifndef BUTTON_READER_H
#define BUTTON_READER_H

#include <Arduino.h>
#include <queue.h> // For FreeRTOS queue

// Define a struct for button events
typedef struct {
    uint8_t pin;
    bool state; // true for pressed (LOW), false for released (HIGH)
    uint8_t bank; // Current bank when the event occurred
} ButtonEvent;

// Function to initialize button pins
void initButtonReader();

// FreeRTOS task for reading button states
void button_reader_task(void *pvParameters);

// External declaration for the button event queue handle
extern QueueHandle_t xButtonEventQueue;

#endif // BUTTON_READER_H
