#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <MIDI.h>
#include "midi_config.h" // Include our custom MIDI configuration

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MidiUsb);

bool lastButtonState[NUM_BUTTONS]; // Array to store the previous state of each button



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
}

void loop() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    bool currentButtonState = digitalRead(buttons[i].pin);

    // Only send MIDI message if the button state has changed
    if (currentButtonState != lastButtonState[i]) {
      if (currentButtonState == LOW) { // Button pressed (active-low)
        MidiUsb.sendControlChange(buttons[i].cc_number, 127, 1);
        Serial.print("Button ");
        Serial.print(buttons[i].pin);
        Serial.print(" pressed, sending CC ");
        Serial.println(buttons[i].cc_number);
      } else { // Button released
        MidiUsb.sendControlChange(buttons[i].cc_number, 0, 1);
        Serial.print("Button ");
        Serial.print(buttons[i].pin);
        Serial.print(" released, sending CC ");
        Serial.println(buttons[i].cc_number);
      }
      lastButtonState[i] = currentButtonState; // Update the last state
    }
  }
  
  delay(1); // Small delay to prevent busy-waiting
}
