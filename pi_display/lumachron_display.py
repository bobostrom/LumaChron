#!/usr/bin/env python3
"""
LumaChron PI Display Node
Runs on Raspberry Pi 5.

TIME SYNCHRONIZATION:
This script assumes the underlying Operating System is disciplined by a high-precision source:
1. GPS w/ PPS (Stratum 0) - Primary
2. NTP (Stratum 1) - Fallback
3. RTC - Hardware Backup

ARCHITECTURE:
- PI Display (This Script): The "Master" clock.
- Digits (RP2040): Connected via I2C (Address 0x20).
- PC Controller: Connected via LoRa (Serial) for remote management.
"""

import time
import serial
import logging
import struct

try:
    from smbus2 import SMBus
except ImportError:
    SMBus = None
    logging.warning("smbus2 not installed. I2C will be simulated.")

# Configuration
I2C_BUS_ID = 1          # Raspberry Pi default I2C bus
DIGIT_I2C_ADDR = 0x20   # Default address for RP2040 Digits

LORA_PORT = "/dev/ttyAMA0" # Primary UART for PC Link
LORA_BAUD = 9600

# Protocol Constants
CMD_UPDATE_TIME = 0x10

class I2CDigitTransmitter:
    """Handles communication with the 7-segment displays via I2C."""
    
    def __init__(self, bus_id, address):
        self.bus_id = bus_id
        self.address = address
        self.bus = None
        self.connect()

    def connect(self):
        if SMBus:
            try:
                self.bus = SMBus(self.bus_id)
                logging.info(f"Connected to I2C Bus {self.bus_id}")
            except Exception as e:
                logging.error(f"Failed to connect to I2C Bus: {e}")
                self.bus = None
        else:
            logging.info("I2C in SIMULATION MODE")

    def send_update(self, hour, minute, second):
        """
        Sends time update to the digits via I2C.
        Payload: [H, M, S]
        """
        if not self.bus:
            # Simulation
            # logging.debug(f"I2C TX: {hour:02}:{minute:02}:{second:02}")
            return

        try:
            # Write block data: Cmd, [Payload...]
            # Using write_i2c_block_data(addr, cmd, [data])
            # We treat CMD_UPDATE_TIME as the register/cmd byte
            self.bus.write_i2c_block_data(self.address, CMD_UPDATE_TIME, [hour, minute, second])
        except Exception as e:
            logging.error(f"I2C TX Error: {e}")

class LoRaUplink:
    """Handles upstream communication with the PC Controller."""
    def __init__(self, port, baud):
        self.port = port
        self.baud = baud
        self.ser = None
        # self.connect() # Disabled for now until hardware is confirmed

    def connect(self):
        try:
            self.ser = serial.Serial(self.port, self.baud, timeout=0.1)
            logging.info(f"Connected to Upstream LoRa on {self.port}")
        except Exception as e:
            logging.error(f"Failed to connect to Upstream LoRa: {e}")

def main():
    logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
    logging.info("LumaChron PI Display Node Starting...")
    
    # 1. Initialize Digits (I2C)
    digits = I2CDigitTransmitter(I2C_BUS_ID, DIGIT_I2C_ADDR)
    
    # 2. Initialize Upstream Link (LoRa)
    uplink = LoRaUplink(LORA_PORT, LORA_BAUD)
    
    # Align to the top of the second
    now = time.time()
    next_sec = int(now) + 1
    sleep_dur = next_sec - now
    if sleep_dur > 0:
        time.sleep(sleep_dur)

    try:
        while True:
            # 1. Get Precise System Time
            t_now = time.time()
            t_struct = time.localtime(t_now)
            
            # 2. Update Digits (I2C)
            digits.send_update(
                t_struct.tm_hour, 
                t_struct.tm_min, 
                t_struct.tm_sec
            )
            
            # 3. Handle Upstream (Future: Check self.uplink.ser.in_waiting)
            
            # 4. Wait for next second
            now = time.time()
            next_edge = int(now) + 1
            delay = next_edge - now
            if delay > 0:
               time.sleep(delay)

    except KeyboardInterrupt:
        logging.info("Stopping PI Display Node...")

if __name__ == "__main__":
    main()
