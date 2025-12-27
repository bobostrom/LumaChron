# LumaChron Raspberry Pi Master Setup

This guide details how to configure a **Raspberry Pi 5** to act as the LumaChron Master Node.

## 1. Hardware Connections

### P1 Header (GPIO)
*   **Pin 1 (3.3V)** -> GPS VCC, LoRa VCC
*   **Pin 6 (GND)** -> GPS GND, LoRa GND
*   **Pin 8 (GPIO 14 / TXD0)** -> LoRa RX
*   **Pin 10 (GPIO 15 / RXD0)** -> LoRa TX
*   **Pin 12 (GPIO 18 / PCM_CLK)** -> GPS PPS (Pulse Per Second)
*   **Pin ?** -> GPS TX is connected to a USB adapter OR a secondary UART (UART3 on GPIO 4/5 if enabled).
    *   *Recommendation*: Use a cheap USB-UART adapter for the GPS NMEA data so it doesn't conflict with the LoRa module on the main UART, or enable UART3.

### Internal RTC
*   Connect a **Panasonic ML-2020** or compatible rechargeable coin cell to the battery connector (J5).

## 2. OS Configuration (Bookworm)

Edit `/boot/firmware/config.txt`:

```ini
# Enable UART0 for LoRa (Disable console)
# Run `sudo raspi-config` -> Interface Options -> Serial Port -> Login Shell=NO, Hardware=YES

# Enable PPS on GPIO 18
dtoverlay=pps-gpio,gpiopin=18

# Enable RTC (Pi 5 loads this automatically, but ensure i2c is on)
dtparam=i2c_arm=on
```

## 3. Software Installation

```bash
# Update System
sudo apt update && sudo apt full-upgrade -y

# Install tools
sudo apt install -y gpsd gpsd-clients python3-serial pps-tools chrony

# Install Python deps
pip3 install pyserial
```

## 4. GPSD Configuration
Edit `/etc/default/gpsd`:
```bash
# Devices (Assuming USB GPS)
DEVICES="/dev/ttyUSB0 /dev/pps0"

# Options -n (Don't wait for client), -b (Read only)
GPSD_OPTIONS="-n -b"
```
