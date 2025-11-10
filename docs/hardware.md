# Hardware Reference

This document describes the hardware required and wiring for the **Smart Emergency Alert System (ESP32 + Blynk, fixed coords)**.

> Note: this design uses fixed coordinates stored in firmware. No GPS module is required.

---

## Bill of Materials (BOM)

| Qty | Part | Purpose / Notes |
|-----:|------|------------------|
| 1 | ESP32 development board (ESP32-WROOM-32 or similar) | Main controller, 3.3V IO |
| 1 | Flame sensor module (digital output) | e.g., YL-38 / YL-39 — use digital output for simple detection |
| 1 | Gas sensor module (MQ-2 or MQ-135) | Gas detection; analog output can be read via ADC for better sensitivity |
| 1 | Push button (tactile) | Manual SOS trigger |
| 1 | 5V → 3.3V regulator or USB power | Stable 3.3V supply capable of board + sensors |
| 1 | 1 kΩ resistor (optional) | Pull-down or series for button debouncing (see notes) |
| 1 | 10 kΩ resistor (optional) | Pull-up or pull-down for button if not using internal MCU pull-ups |
| 1 | Buzzer or LED (optional) | Local alarm/visual indicator |
| Jumper wires, breadboard or PCB, headers | Interconnects and mounting |

---

## Pin assignment (recommended)

| Signal | ESP32 GPIO | Type | Notes |
|--------|------------:|------|-------|
| Flame sensor (digital) | GPIO33 | Digital input | Example: flame module digital output; often active LOW (check module docs) |
| Gas sensor (digital or analog) | GPIO32 (or ADC1_CH4) | Digital input or ADC | For analog MQ output use an ADC-capable pin (ADC1 channels recommended) |
| SOS push button | GPIO25 | Digital input | Use proper pull-down/up (see wiring notes) |
| 3.3V | 3.3V | Power | Common supply for sensors (do not exceed 3.3V on GPIO) |
| GND | GND | Ground | Connect all grounds together |

---

## Wiring notes

### Flame sensor
- Many flame sensor modules provide both digital and analog outputs.
- Use the **digital output** for a simple presence/absence alert; it often goes **LOW** when flame is detected (active LOW). Verify with your module.
- If the module needs 5V supply, use proper level shifting for the digital output (or power sensors from 3.3V if compatible).

### Gas sensor (MQ-series)
- MQ sensors typically require a warm-up time (→ 24–48 hours for stable baseline; initial testing may take minutes).
- Modules often include an onboard comparator that provides a digital output. For best sensitivity use the **analog output** and read it via an ADC pin.
- Recommended: use an **ADC1** channel (e.g., GPIO32 → ADC1_CH4) because ADC2 is shared with Wi-Fi and can conflict when Wi-Fi is active on some ESP32 chips.

### SOS push button
- Wiring options:
  - Pull-down: Button connects 3.3V → GPIO when pressed; configure GPIO with pull-down resistor.
  - Pull-up: Button connects GPIO → GND when pressed; configure GPIO with pull-up and invert logic.
- The ESP32 has software-configurable internal pull-ups/pull-downs; for reliability on noisy environments, add an external 10 kΩ pull resistor.
- Use debouncing in firmware (the provided `read_debounced()` function implements a simple software debounce).

### Power / Ground
- Use a stable 3.3V regulator with sufficient current (>= 500 mA recommended for ESP32 + sensors; 1A for more margin).
- If any peripheral requires 5V (some MQ modules), ensure that sensor outputs are compatible with 3.3V logic or use level shifting.
- Connect all grounds (ESP32, sensors, power supply) to a common reference.

---

## Suggested simple wiring (breadboard / prototyping)

