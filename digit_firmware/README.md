# Digit Firmware (RP2040)

**Target Board**: RP2040 (e.g., Raspberry Pi Pico, Seeed Xiao RP2040)
**Language**: Rust (Embassy or rp-hal)

## Features
- Controls 7-segment display via NeoPixels.
- Listens for RS485 commands from Master.
- "Catch-Up" timing logic implemented locally? (TBD)

## Setup
1. Install Rust `rustup`.
2. Install target: `rustup target add thumbv6m-none-eabi`.
3. Install flip-link: `cargo install flip-link`.
