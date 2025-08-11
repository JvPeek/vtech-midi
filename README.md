This project implements a MIDI controller using an ESP32-S3, PlatformIO, and the Adafruit TinyUSB library. It features a highly responsive and reliable design, ensuring smooth MIDI performance.

This specific implementation is designed for a **Winnie the Pooh learning laptop MIDI controller**.

## Features

*   **Button-to-MIDI CC Mapping:** Configurable mapping of physical buttons to MIDI Control Change (CC) messages.
*   **Efficient MIDI Output:** Only sends MIDI CC messages when a button's state (pressed/released) actually changes, reducing MIDI bus traffic.
*   **3-Pin Bank Switching:** Supports 4 distinct banks of CC assignments, controlled by three digital input pins.
    *   Bank 0: All three bank switch pins are HIGH (inactive).
    *   Bank 1: First bank switch pin is LOW (active).
    *   Bank 2: Second bank switch pin is LOW (active).
    *   Bank 3: Third bank switch pin is LOW (active).
*   **Configurable Bank CC Output:** Sends the current active bank as a MIDI Control Change message on a configurable CC number (defaulting to CC 0).

## Architecture

This project has been written to utilize a multi-threaded architecture based on FreeRTOS, enhancing responsiveness and maintainability. Key functionalities are now separated into dedicated tasks:

*   **Button Reader Task:** Continuously monitors physical button states and efficiently detects changes.
*   **MIDI Logic Task:** Processes button events and bank switch inputs, determining the appropriate MIDI messages to send.
*   **MIDI Sender Task:** Handles the reliable transmission of MIDI messages over USB.

This modular design ensures that each component operates independently, improving overall system performance and making future development and debugging more straightforward.

## Pinout

The following pins are used in this project. All pins are wired as `INPUT_PULLUP`.

### Button Pins

| Pin | Function | CC Bank 0 | CC Bank 1 | CC Bank 2 | CC Bank 3 |
| :-- | :------- | :-------- | :-------- | :-------- | :-------- |
| 1   | Button 1 | 10        | 20        | 30        | 40        |
| 2   | Button 2 | 11        | 21        | 31        | 41        |
| 4   | Button 3 | 12        | 22        | 32        | 42        |
| 5   | Square   | 13        | 23        | 33        | 43        |
| 6   | Circle   | 14        | 24        | 34        | 44        |
| 7   | Star     | 15        | 25        | 35        | 45        |
| 8   | Triangle | 16        | 26        | 36        | 46        |
| 9   | Heart    | 17        | 27        | 37        | 47        |

### Bank Switch Pins

| Pin | Function                                                              |
| :-- | :-------------------------------------------------------------------- |
| 10  | Bank Switch Pin 0 (LOW = Bank 1)                                      |
| 11  | Bank Switch Pin 1 (LOW = Bank 2)                                      |
| 12  | Bank Switch Pin 2 (LOW = Bank 3)                                      |

*Note: If all bank switch pins are HIGH (inactive), Bank 0 is selected.*

## Getting Started

This solution was tested with `platform = espressif32 @ 6.9.0`.
