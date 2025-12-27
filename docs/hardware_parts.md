# LumaChron (Formally Basys 3 Race Clock) - Hardware BOM

## 1. FPGA Development Board
- **[Have] Digilent Basys 3** (Artix-7 FPGA)
    - *Role*: Master Controller, Timing Core.

## 2. Timing & Sync (GPS) - **CRITICAL**
- **1x Digilent Pmod GPS**
    - *Role*: Since LoRa has high latency, we cannot "send" the time over the air accurately. **Each display must have its own GPS** to stay perfectly synchronized (Start/Stop signals sent efficiently, but the "Time" is derived locally).
    - *Interface*: UART.

## 3. Wireless Network (LoRa)
- **1x Digilent Pmod LoRa** (based on Semtech SX1272)
    - *Role*: Long-range command link (Start, Stop, "Change Sequence").
    - *Interface*: SPI (Requires an SPI Controller in FPGA).
- **OR**
- **1x UART LoRa Module** (e.g., Ebyte E32/E22 series) + Pmod Breakout Cable
    - *Role*: Same as above, but much easier to program (simple Serial TX/RX).
    - *Interface*: UART.

## 4. Local Digit Control (FPGA -> Digits)
- **1x Digilent Pmod RS485** (for Local Bus)
    - *Role*: To talk to the RP2040 digits reliably.
- **1x Pmod Splitter/Breakout**
    - For power and signal access.

## 5. Master Station (The Control Hub)
To control the system wirelessly, your PC needs a radio too.
- **1x LoRa USB Dongle** (matching the frequency of your specific Pmod, e.g., 915MHz or 868MHz).
    - *Role*: Plug into laptop to send "Start/Stop" commands to the field of displays.

## Summary Checklist
- [ ] Pmod GPS (One per Display Unit)
- [ ] Pmod LoRa (One per Display Unit + 1 for Master PC)
- [ ] Pmod RS485 (One per Display Unit)
