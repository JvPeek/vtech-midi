#include <Arduino.h>
#include "button_reader.h"
#include "midi_logic.h"
#include "midi_sender.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Define queue lengths and task stack sizes/priorities
#define BUTTON_EVENT_QUEUE_LENGTH 10
#define MIDI_MESSAGE_QUEUE_LENGTH 10
#define BUTTON_READER_TASK_STACK_SIZE 2048
#define MIDI_LOGIC_TASK_STACK_SIZE 4096 // Logic might be heavier
#define MIDI_SENDER_TASK_STACK_SIZE 2048
#define TASK_PRIORITY 1 // All tasks same priority for now

// Global queue handles (declared extern in respective headers)
// Defined in their respective .cpp files

// Global currentBank variable (declared extern in midi_logic.h)
extern int currentBank; 

void setup() {
  // Initialize Serial for debugging (already done in midi_sender.cpp, but good to have here too)
  Serial.begin(115200);
  Serial.println("Starting VTech MIDI Refactor...");

  // Create the queues
  xButtonEventQueue = xQueueCreate(BUTTON_EVENT_QUEUE_LENGTH, sizeof(ButtonEvent));
  xMidiMessageQueue = xQueueCreate(MIDI_MESSAGE_QUEUE_LENGTH, sizeof(MidiMessage));

  if (xButtonEventQueue == NULL || xMidiMessageQueue == NULL) {
    Serial.println("Failed to create FreeRTOS queues. Out of memory?");
    while (1); // Halt if queues cannot be created
  }

  // Initialize modules
  initButtonReader();
  initMidiLogic();
  initMidiSender();

  // Create and launch the FreeRTOS tasks
  xTaskCreate(
    button_reader_task,     // Task function
    "ButtonReader",         // Name of task
    BUTTON_READER_TASK_STACK_SIZE, // Stack size (bytes)
    &currentBank,           // Parameter to pass to the task (pointer to currentBank)
    TASK_PRIORITY,          // Task priority
    NULL                    // Task handle
  );

  xTaskCreate(
    midi_logic_task,        // Task function
    "MidiLogic",            // Name of task
    MIDI_LOGIC_TASK_STACK_SIZE, // Stack size (bytes)
    NULL,                   // Parameter to pass to the task
    TASK_PRIORITY,          // Task priority
    NULL                    // Task handle
  );

  xTaskCreate(
    midi_sender_task,       // Task function
    "MidiSender",           // Name of task
    MIDI_SENDER_TASK_STACK_SIZE, // Stack size (bytes)
    NULL,                   // Parameter to pass to the task
    TASK_PRIORITY,          // Task priority
    NULL                    // Task handle
  );

  Serial.println("FreeRTOS tasks created. Starting scheduler...");
  // The scheduler starts automatically after setup() on ESP-IDF/Arduino
}

void loop() {
  // loop() is now empty as FreeRTOS tasks handle everything
  vTaskDelay(pdMS_TO_TICKS(1)); // Keep the loop alive, yield to other tasks
}