# Race Clock PC Controller

**Language**: Python
**Dependencies**: `pyserial`, `tkinter` or `PyQt` (GUI)

## Role
- Connects to the "Master Radio" (LoRa USB Dongle).
- Sends high-level "Gun Start", "Recall", "Configure" commands to the system.
- Does NOT handle real-time sequencing (that's the FPGA's job). This is just the Remote Control.
