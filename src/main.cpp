#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <MIDI.h>

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MidiUsb);

void setup() {
  TinyUSBDevice.setManufacturerDescriptor("PIO");
  TinyUSBDevice.setProductDescriptor("EspUSB");
  MidiUsb.begin(MIDI_CHANNEL_OMNI);
  Serial.println("Midi USB setup done");
}

void loop() {
  for (int i = 0; i < 128; i++) {
    MidiUsb.sendControlChange(0, i, 1);
    delay(100);
  }
}
