#ifndef MIDI_CONFIG_H
#define MIDI_CONFIG_H

#include <Arduino.h>

// Define the pins for the bank selection switch
// These pins will be read to determine the current bank (0-3)
// All pins HIGH (INPUT_PULLUP, not active) = Bank 0
// BANK_SWITCH_PIN_0 LOW = Bank 1
// BANK_SWITCH_PIN_1 LOW = Bank 2
// BANK_SWITCH_PIN_2 LOW = Bank 3
const uint8_t BANK_SWITCH_PIN_0 = 10;
const uint8_t BANK_SWITCH_PIN_1 = 11;
const uint8_t BANK_SWITCH_PIN_2 = 12;

// Define the CC number for sending the current bank value
const uint8_t BANK_CC_NUMBER = 0; // Default to CC 0

struct ButtonConfig {
    uint8_t pin;
    // Array to hold CC numbers for each of the 4 banks
    // cc_numbers[0] for Bank 0, cc_numbers[1] for Bank 1, etc.
    uint8_t cc_numbers[4];
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
// {BUTTON_PIN, {CC_BANK0, CC_BANK1, CC_BANK2, CC_BANK3}},
const ButtonConfig buttons[] = {
    {1, {10, 20, 30, 40}},  // Button 1
    {2, {11, 21, 31, 41}},  // Button 2
    {4, {12, 22, 32, 42}},  // Button 3
    {5, {13, 23, 33, 43}},  // Button Square
    {6, {14, 24, 34, 44}},  // Button Circle
    {7, {15, 25, 35, 45}},  // Button Star
    {8, {16, 26, 36, 46}},  // Button Triangle
    {9, {17, 27, 37, 47}},  // Button Heart
};

const int NUM_BUTTONS = sizeof(buttons) / sizeof(buttons[0]);

#endif // MIDI_CONFIG_H
