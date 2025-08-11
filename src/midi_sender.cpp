#include "midi_sender.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include "midi_logic.h" // For MidiMessage struct
using namespace midi;

// Global MIDI USB object (from main.cpp)
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MidiUsb);

void initMidiSender() {
  TinyUSBDevice.setManufacturerDescriptor("PIO");
  TinyUSBDevice.setProductDescriptor("EspUSB");
  MidiUsb.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200); // Initialize serial for debugging
  Serial.println("Midi USB setup done");
}

void midi_sender_task(void *pvParameters) {
  MidiMessage receivedMsg;
  for (;;) {
    if (xQueueReceive(xMidiMessageQueue, &receivedMsg, portMAX_DELAY) == pdPASS) {
      // Process and send the MIDI message
      switch (receivedMsg.type) {
        case midi::MidiType::ControlChange:
          MidiUsb.sendControlChange(receivedMsg.data1, receivedMsg.data2, receivedMsg.channel);
          break;
        // Add other MIDI message types as needed (NoteOn, NoteOff, etc.)
        // For now, we only handle Control Change as per original main.cpp
        default:
          // Handle unsupported message type or log an error
          Serial.print("Unsupported MIDI message type: ");
          Serial.println(receivedMsg.type);
          break;
      }
    }
  }
}
