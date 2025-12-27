# Hardware Setup Guide

## 1. PI Display Node (Master)

The **PI Display** node acts as the primary timekeeper and race controller.

### 1.1 Components
*   Raspberry Pi 5
*   ATGM336H GPS Module (Amazon B09LQDG1HY)
*   EBYTE E32 LoRa Module

### 1.2 GPS Module Wiring (ATGM336H)

The ATGM336H must be connected to the Raspberry Pi's GPIO header to provide both serial data (NMEA) and the high-precision PPS timing pulse.

| GPS Pin | Raspberry Pi Pin | Description |
| :--- | :--- | :--- |
| **VCC** | Pin 1 (3.3V) | Power input. (Compatible with 3.3V or 5V, 3.3V is safer for Pi logic) |
| **GND** | Pin 6 (GND) | Ground |
| **TXD** | Pin 10 (GPIO 15 / RXD) | Sends GPS data -> Pi |
| **RXD** | Pin 8 (GPIO 14 / TXD) | Receives commands <- Pi |
| **PPS** | Pin 12 (GPIO 18 / PCM_CLK) | **CRITICAL**: Pulse Per Second for microsecond timing |

> [!NOTE]
> Ensure the Raspberry Pi is powered off while making these connections.

### 1.3 Digit Control Wiring (I2C)

The Display Digits (RP2040s) are connected via the Raspberry Pi's I2C bus.

| Pi Pin | Function | Connect To |
| :--- | :--- | :--- |
| **Pin 3** | **SDA** (GPIO 2) | RP2040 SDA |
| **Pin 5** | **SCL** (GPIO 3) | RP2040 SCL |
| **GND** | Ground | RP2040 GND |

> [!NOTE]
> I2C requires shared ground reference.

### 1.4 PC Link Wiring (LoRa)

Usage: Telemetry and Control to/from the PC Controller.

| LoRa Pin | Pi Pin | Description |
| :--- | :--- | :--- |
| **VCC** | 3.3V or 5V | Power |
| **GND** | GND | Ground |
| **TX** | RXD (GPIO 15) | Radio -> Pi |
| **RX** | TXD (GPIO 14) | Pi -> Radio |
| **M0/M1** | - | Mode Selection (Set to GND/Low for Normal Mode) |
