#ifndef MIDI_CONFIG_H
#define MIDI_CONFIG_H

#include <Arduino.h>

struct ButtonConfig {
    uint8_t pin;
    uint8_t cc_number;
};

// --- ESP32-S3-DevKitC-1 Usable GPIOs for General Purpose I/O ---
// These pins are generally safe to use for buttons, LEDs, etc., assuming
// you are NOT using Octal SPI flash/PSRAM or specific USB/JTAG functions.
// Always double-check your specific board's documentation.
//
// GPIOs to generally AVOID (or use with extreme caution):
// - GPIO0, GPIO3, GPIO45, GPIO46: Strapping pins (boot mode issues)
// - GPIO19, GPIO20: USB pins
// - GPIO26-32, GPIO35-37: Often used for SPI Flash/PSRAM (if not using PSRAM, some might be usable, but proceed with caution)
// - GPIO39-42: JTAG debugging pins
// - GPIO43, GPIO44: Default UART0 (Serial) TX/RX pins
// - GPIO48: Typically connected to the onboard RGB LED
//
// Generally SAFE GPIOs (assuming no PSRAM/Octal SPI flash):
// GPIO1, GPIO2, GPIO4, GPIO5, GPIO6, GPIO7, GPIO8, GPIO9, GPIO10,
// GPIO11, GPIO12, GPIO13, GPIO14, GPIO15, GPIO16, GPIO17, GPIO18,
// GPIO21, GPIO33, GPIO34, GPIO38, GPIO47

// Define your buttons here
// {PIN_NUMBER, CC_NUMBER},
const ButtonConfig buttons[] = {
    {1, 10},  // Button 1
    {2, 11},  // Button 2
    {4, 12},  // Button 3
    {5, 13},  // Button Square
    {6, 14},  // Button Circle
    {7, 15},  // Button Star
    {8, 16},  // Button Triangle
    {9, 17},  // Button Heart
};

const int NUM_BUTTONS = sizeof(buttons) / sizeof(buttons[0]);

#endif // MIDI_CONFIG_H
