# 🎛️ Control Box (by Cyrald Labs)

> **A wireless Bluetooth Low Energy (BLE) macro pad and productivity shortcut controller built with an ESP32 and a 3x4 matrix keypad.**

---

## ⚡ Features

- **Wireless Bluetooth HID**: Connects directly to Linux / Windows / macOS as a native Bluetooth keyboard.
- **Short & Long Press Multi-Functionality**:
  - Snappy 450ms hold-time threshold for instant responses without long delays.
- **Native App Launching & Window Focusing**:
  - Automatically brings open background apps straight to the front, or launches them if closed.
- **Continuous Holding Actions**:
  - Smooth volume gliding and screen brightness stepping while holding down buttons.
- **Safe System Control**:
  - Sleep PC with a short tap; protected 3-flash warning on long hold to shutdown.
- **Visual Feedback**:
  - Blue onboard LED pulses for taps and confirms multi-function holds.

---

## 🗺️ Keypad Mapping Reference

| Key | Short Tap Action | Long Hold Action (Hold ~0.45s) |
| :---: | :--- | :--- |
| **`1`** | **Select All** (`Ctrl + A`) | 🌐 **Bring to Front / Launch Google Chrome** |
| **`2`** | **Copy** (`Ctrl + C`) | ✈️ **Bring to Front / Launch Telegram** |
| **`3`** | **Paste** (`Ctrl + V`) | 🚀 **Bring to Front / Launch Antigravity IDE** |
| **`4`** | **Previous Song** | ☀️ **Continuous Brightness Down** (`Super + F5`) |
| **`5`** | ⏯️ **Play / Pause Spotify** | 🎧 **Bring to Front / Launch Spotify** |
| **`6`** | **Next Song** | ☀️ **Continuous Brightness Up** (`Super + F6`) |
| **`7`** | 🔉 **Volume Down (Step)** | 🔉 **Continuous Volume Down** *(smooth glide)* |
| **`8`** | 🔇 **Mute / Unmute** | *(same)* |
| **`9`** | 🔊 **Volume Up (Step)** | 🔊 **Continuous Volume Up** *(smooth glide)* |
| **`*`** | 🗕 **Minimize Window** (`Super + H`) | ❌ **Close Window** (`Alt + F4`) |
| **`0`** | 📸 **Screenshot** (`PrintScreen`) | ⎋ **Escape** (`ESC`) |
| **`#`** | 🌙 **Sleep PC** (Suspend) | ⚡ **Shutdown PC** (Power Off) |

---

## 🔌 Hardware & Wiring

### Components:
1. **ESP32 DevKitC V1** (Dual-Core Xtensa LX6, 4MB Flash)
2. **ESP32 Sensor Expansion Board**
3. **3x4 Membrane Matrix Keypad** (7-pin flex ribbon)

### Expansion Board Pinout (Signal `S` Column):

| Ribbon Pin (Right to Left) | Keypad Function | Expansion Board Pin | ESP32 GPIO |
| :--- | :--- | :---: | :---: |
| **Pin 1** | Col 3 (keys 3, 6, 9, #) | **`D13`** | GPIO 13 |
| **Pin 2** | Col 2 (keys 2, 5, 8, 0) | **`D12`** | GPIO 12 |
| **Pin 3** | Col 1 (keys 1, 4, 7, *) | **`D14`** | GPIO 14 |
| **Pin 4** | Row 4 (keys *, 0, #) | **`D27`** | GPIO 27 |
| **Pin 5** | Row 3 (keys 7, 8, 9) | **`D26`** | GPIO 26 |
| **Pin 6** | Row 2 (keys 4, 5, 6) | **`D25`** | GPIO 25 |
| **Pin 7** | Row 1 (keys 1, 2, 3) | **`D33`** | GPIO 33 |

*(Note: Matrix keypads are passive mechanical switches and only require the Signal `S` pins; no `V` (Power) or `G` (Ground) needed).*

---

## 🚀 Building & Flashing

This project uses [PlatformIO](https://platformio.org/).

### 1. Compile Firmware
```bash
pio run
```

### 2. Upload to ESP32
```bash
pio run -t upload
```

### 3. Open Serial Monitor
```bash
pio device monitor
```

---

## 🏷️ Bluetooth Pairing
1. Go to your computer's **Bluetooth Settings**.
2. Find **`Control Box`** (Manufacturer: `Cyrald Labs`) and click **Connect**.
