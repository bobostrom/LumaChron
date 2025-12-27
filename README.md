# LumaChron

**LumaChron** is a distributed, high-precision race clock system featuring:

*   **Master Node**: Raspberry Pi (Python) for flexible race logic (Start/Stop, Catch-up).
*   **GPS Discipline**: Kernel-level PPS synchronization for microsecond accuracy.
*   **Wireless Network**: Long-range LoRa control for distributed displays.
*   **NeoPixel Display**: 6-digit, 7-segment displays (RP2040-driven) with stateless behavior.

## Architecture

*   **PC Controller** (`pc_controller/`): **Python-based** GUI for centralized race management and remote display control (via LoRa or TCP).
*   **PI Display** (`pi_display/`): **C++ Application** handling the core Race Control logic, GPS/PPS timing, and I2C/LoRa communications.
*   **RP2040 Digit Firmware** (`digit_firmware/`): Firmware for the 7-segment display controllers.

## Time Synchronization Strategy
The system uses a tiered priority for timekeeping to ensure race-grade accuracy:
1.  **GPS (Stratum 0)**: Primary source. Uses PPS (Pulse Per Second) for microsecond-level accuracy.
2.  **NTP (Stratum 1/2)**: Secondary source. Used if GPS is unavailable but network is present.
3.  **RTC (Hardware)**: Fail-safe. Keeps time during power loss; prevents booting to 1970 if GPS/Network are missing.

## Project Structure

```text
LumaChron/
├── pc_controller/     # Wireless Command Station
├── pi_display/        # Master Race Control & Timing Logic
├── digit_firmware/    # RP2040 Display Driver
└── docs/              # Hardware BOM & Protocols
```

## Hardware Specification

### PI Display (Timing Core)
*   **Host**: Raspberry Pi 5
*   **GPS**: ATGM336H GPS Module (PPS connected to GPIO 18)
*   **Display Output**: I2C (Bus 1) to RP2040 Digits
*   **Remote Control**: EBYTE E32 LoRa (UART0/AMA0) to PC Controller
*   **OS**: Raspberry Pi OS (Lite recommended)

### Display Nodes (RP2040)
*   **Controller**: RP2040 (Raspberry Pi Pico)
*   **Input**: I2C (Address 0x20) from PI Display
*   **Visuals**: WS2812B "NeoPixel" driven segments
*   **Behavior**: Stateless (Updates segments based on received I2C commands)

## Build & Run Instructions

### 1. PI Display (C++)
Run these commands on the Raspberry Pi:
```bash
cd pi_display
cmake -S . -B build
cmake --build build
sudo ./build/LumaChronDisplay
```

### 2. PC Controller (Python)
Run on your control PC.
*   **TCP Mode** (Development/WiFi):
    ```bash
    python pc_controller/pyApp.py --tcp <PI_IP_ADDRESS>
    ```
*   **Serial Mode** (Production/LoRa):
    ```bash
    python pc_controller/pyApp.py COM7
    ```
