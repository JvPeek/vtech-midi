#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <MIDI.h>
#include "midi_config.h" // Include our custom MIDI configuration

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MidiUsb);

bool lastButtonState[NUM_BUTTONS]; // Array to store the previous state of each button
int currentBank = 0; // Global variable to store the current bank

void setup() {
  TinyUSBDevice.setManufacturerDescriptor("PIO");
  TinyUSBDevice.setProductDescriptor("EspUSB");
  MidiUsb.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200); // Initialize serial for debugging
  Serial.println("Midi USB setup done");

  // Initialize button pins and their states
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP); // Use INPUT_PULLUP for active-low buttons
    lastButtonState[i] = digitalRead(buttons[i].pin); // Set initial state
  }

  // Initialize bank switch pins
  pinMode(BANK_SWITCH_PIN_0, INPUT_PULLUP);
  pinMode(BANK_SWITCH_PIN_1, INPUT_PULLUP);
  pinMode(BANK_SWITCH_PIN_2, INPUT_PULLUP);
}

void loop() {
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
    MidiUsb.sendControlChange(BANK_CC_NUMBER, currentBank, 1); // CC for bank, value = currentBank, channel 1
    Serial.print("Sending Bank CC (CC ");
    Serial.print(BANK_CC_NUMBER);
    Serial.print(") with value: ");
    Serial.println(currentBank);
  }

  for (int i = 0; i < NUM_BUTTONS; i++) {
    bool currentButtonState = digitalRead(buttons[i].pin);

    // Only send MIDI message if the button state has changed
    if (currentButtonState != lastButtonState[i]) {
      // Get the CC number for the current bank
      uint8_t cc_to_send = buttons[i].cc_numbers[currentBank];

      if (currentButtonState == LOW) { // Button pressed (active-low)
        MidiUsb.sendControlChange(cc_to_send, 127, 1);
        Serial.print("Button ");
        Serial.print(buttons[i].pin);
        Serial.print(" pressed, sending CC ");
        Serial.print(cc_to_send);
        Serial.print(" (Bank ");
        Serial.print(currentBank);
        Serial.println(")");
      } else { // Button released
        MidiUsb.sendControlChange(cc_to_send, 0, 1);
        Serial.print("Button ");
        Serial.print(buttons[i].pin);
        Serial.print(" released, sending CC ");
        Serial.print(cc_to_send);
        Serial.print(" (Bank ");
        Serial.print(currentBank);
        Serial.println(")");
      }
      lastButtonState[i] = currentButtonState; // Update the last state
    }
  }
  
  delay(1); // Small delay to prevent busy-waiting
}
