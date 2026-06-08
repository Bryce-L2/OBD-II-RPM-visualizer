# OBD-II RPM LED Visualizer

An Arduino project that reads **live engine RPM** from a car's OBD-II port and maps it onto a NeoPixel LED strip in real time. The LEDs climb **purple → pink → white** as RPM rises, then the whole strip strobes white at redline.

Built and tested on a 2017 Ford Fusion Energi, but works on any OBD-II vehicle (1996+).

## How it works

| Layer | What it does |
| --- | --- |
| Communication | Freematics adapter pulls live RPM from the car's ECU and sends it to the Arduino over hardware serial |
| Physics | A bounce algorithm makes the LED level rise fast and fall naturally for a realistic feel |
| Output | RPM is mapped to a NeoPixel color gradient, with a strobe effect at redline |

## Parts

- Arduino Uno R3
- Freematics ELM327 OBD-II UART adapter
- WS2812B NeoPixel LED strip (144 LED/m)
- Buck converter (12V → 5V) + fuse tap for power

## Tech

C++ / Arduino · Adafruit NeoPixel · OBD-II (PID 0x0C / RPM) · Hardware Serial

## Setup

1. Install the **Adafruit NeoPixel** library in the Arduino IDE.
2. Wire the adapter to the Arduino's hardware serial pins **D0 (RX)** and **D1 (TX)**.
3. **Unplug D0 and D1 before uploading the sketch**, then reconnect them after — they share the USB upload line.
4. Upload, plug into the OBD-II port, and start the car.

## Behavior

- Idle cutoff: ~600 RPM
- Color gradient: purple → pink → white as RPM climbs
- Redline warning strobe: 7000 RPM
- Max scale: 9000 RPM

## Notes

The adapter is powered by the OBD-II port itself, so its **VCC must stay disconnected** from the Arduino's 5V — wiring it in corrupts the serial data. Tracking that down was the hardest part of the build.

---

**Bryce Lombardo** · [@Bryce-L2](https://github.com/Bryce-L2)
