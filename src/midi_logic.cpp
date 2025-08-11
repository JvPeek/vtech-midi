#include "midi_logic.h"
#include "midi_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <MIDI.h>
using namespace midi;

// Define the queue handle for MIDI messages
QueueHandle_t xMidiMessageQueue;

// Shared variable for the current bank
int currentBank = 0; 

void initMidiLogic() {
  // Initialize bank switch pins
  pinMode(BANK_SWITCH_PIN_0, INPUT_PULLUP);
  pinMode(BANK_SWITCH_PIN_1, INPUT_PULLUP);
  pinMode(BANK_SWITCH_PIN_2, INPUT_PULLUP);
}

void midi_logic_task(void *pvParameters) {
  int lastBank = -1; // Initialize with an invalid bank to force initial update

  for (;;) {
    // Read bank switch pins and determine current bank
    int newBank = 0; // Default to Bank 0 (all pins HIGH)

    if (digitalRead(BANK_SWITCH_PIN_0) == LOW) {
      newBank = 1;
    } else if (digitalRead(BANK_SWITCH_PIN_1) == LOW) {
      newBank = 2;
    } else if (digitalRead(BANK_SWITCH_PIN_2) == LOW) {
      newBank = 3;
    }

    if (newBank != currentBank) {
      currentBank = newBank;
      Serial.print("Bank changed to: ");
      Serial.println(currentBank);
      
      // Send current bank as CC 0
      MidiMessage bankCcMsg;
      bankCcMsg.type = midi::MidiType::ControlChange;
      bankCcMsg.data1 = BANK_CC_NUMBER;
      bankCcMsg.data2 = currentBank;
      bankCcMsg.channel = 1; // Assuming channel 1 for bank changes
      if (xQueueSend(xMidiMessageQueue, &bankCcMsg, portMAX_DELAY) != pdPASS) {
        Serial.println("Failed to send bank CC message to queue");
      }
      Serial.print("Sending Bank CC (CC ");
      Serial.print(BANK_CC_NUMBER);
      Serial.print(") with value: ");
      Serial.println(currentBank);
    }

    ButtonEvent receivedEvent;
    // Check for button events from the queue
    if (xQueueReceive(xButtonEventQueue, &receivedEvent, pdMS_TO_TICKS(10)) == pdPASS) {
      // Find the button configuration for the received pin
      int buttonIndex = -1;
      for (int i = 0; i < NUM_BUTTONS; i++) {
        if (buttons[i].pin == receivedEvent.pin) {
          buttonIndex = i;
          break;
        }
      }

      if (buttonIndex != -1) {
        // Get the CC number for the bank that was active when the button event occurred
        uint8_t cc_to_send = buttons[buttonIndex].cc_numbers[receivedEvent.bank];

        MidiMessage msg;
        msg.type = midi::MidiType::ControlChange;
        msg.data1 = cc_to_send;
        msg.channel = 1; // Assuming channel 1 for button CCs

        if (receivedEvent.state == LOW) { // Button pressed (active-low)
          msg.data2 = 127;
          Serial.print("Button ");
          Serial.print(receivedEvent.pin);
          Serial.print(" pressed, sending CC ");
          Serial.print(cc_to_send);
          Serial.print(" (Bank ");
          Serial.print(receivedEvent.bank);
          Serial.println(")");
        } else { // Button released
          msg.data2 = 0;
          Serial.print("Button ");
          Serial.print(receivedEvent.pin);
          Serial.print(" released, sending CC ");
          Serial.print(cc_to_send);
          Serial.print(" (Bank ");
          Serial.print(receivedEvent.bank);
          Serial.println(")");
        }
        
        if (xQueueSend(xMidiMessageQueue, &msg, portMAX_DELAY) != pdPASS) {
          Serial.println("Failed to send MIDI message to queue");
        }
      }
    }
    // A small delay or yield is good practice if no queue receive timeout is used
    // or if the task is very busy. pdMS_TO_TICKS(10) already provides a delay.
  }
}
