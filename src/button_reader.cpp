#include "button_reader.h"
#include "midi_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Define the queue handle for button events
QueueHandle_t xButtonEventQueue;

bool lastButtonState[NUM_BUTTONS]; // Array to store the previous state of each button

void initButtonReader() {
  // Initialize button pins and their states
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP); // Use INPUT_PULLUP for active-low buttons
    lastButtonState[i] = digitalRead(buttons[i].pin); // Set initial state
  }
}

void button_reader_task(void *pvParameters) {
  // The currentBank variable is passed as a parameter to the task
  // This allows the button reader to know the current bank when a button event occurs
  int *currentBankPtr = (int *)pvParameters;
  int currentBank = *currentBankPtr;

  for (;;) { // Infinite loop for the FreeRTOS task
    // Update currentBank from the shared variable (or a more robust IPC if needed)
    // For simplicity, we'll assume currentBankPtr is updated by midi_logic_task
    // and this task reads it. A mutex or semaphore might be needed for true robustness.
    currentBank = *currentBankPtr;

    for (int i = 0; i < NUM_BUTTONS; i++) {
      bool currentButtonState = digitalRead(buttons[i].pin);

      // Only send MIDI message if the button state has changed
      if (currentButtonState != lastButtonState[i]) {
        ButtonEvent event;
        event.pin = buttons[i].pin;
        event.state = currentButtonState; // true for pressed (LOW), false for released (HIGH)
        event.bank = currentBank; // Capture the bank at the time of the event

        // Send the event to the queue
        if (xQueueSend(xButtonEventQueue, &event, portMAX_DELAY) != pdPASS) {
          // Failed to send to queue, handle error (e.g., print to Serial)
          Serial.println("Failed to send button event to queue");
        }
        lastButtonState[i] = currentButtonState; // Update the last state
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1)); // Small delay to prevent busy-waiting
  }
}
