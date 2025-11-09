# Smart Emergency Alert System (ESP32 + Blynk IoT)

## Project Overview
The **Smart Emergency Alert System** is an IoT-based safety project developed using an **ESP32 microcontroller**.  
It is designed to detect **fire**, **gas leaks**, and **manual SOS triggers**, and then send real-time alerts containing **fixed location coordinates** to a **Blynk IoT dashboard** via Wi-Fi.

The project is implemented entirely in **C language** using the **ESP-IDF framework**, ensuring reliability, low latency, and modular software structure.

---

## Key Features
- Fire detection using a digital flame sensor  
- Gas leak detection using MQ-series gas sensor (e.g., MQ-2)  
- Manual SOS trigger through a push button  
- Fixed coordinates embedded in firmware (no GPS module)  
- Wi-Fi based alerts using **Blynk IoT**  
- Modular and scalable ESP-IDF component structure  

---

## Hardware Requirements

| Component | Description | Example |
|------------|--------------|----------|
| ESP32 Dev Board | Main controller | ESP32-WROOM-32 |
| Flame Sensor | Digital output fire detector | YL-38 / YL-39 |
| Gas Sensor | Gas detection module | MQ-2 / MQ-135 |
| Push Button | Manual SOS trigger | Tactile switch |
| Power Supply | 5V input, 3.3V regulated output | AMS1117-3.3 / USB |
| Optional | Buzzer or LED | Visual or audible alert |

---

## Pin Configuration

| Signal | GPIO Pin | Description |
|---------|-----------|-------------|
| Flame Sensor | GPIO33 | Active LOW signal |
| Gas Sensor | GPIO32 | Active HIGH signal |
| SOS Button | GPIO25 | Active HIGH input |
| 3.3V / GND | Power | Common reference |

Refer to **docs/hardware.md** for detailed connection diagrams.

---

## Software Requirements
- ESP-IDF v4.x or v5.x  
- Python 3.8+ (for ESP-IDF tools)  
- Blynk IoT account and authentication token  

---

## Configuration
Before building, update the file `sdkconfig.defaults` with your credentials:

```text
CONFIG_WIFI_SSID="your_wifi_ssid"
CONFIG_WIFI_PASS="your_wifi_password"
CONFIG_BLYNK_TOKEN="your_blynk_token"
CONFIG_BLYNK_SERVER="blynk-cloud.com"
CONFIG_FIXED_LAT="12.9716"
CONFIG_FIXED_LON="77.5946"
