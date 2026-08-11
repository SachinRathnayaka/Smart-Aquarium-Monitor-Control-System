<div align="center">

<img src="Images/3D_PCB_Isometric.png" alt="SRAqua Guard PCB" width="420"/>

# 🐠 SRAqua Guard
### Ultimate Smart Aquarium Monitor & Automation System

*Real-time water telemetry · Intelligent automation · Voice control · Cross-platform monitoring*

---

![ESP32](https://img.shields.io/badge/ESP32-WROOM--32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![Firebase](https://img.shields.io/badge/Firebase-RTDB-FFCA28?style=for-the-badge&logo=firebase&logoColor=black)
![Electron](https://img.shields.io/badge/Electron-Windows--App-47848F?style=for-the-badge&logo=electron&logoColor=white)
![Android](https://img.shields.io/badge/Android-minSdk%2023-3DDC84?style=for-the-badge&logo=android&logoColor=white)
![HTML5](https://img.shields.io/badge/HTML5-Dashboard-E34F26?style=for-the-badge&logo=html5&logoColor=white)
![CSS3](https://img.shields.io/badge/CSS3-Glassmorphism-1572B6?style=for-the-badge&logo=css3&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-ES6+-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)
![KiCad](https://img.shields.io/badge/KiCad-10.0.3-314CB0?style=for-the-badge&logo=kicad&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge&logo=opensourceinitiative&logoColor=white)

**Author:** [Sachin Rathnayaka](https://github.com/SachinRathnayaka) &nbsp;·&nbsp; **Repo:** [Smart-Aquarium-Monitor-Control-System](https://github.com/SachinRathnayaka/Smart-Aquarium-Monitor-Control-System) &nbsp;·&nbsp; **Live Demo:** [Web Dashboard](https://sachinrathnayaka.github.io/Smart-Aquarium-Monitor-Control-System/)

</div>

---

## 📖 Table of Contents

- [Project Status](#-project-status)
- [Project Overview](#-project-overview)
- [Visual Showcase](#-visual-showcase)
- [Key Features](#-key-features)
- [System Architecture](#-system-architecture)
- [Repository Structure](#-repository-structure)
- [Getting Started](#-getting-started)
  - [Prerequisites](#prerequisites)
  - [1 · Firmware (ESP32)](#1--firmware-esp32)
  - [2 · Web Dashboard](#2--web-dashboard)
  - [3 · Android App](#3--android-app)
  - [4 · Windows Desktop App](#4--windows-desktop-app)
  - [Pre-built Binaries](#pre-built-binaries)
- [Firebase Configuration](#-firebase-configuration)
- [Voice Commands](#-voice-commands)
- [Sensor Calibration](#-sensor-calibration)
- [Troubleshooting](#-troubleshooting)
- [Electrical Safety](#-electrical-safety)
- [Hardware & PCB](#-hardware--pcb)
- [Bill of Materials](#-bill-of-materials-summary)
- [Roadmap](#️-roadmap)
- [Contributing](#-contributing)
- [License](#-license)

---

## 🚧 Project Status

**Status: Active Development**

SRAqua Guard is an actively developed open-source hardware and software project. Features, firmware behaviour, PCB revisions, and application interfaces may change between releases. The project is functional and has been tested with real hardware — see the Visual Showcase for screenshots. Always refer to the latest `main` branch for the most current state.

| Component | Status |
|---|---|
| ESP32 Firmware | ✅ Functional |
| Web Dashboard | ✅ Functional |
| Android App | ✅ Functional |
| Windows Desktop App | ✅ Functional |
| KiCad PCB (Hardware) | ✅ Complete |
| Documentation | ✅ Complete |

---

## 🌊 Project Overview

**SRAqua Guard** is a fully open-source, advanced IoT aquarium monitoring and automation system that transforms any aquarium into an intelligently managed ecosystem. The system is built around an **ESP32 microcontroller** with a custom **KiCad PCB**, pushing real-time sensor telemetry to **Firebase Realtime Database**, and surfaced through three client platforms — a browser-based web dashboard, an Android mobile app, and a Windows desktop app.

The firmware is engineered with a **fully non-blocking `millis()` architecture**, a **DS3231 RTC** as the sole time source (ensuring offline-safe automation), and intelligent fallback behavior: the system continues sensing, controlling, and displaying locally even without Wi-Fi. When battery voltage drops below a threshold, the ESP32 enters a **5-minute deep sleep cycle** to preserve power.

> [!NOTE]
> This project includes **240 V AC mains wiring** for the water heater relay. All high-voltage sections must be handled by a qualified person. Use appropriate insulation, cable sleeving, and maintain required creepage clearances at all times.

---

## 🖼️ Visual Showcase

### Web Dashboard

| Overview | Firebase Setup | Theme Settings |
|:---:|:---:|:---:|
| ![Overview](Images/Web_Dashboard_Screenshots/Web_Dashboard_Overview.png) | ![Setup](Images/Web_Dashboard_Screenshots/Web_Dashboard_Firebase_Setup.png) | ![Themes](Images/Web_Dashboard_Screenshots/Web_Dashboard_Theme_Settings.png) |

| 21-Day Sensor Analytics | 21-Day Feed Analytics | Auto Feeder Scheduler |
|:---:|:---:|:---:|
| ![Sensor Chart](Images/Web_Dashboard_Screenshots/Web_Dashboard_Analytics_Sensors_Chart.png) | ![Feed Chart](Images/Web_Dashboard_Screenshots/Web_Dashboard_Analytics_Feeds_Chart.png) | ![Scheduler](Images/Web_Dashboard_Screenshots/Web_Dashboard_Feeder_Scheduler.png) |

| Live Voice Assistant | Voice Commands List | Alerts & History |
|:---:|:---:|:---:|
| ![Voice Live](Images/Web_Dashboard_Screenshots/Web_Dashboard_Voice_Assistant_Live.png) | ![Voice Commands](Images/Web_Dashboard_Screenshots/Web_Dashboard_Voice_Commands_List.png) | ![Alerts](Images/Web_Dashboard_Screenshots/Web_Dashboard_Alerts_History.png) |

---

### Android Mobile App

| Firebase Setup | Telemetry & Sensors | Header Navigation | Command Center | App Icon |
|:---:|:---:|:---:|:---:|:---:|
| ![Setup](Images/Mobile_App_Screenshots/Mobile_App_Firebase_Setup.jpeg) | ![Sensors](Images/Mobile_App_Screenshots/Mobile_App_Telemetry_Sensors.jpeg) | ![Nav](Images/Mobile_App_Screenshots/Mobile_App_Header_Navigation.jpeg) | ![Command](Images/Mobile_App_Screenshots/Mobile_App_Command_Center.jpeg) | ![Icon](Images/Mobile_App_Screenshots/Mobile_App_Icon_Preview.jpeg) |

---

### Windows Desktop App

| Main Dashboard | Firebase Setup | Theme Settings | Command Center | Desktop Icon |
|:---:|:---:|:---:|:---:|:---:|
| ![Main](Images/Desktop_App_Screenshots/Desktop_App_Main_Dashboard.png) | ![Setup](Images/Desktop_App_Screenshots/Desktop_App_Firebase_Setup.png) | ![Theme](Images/Desktop_App_Screenshots/Desktop_App_Theme_Settings.png) | ![Command](Images/Desktop_App_Screenshots/Desktop_App_Command_Center.png) | ![Icon](Images/Desktop_App_Screenshots/Desktop_App_Icon_Preview.jpeg) |

---

### Custom KiCad PCB — 3D Renders

**Standard Renders**

| Top | Bottom | Front | Back | Left | Right |
|:---:|:---:|:---:|:---:|:---:|:---:|
| ![Top](Hardware/3D_Models/3D_Standard/3D_PCB_Top.png) | ![Bottom](Hardware/3D_Models/3D_Standard/3D_PCB_Bottom.png) | ![Front](Hardware/3D_Models/3D_Standard/3D_PCB_Side_Front.png) | ![Back](Hardware/3D_Models/3D_Standard/3D_PCB_Side_Back.png) | ![Left](Hardware/3D_Models/3D_Standard/3D_PCB_Side_Left.png) | ![Right](Hardware/3D_Models/3D_Standard/3D_PCB_Side_Right.png) |

**Raytraced Renders**

| Top | Bottom | Front | Back | Left | Right |
|:---:|:---:|:---:|:---:|:---:|:---:|
| ![Top](Hardware/3D_Models/3D_Raytraced/3D_PCB_Top_Raytraced.png) | ![Bottom](Hardware/3D_Models/3D_Raytraced/3D_PCB_Bottom_Raytraced.png) | ![Front](Hardware/3D_Models/3D_Raytraced/3D_PCB_Side_Front_Raytraced.png) | ![Back](Hardware/3D_Models/3D_Raytraced/3D_PCB_Side_Back_Raytraced.png) | ![Left](Hardware/3D_Models/3D_Raytraced/3D_PCB_Side_Left_Raytraced.png) | ![Right](Hardware/3D_Models/3D_Raytraced/3D_PCB_Side_Right_Raytraced.png) |

---

## ✨ Key Features

### 🔬 Water Quality Telemetry
- **Temperature** via DS18B20 OneWire sensor — with DS18B20 power-on sentinel (85 °C) rejection
- **pH** via analog sensor module — linear calibration with configurable slope/offset
- **TDS (Total Dissolved Solids)** via analog sensor — temperature-compensated formula
- **Turbidity** via analog sensor — quadratic NTU conversion with 20-point Simple Moving Average (SMA) and degradation trend prediction (days to next maintenance)
- Sensor readings every **2 seconds** in non-blocking loop

### ⚙️ Intelligent Automation
| Condition | Automatic Response |
|---|---|
| Temperature > 29 °C | Cooling fan ON |
| Temperature < 25 °C | Water heater ON (with 30-min hard safety cutoff) |
| Turbidity > 35 NTU or TDS > 500 ppm | Submersible pump ON |
| Turbidity > 70 NTU | Alarm buzzer ON |
| pH outside 6.3–8.3 | Alarm buzzer ON |
| TDS > 1200 ppm | Auto water-change sequence (8-min max, 1-hr cooldown) |
| Pump running 2 h+ with high turbidity | Filter-change alert pushed to Firebase + LCD |
| Temperature sensor fault (OOR / 85 °C sentinel) | Heater hard-disabled, buzzer triggered |
| Battery-only inferred (voltage < 11.2 V, no charge trend) | All loads OFF → 5-min deep sleep cycle |

### 💡 ARGB Lighting Day-Cycle (RTC-driven)
| Time | Lighting |
|---|---|
| 06:00 – 09:59 | Sunrise — dim orange / warm yellow |
| 10:00 – 17:59 | Daytime — bright white / light blue |
| 18:00 – 05:59 | Night — calm breathing blue/purple (non-blocking animation) |

### 🎙️ Voice Assistant
- **8 core command groups** (Status · Feed · Pump · Heater · Fan · Lights · Buzzer · Mode) plus Demo Mode test commands, via the **Web Speech API** — no cloud API key required
- Live transcription displayed in the dashboard header with 5-second silence auto-stop
- Voice TTS feedback: browser speaks alert summaries aloud

### 📅 Auto Fish Feeder
- Servo-based feeder FSM (4-state: Idle → Extend → Hold → Return)
- Unlimited scheduled feeds — configurable time, day(s) of week, specific date, and portion count
- Manual one-click feed button on web/local dashboard
- Daily feed counter persisted in browser `localStorage`

### 🔋 Power Telemetry
- 12 V battery voltage via resistor-divider (×4 ratio) on GPIO 33
- Battery percentage from 9.0 V (0%) to 12.6 V (100%)
- Charging inference via 10-point voltage trend (>50 mV rise = charging)
- Solar charging input terminal on PCB (J15)

### 🖥️ Web Dashboard

👉 **[🚀 Try Live Web Dashboard Here](https://sachinrathnayaka.github.io/Smart-Aquarium-Monitor-Control-System/)** *(Fully functional Demo Mode available — no hardware required!)*

- Pure single-file HTML5/CSS3/JavaScript — **zero build step, zero dependencies to install**
- Firebase Realtime Database v10.8.0 SDK — real-time bidirectional sync
- **4 built-in themes:** Ocean · Cyber · Emerald · Sunset + custom hex color picker
- **Demo Mode** — fully functional without any hardware (simulated sensor data)
- 21-day historical analytics charts (Temperature, pH, TDS, Turbidity, Feed count) via Chart.js
- ARGB hex color picker — picks and syncs live `light_color` to Firebase
- RTC time sync — pushes browser clock to ESP32 via Firebase (`/commands/rtc_sync`)
- Alerts & notification history (last 50 events)

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                        ESP32 (Firmware)                             │
│                                                                     │
│  Sensors ──► readAllSensors() ──► evaluateAutomation()              │
│  DS18B20 · pH · TDS · Turbidity · Battery                           │
│                    │                                                │
│         millis() non-blocking loop (no delay())                     │
│                    │                                                │
│  ┌─────────────────┼─────────────────────────┐                     │
│  │ DS3231 RTC      │ FastLED ARGB   Servo     │                     │
│  │ (offline time)  │ (day cycle)    (feeder)  │                     │
│  └─────────────────┼─────────────────────────┘                     │
│                    │                                                │
│         pushFirebase() every 5 s  ◄──►  Firebase RTDB              │
│         streamCallback()          ◄──►  /controls + /commands       │
│         Local WebServer (port 80) → in-network dashboard            │
└─────────────────────────────────────────────────────────────────────┘
                              │
                  Firebase Realtime Database
                  /devices/aquaSense/
                  ├── sensors/
                  ├── status/
                  ├── controls/
                  ├── commands/
                  └── alerts/
                              │
          ┌───────────────────┼───────────────────┐
          │                   │                   │
   Web Dashboard        Android App         Windows App
   (index.html)     (WebView wrapper)    (Electron shell)
   Any browser      com.wrscn.sraquaguard  com.wrscn.sraquaguard
```

### ESP32 GPIO Pin Map

| Pin | Signal | Direction |
|-----|--------|-----------|
| GPIO 34 | pH Sensor (Analog) | Input |
| GPIO 35 | TDS Sensor (Analog) | Input |
| GPIO 32 | Turbidity Sensor (Analog) | Input |
| GPIO 25 | DS18B20 Temperature (1-Wire) | Input |
| GPIO 33 | BMS / Battery Voltage (Analog) | Input |
| GPIO 26 | Cooling Fan Relay | Output |
| GPIO 27 | Submersible Pump Relay | Output |
| GPIO 17 | Water Heater Relay ⚠️ 240 V | Output |
| GPIO 23 | Fish Feeder Servo (PWM) | Output |
| GPIO 16 | ARGB LED Strip (WS2812B) | Output |
| GPIO 13 | Alarm Buzzer | Output |
| GPIO 21 | I²C SDA (LCD + RTC) | I²C |
| GPIO 22 | I²C SCL (LCD + RTC) | I²C |
| GPIO 4 | Wi-Fi / Firebase Status LED | Output |

### Arduino Libraries Required

| Library | Purpose | Install via |
|---------|---------|-------------|
| `Firebase ESP Client` by mobizt | Firebase RTDB | Arduino Library Manager |
| `LiquidCrystal I2C` | 16×2 LCD display | Arduino Library Manager |
| `DallasTemperature` | DS18B20 sensor | Arduino Library Manager |
| `OneWire` | 1-Wire bus | Arduino Library Manager |
| `RTClib` | DS3231 RTC | Arduino Library Manager |
| `FastLED` | WS2812B ARGB LEDs | Arduino Library Manager |
| `ESP32Servo` | Servo motor control | Arduino Library Manager |

---

## 📁 Repository Structure

```
Smart-Aquarium-Monitor-Control-System/
│
├── 📁 Firmware/
│   └── ESP32/
│       └── IoT_Based_Smart_Aquarium_Monitor__Control_System_ESP32/
│           ├── IoT_Based_Smart_Aquarium_Monitor__Control_System_ESP32.ino
│           │   # Main ESP32 firmware — non-blocking millis() architecture,
│           │   # offline-safe automation, DS3231 RTC, Firebase RTDB streaming,
│           │   # ARGB day cycle, feeder FSM, battery telemetry, deep sleep
│           └── secrets.example.h
│               # Credential template — copy to secrets.h, fill in values,
│               # add secrets.h to .gitignore (never commit real credentials)
│
├── 📁 Web_Dashboard/
│   ├── index.html          # Single-file dashboard — no build step required
│   └── assets/
│       └── logo.png
│
├── 📁 Android_App/          # Android Studio project (com.wrscn.sraquaguard)
│   ├── app/
│   │   ├── build.gradle    # compileSdk 35, minSdk 23 (Android 6.0+), versionName 1.0
│   │   └── src/main/
│   │       ├── AndroidManifest.xml
│   │       ├── assets/
│   │       │   ├── index.html      # Bundled web dashboard
│   │       │   └── assets/logo.png
│   │       ├── java/com/wrscn/sraquaguard/
│   │       │   └── MainActivity.java  # WebView with immersive fullscreen
│   │       └── res/
│   │           ├── drawable/sraqua_logo.png
│   │           ├── mipmap-*/       # Launcher icons (all densities)
│   │           └── values/
│   │               ├── colors.xml
│   │               └── styles.xml  # Theme.SRAquaGuard (dark slate)
│   └── README.md
│
├── 📁 Windows_App/          # Electron desktop application
│   ├── main.js             # BrowserWindow 1280×800, contextIsolation: true
│   ├── index.html          # Dashboard file loaded by Electron
│   ├── package.json        # electron + electron-builder, NSIS target
│   ├── package-lock.json
│   └── assets/
│       ├── logo.ico        # Windows app icon
│       └── logo.png
│
├── 📁 Hardware/             # KiCad 10.0.3 PCB project
│   ├── *.kicad_pcb         # PCB layout
│   ├── *.kicad_sch         # Schematic
│   ├── *.kicad_pro         # Project file
│   ├── 3D_Models/
│   │   ├── 3D_Standard/    # 6 standard 3D renders (Top/Bottom/Sides)
│   │   └── 3D_Raytraced/   # 6 raytraced 3D renders (Top/Bottom/Sides)
│   ├── Gerber_Files/       # Full Gerber set (F_Cu, B_Cu, Masks, Silkscreen,
│   │   ├── *.gbr           #   Edge Cuts, Drill files) + zipped Gerber bundle
│   │   └── Gerber_ZIP/
│   └── DIY_Etching_PDFs/   # PDFs for DIY toner-transfer / UV etching
│       ├── Copper_Layers/  # Top + Bottom copper (normal + mirrored)
│       ├── Silkscreen_Layers/
│       └── Assembly_Drawings/
│
├── 📁 Docs/
│   ├── Bill_of_Materials.md          # 41 line items · 74 total components
│   ├── Components_List_BOM.csv       # Machine-readable BOM
│   ├── Schematic_Diagram.pdf         # Full schematic PDF export
│   └── PCB_Artwork_Etching_Layout.pdf
│
├── 📁 Images/               # All screenshots and renders used in this README
│   ├── 3D_PCB_Isometric.png
│   ├── ESP32_Pin_Pitch_Dimensions.png
│   ├── Web_Dashboard_Screenshots/    # 9 screenshots
│   ├── Mobile_App_Screenshots/       # 5 screenshots
│   └── Desktop_App_Screenshots/      # 5 screenshots
│
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

| Requirement | Details |
|---|---|
| Arduino IDE ≥ 2.x **or** PlatformIO | For flashing the ESP32 firmware |
| ESP32 Arduino Core | Board package for ESP32 (add `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` to board manager) |
| Firebase project | Free Spark plan is sufficient — Realtime Database enabled |
| Any modern browser | Chrome recommended for Voice Control (Web Speech API) |
| Android Studio (optional) | To build the `.apk` from source |
| Node.js ≥ 18 (optional) | To run/build the Windows Electron app from source |

---

### 1 · Firmware (ESP32)

#### Step 1 — Install Libraries

Open **Arduino IDE → Tools → Manage Libraries** and install each of the following:

```
Firebase ESP Client    (by mobizt)
LiquidCrystal I2C
DallasTemperature
OneWire
RTClib
FastLED
ESP32Servo
```

#### Step 2 — Configure Credentials (secrets.h pattern)

To avoid accidentally committing credentials to a public repository, use the recommended `secrets.h` approach:

**2a.** Copy the provided template:
```
Firmware/ESP32/.../secrets.example.h  →  copy as  secrets.h
```

**2b.** Open `secrets.h` and fill in your values:
```cpp
// secrets.h — DO NOT commit this file
#pragma once

// Wi-Fi
#define WIFI_SSID     "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Firebase
#define API_KEY       "YOUR_FIREBASE_API_KEY"
#define DATABASE_URL  "YOUR_FIREBASE_DATABASE_URL"
#define USER_EMAIL    "YOUR_FIREBASE_USER_EMAIL"
#define USER_PASSWORD "YOUR_FIREBASE_USER_PASSWORD"
```

**2c.** Add `secrets.h` to your `.gitignore` so it is never tracked:
```
# .gitignore
secrets.h
```

The main `.ino` file already contains `#include "secrets.h"` and uses these defines — **do not paste real credentials directly into the `.ino` file.**

> [!WARNING]
> **Never commit real credentials to a public repository.** The `secrets.example.h` template uses intentionally non-functional placeholder strings. Always work from `secrets.h` (git-ignored) in your local copy.

> [!NOTE]
> **Firebase API Key vs secret credentials:** The Firebase Web API Key (found in Project Settings) is a *project identifier* — it is not a secret by itself, as it is embedded in every web client. What must be protected are your **Firebase Auth user email and password** (the device account). Firebase Database Rules (see the Firebase Configuration section) are your primary security boundary — ensure they are correctly configured before going live.

#### Step 3 — Sensor Calibration (optional)

The firmware ships with **Dummy Data switches** that generate realistic random values for dry testing without any sensors connected:

```cpp
bool DUMMY_PH        = false;   // set true for dry test
bool DUMMY_TDS       = false;
bool DUMMY_TURBIDITY = false;
bool DUMMY_TEMP      = false;
bool DUMMY_BATTERY   = false;
```

For a real deployment, calibrate your pH probe and update:

```cpp
float PH_SLOPE  = -5.70f;   // Adjust after 2-point calibration
float PH_OFFSET = 21.34f;
```

#### Step 4 — Flash

Select **Board: `ESP32 Dev Module`**, choose the correct COM port, then click **Upload**.

---

### 2 · Web Dashboard

The dashboard is a **single self-contained HTML file** with no build step.

**Option A — Open locally in browser**
```
Open Web_Dashboard/index.html directly in Chrome or Edge
```

**Option B — Host on GitHub Pages**
1. Copy `Web_Dashboard/index.html` and `Web_Dashboard/assets/` to the root of a GitHub Pages branch.
2. Enable GitHub Pages in your repository settings.
3. Share the `https://<username>.github.io/<repo>/` URL.

**Connecting to Firebase**

When the dashboard loads, a setup modal appears. Enter your Firebase project credentials:

| Field | Where to find it |
|---|---|
| **API Key** | Firebase Console → Project Settings → General |
| **Database URL** | Firebase Console → Realtime Database → Data tab |
| **User Email** | Firebase Console → Authentication → Users |
| **User Password** | The password for that Firebase Auth user |

Click **Connect Firebase** to go live, or **Enter Demo Mode** to explore with simulated data — no hardware needed.

---

### 3 · Android App

**Build from source (Android Studio)**

1. Open the `Android_App/` folder in **Android Studio**.
2. Allow Gradle to sync (compileSdk 35, minSdk 23 — Android 6.0+).
3. Build the APK:

```bash
./gradlew assembleDebug
# Output: Android_App/app/build/outputs/apk/debug/app-debug.apk
```

Or use **Build → Build App Bundle(s)/APK(s) → Build APK(s)** from the menu.

The app loads `index.html` bundled in `app/src/main/assets/` via a hardware-accelerated WebView with immersive fullscreen mode. Firebase credentials are configured through the in-app setup modal — no separate `google-services.json` is needed.

> [!NOTE]
> The web dashboard inside the APK includes its own Firebase setup flow. Simply enter your Firebase credentials the first time you launch the app, and they are persisted in the WebView's `localStorage`.

---

### 4 · Windows Desktop App

**Run from source**

```bash
cd Windows_App
npm install
npm start
```

**Build an installable `.exe`**

```bash
npm run build
# Output: Windows_App/dist/SRAqua Guard Setup x.x.x.exe  (NSIS installer)
```

The Electron window opens at **1280 × 800** (minimum 1000 × 650) and loads `index.html`. Node integration is disabled (`nodeIntegration: false`, `contextIsolation: true`) for security.

---

### Pre-built Binaries

Pre-compiled binaries are published through the **GitHub Releases** page. Check the Releases tab for the latest available versions:

| Platform | File | Notes |
|---|---|---|
| Android | `SRAqua_Guard_v1.0.apk` | Sideload — enable "Install from unknown sources" in device settings |
| Windows | `SRAqua Guard Setup 1.0.0.exe` | NSIS installer — run and follow on-screen prompts |

👉 **[View GitHub Releases](https://github.com/SachinRathnayaka/Smart-Aquarium-Monitor-Control-System/releases)**

> [!NOTE]
> If no release is listed yet, build from source using the instructions above — it only takes a few minutes.

---

## 🔥 Firebase Configuration

### Database Path Structure

All device data lives under a single root path:

```
/devices/aquaSense/
├── sensors/            ← pushed by ESP32 every 5 s
│   ├── temperature_c
│   ├── ph_value / ph_voltage
│   ├── tds_ppm / tds_voltage
│   ├── turbidity_ntu / turbidity_sma
│   ├── predicted_days_to_maintenance
│   ├── battery_voltage / battery_percent
│   ├── charging_inferred
│   ├── dummy_ph / dummy_tds / dummy_turbidity / dummy_temp / dummy_battery
│   └── timestamp
├── status/             ← pushed by ESP32 every 5 s
│   ├── manual_mode / fan / pump / heater / buzzer
│   ├── water_change_active / filter_change_required
│   ├── wifi_connected / firebase_ready / battery_only_mode
├── controls/           ← written by dashboard, read by ESP32 stream
│   ├── manual_mode / manual_fan / manual_pump
│   ├── manual_heater / manual_buzzer
│   ├── manual_light / light_color
│   └── feed_now
├── commands/           ← one-shot voice hooks (auto-reset after each push)
│   ├── voice_feed
│   ├── voice_pump
│   └── rtc_sync
└── alerts/
    └── filter_change_required_at
```

### Recommended Security Rules

The rules below apply **least-privilege** access: only authenticated users can read or write device data. No unauthenticated access is permitted at any path.

```json
{
  "rules": {
    ".read":  false,
    ".write": false,
    "devices": {
      "aquaSense": {
        "sensors":  { ".read": "auth != null", ".write": "auth != null" },
        "status":   { ".read": "auth != null", ".write": "auth != null" },
        "controls": { ".read": "auth != null", ".write": "auth != null" },
        "commands": { ".read": "auth != null", ".write": "auth != null" },
        "alerts":   { ".read": "auth != null", ".write": "auth != null" }
      }
    }
  }
}
```

> [!IMPORTANT]
> **Never use `".read": true, ".write": true`** (open rules) — this exposes your device data to anyone on the internet. The root-level `false` defaults above ensure that any path not explicitly listed is denied by default. The firmware authenticates using the dedicated Firebase Auth device account configured in `secrets.h`.

> [!NOTE]
> For a single-owner deployment, the `auth != null` check is sufficient since only your device account and your own dashboard sessions are authenticated. For multi-user or team setups, consider tightening rules further using `auth.uid` matching against an allow-list stored in the database.

> [!TIP]
> **Future enhancement — role-based rules:** In an ideal setup, write permissions would be further restricted by path role: `sensors/` and `status/` would be write-restricted to the ESP32 device account only, while `controls/` and `commands/` would be writable by dashboard users. This can be implemented by storing your ESP32's Firebase Auth UID in the database and matching against it in rules. For a personal single-device deployment the current rules are adequate.

### Authentication Setup

1. In Firebase Console → **Authentication → Sign-in method**, enable **Email/Password**.
2. Under **Authentication → Users**, click **Add user** and create a dedicated device account.
3. Use those credentials in the firmware's `USER_EMAIL` and `USER_PASSWORD` defines.

---

## 🎙️ Voice Commands

Voice control uses the browser's built-in **Web Speech API** (no API key required). Click the **Voice Control** button in the dashboard header, then speak any of the following commands:

| Voice Command | Alternatives | Action |
|---|---|---|
| **"Tell Status"** | "Tank Status", "How is the water" | Reads the current sensor values via TTS and reports whether the aquarium conditions are healthy or critical |
| **"Feed"** | — | Triggers fish feeder immediately — sends `voice_feed` to Firebase |
| **"Pump on"** | "Turn on pump" | Enables manual mode and turns pump ON — sends `voice_pump` to Firebase |
| **"Pump off"** | "Turn off pump" | Enables manual mode and turns pump OFF |
| **"Heater on"** | "Turn on heater" | Enables manual mode and turns heater ON |
| **"Heater off"** | "Turn off heater" | Enables manual mode and turns heater OFF |
| **"Fan on"** | "Turn on fan" | Enables manual mode and turns cooling fan ON |
| **"Fan off"** | "Turn off fan" | Enables manual mode and turns cooling fan OFF |
| **"Lights on"** | "Light on", "Turn on lights" | Turns ARGB tank lights ON |
| **"Lights off"** | "Light off", "Turn off lights" | Turns ARGB tank lights OFF |
| **"Buzzer on"** | "Turn on buzzer" | Manually enables alarm buzzer |
| **"Buzzer off"** | "Turn off buzzer" | Manually disables alarm buzzer |
| **"Auto Mode"** | "Auto" | Switches system to automatic control |
| **"Manual Mode"** | "Manual" | Switches system to manual control |
| **"Test Alert"** | — | Simulates a critical alert (Demo Mode) |
| **"Fix Alert"** | "Reset Alert" | Restores simulated normal conditions (Demo Mode) |

> [!NOTE]
> Voice control requires a browser that supports the Web Speech API (Chrome and Edge recommended). Microphone permission must be granted. The recognition session auto-stops after **15 seconds** of silence, or **5 seconds** after the last detected word. Web Speech API support and behaviour may vary by browser version and operating system — Chrome on desktop provides the most consistent experience.

---

## 🧪 Sensor Calibration

The firmware ships with **Dummy Data mode** for all sensors — ideal for testing the dashboard and Firebase integration without any physical hardware connected. Set the relevant flag to `true` in the firmware:

```cpp
bool DUMMY_PH        = false;   // set true to generate simulated pH readings
bool DUMMY_TDS       = false;
bool DUMMY_TURBIDITY = false;
bool DUMMY_TEMP      = false;
bool DUMMY_BATTERY   = false;
```

### pH Calibration (2-point method)

Perform a standard 2-point calibration using pH 4.0 and pH 7.0 buffer solutions:

1. Dip the probe in **pH 7.0** buffer → note the raw ADC voltage reading from the serial monitor.
2. Dip the probe in **pH 4.0** buffer → note the second ADC voltage reading.
3. Calculate slope and offset from those two readings, then update the firmware:

```cpp
float PH_SLOPE  = -5.70f;   // Volts per pH unit — adjust after calibration
float PH_OFFSET = 21.34f;   // Intercept — adjust after calibration
```

> [!NOTE]
> The default values above (`PH_SLOPE = -5.70`, `PH_OFFSET = 21.34`) are example starting points. Your actual probe will require calibration — results will be inaccurate if you skip this step.

### TDS Calibration

The TDS sensor uses a temperature-compensated formula built into the firmware. For best accuracy, perform the reading in water at a known temperature and compare against a reference TDS meter. Adjust the conversion coefficient in firmware if needed.

### Turbidity Calibration

Turbidity uses a quadratic NTU conversion with a 20-point Simple Moving Average (SMA) for stability. The SMA also drives the degradation trend prediction (days to next filter maintenance). If your sensor module outputs a different voltage curve, update the quadratic coefficients in the firmware.

### Battery Voltage Calibration

The battery voltage is measured via a ×4 resistor divider on GPIO 33. If your resistor values differ slightly, adjust the divider ratio constant in firmware to match your measured real-world voltage.

---

## 🛠️ Troubleshooting

| Symptom | Likely Cause | Fix |
|---|---|---|
| ESP32 won't connect to Wi-Fi | Wrong credentials in `secrets.h` | Double-check `WIFI_SSID` and `WIFI_PASSWORD` — no extra spaces |
| Firebase stream not receiving data | Firebase rules blocking access | Verify security rules and that `USER_EMAIL`/`USER_PASSWORD` match an active Auth user |
| No sensor readings on dashboard | Wiring issue or Dummy Data off | Check GPIO connections against the pin map, or enable Dummy Data mode for testing |
| Temperature always reads 85 °C | DS18B20 power-on sentinel not rejected / wiring fault | Check 1-Wire pull-up resistor (4.7 kΩ on GPIO 25) and sensor wiring |
| pH / TDS / Turbidity readings wildly wrong | Uncalibrated sensor | Perform 2-point calibration — see Sensor Calibration section above |
| Android build fails (Gradle error) | Gradle/SDK mismatch | Run **File → Sync Project with Gradle Files** in Android Studio; ensure SDK 35 is installed |
| Electron app won't start | Missing Node modules | Run `npm install` inside `Windows_App/` before `npm start` |
| Voice commands not recognised | Browser or permission issue | Use Chrome or Edge; grant microphone permission when prompted; ensure Web Speech API is supported on your OS |
| ARGB lights show wrong colour | `light_color` hex not synced | Use the colour picker in the dashboard — it pushes the hex directly to Firebase `/controls/light_color` |
| Heater relay not triggering | Heater hard-disabled due to sensor fault | Check serial monitor for fault messages; verify DS18B20 wiring and that temperature reads correctly |

> [!NOTE]
> For firmware issues, always open the **Arduino IDE Serial Monitor** at **115200 baud** immediately after flashing — the firmware prints detailed startup diagnostics and runtime status every cycle.

---

## ⚡ Electrical Safety

> [!CAUTION]
> **This project involves 240 V AC mains voltage. Incorrect wiring can cause electric shock, fire, or death. Read this section fully before building.**

| Safety Requirement | Details |
|---|---|
| **Qualified person** | All mains-voltage wiring (relay K5, terminals J17/J18) must be connected and tested only by a suitably qualified person |
| **Mains-rated wiring** | Use cable rated ≥ 250 V for all AC sections — correctly sleeved and terminated |
| **Fusing** | Two 5 A / 250 V slow-blow fuses are included in the BOM and PCB layout — always fit them before energising |
| **Enclosure** | House the PCB in an IP-rated enclosure appropriate for the installation environment. Never leave mains terminals exposed |
| **Creepage & clearance** | Maintain the required PCB creepage distances between high-voltage and low-voltage traces — do not bridge them with solder bridges or conductive debris |
| **Low/high voltage separation** | Keep all 240 V AC wiring physically separated from the 5 V / 3.3 V signal wiring at all times |
| **RCD / GFCI protection** | Install an RCD (residual current device) or GFCI on the mains circuit feeding this system — especially important near water |
| **Waterproofing** | The PCB is not waterproof. Protect it from splashes. All sensor probes that enter the aquarium must be rated for submersion |
| **Heater safety cutoff** | The firmware enforces a **30-minute hard cutoff** on the heater relay and disables it on any temperature sensor fault — do not bypass this logic |

**Never power the mains section of the board during firmware development or debugging.** Work with the AC terminals disconnected and use the Dummy Data mode for testing.

---

## 🔩 Hardware & PCB

The custom PCB was designed in **KiCad 10.0.3** as a through-hole friendly layout suitable for DIY etching or professional PCB fabrication.

### PCB Outputs Provided

| Output | Location | Use |
|---|---|---|
| KiCad project files (`.kicad_pcb`, `.kicad_sch`, `.kicad_pro`) | `Hardware/` | Edit in KiCad 10.x |
| Gerber files + drill files | `Hardware/Gerber_Files/` | Send to any PCB fab (JLCPCB, PCBWay, OSH Park…) |
| Zipped Gerber bundle | `Hardware/Gerber_Files/Gerber_ZIP/` | Ready to upload directly to fab ordering page |
| DIY etching PDFs | `Hardware/DIY_Etching_PDFs/` | Toner-transfer / photoresist etching |
| Standard 3D renders (×6) | `Hardware/3D_Models/3D_Standard/` | Documentation / review |
| Raytraced 3D renders (×6) | `Hardware/3D_Models/3D_Raytraced/` | Realistic visual preview |
| Schematic PDF | `Docs/Schematic_Diagram.pdf` | Reference during assembly |

### ESP32 Fit Note

![ESP32 Dimensions](Images/ESP32_Pin_Pitch_Dimensions.png)

The PCB accepts a standard **ESP32-WROOM-32 / ESP32-DevKitC 38-pin** module mounted on two 1×19 female pin socket rows.

> [!WARNING]
> The board routes **240 V AC mains** through relay K5 (water heater output). Terminals J17 (AC out) and J18 (AC in) must use mains-rated wiring (≥ 250 V rated, properly sleeved). Observe required creepage distances. This section must only be connected and tested by a qualified person.

---

## 📋 Bill of Materials Summary

Full BOM: [`Docs/Bill_of_Materials.md`](Docs/Bill_of_Materials.md) · CSV: [`Docs/Components_List_BOM.csv`](Docs/Components_List_BOM.csv)

**74 components across 41 line items** — designed exclusively with through-hole parts for DIY-friendly assembly.

| Category | Key Parts | Qty |
|---|---|---|
| Microcontroller | ESP32-WROOM-32 / DevKitC (38-pin) | 1 |
| Transistors | BC547 NPN (relay drivers) | 4 |
| Relays | SANYOU SRD Form-C (5 V coil, 10 A / 250 VAC) | 3 |
| Diodes | SR560 Schottky (4×) · 1N4007 rectifier (4×) · RS505 bridge (1×) | 9 |
| Capacitors | 100 nF ceramic (4×) · 470 µF electrolytic (1×) · 1000 µF (3×) · 10 µF (1×) | 9 |
| Resistors | 330 Ω – 30 kΩ (various, ¼ W 5%) | 22 |
| Fuses | 5 A / 250 V slow-blow, 5×20 mm | 2 |
| Power Module | LM2596 / MP1584 DC-DC buck (12 V → 5 V) | 1 |
| Connectors | Screw terminals (5.08 mm) + pin headers (2.54 mm) | 22 |
| Switch | 6×6 mm tactile push button (Reset/Boot) | 1 |

---

## 🗺️ Roadmap

Planned improvements and ideas for future releases. Community contributions towards any of these are very welcome — see [Contributing](#-contributing).

| Feature | Description | Status |
|---|---|---|
| OTA firmware updates | Push firmware updates to the ESP32 wirelessly via Firebase or a local server | Planned |
| Additional sensor support | Water level sensor, dissolved oxygen (DO), CO₂ | Planned |
| Push notifications | Mobile push alerts via Firebase Cloud Messaging when alarms trigger | Planned |
| Advanced automation rules | User-configurable threshold values stored in Firebase (no reflash required) | Planned |
| Extended analytics | Analytics beyond 21 days; CSV export of historical sensor data | Planned |
| Multi-aquarium support | Extend the Firebase path structure to manage multiple tanks from one dashboard | Planned |
| Role-based Firebase security | Per-path UID-restricted rules (ESP32 write-only to `sensors/status/`, dashboard write-only to `controls/commands/`) | Planned |
| Android: native push notifications | Background service for alarm delivery when the app is not in the foreground | Planned |

> [!NOTE]
> This roadmap reflects current intentions and may change. Items marked **Planned** have not been started. If you have ideas or want to contribute to any of these, open an issue to discuss.

---

## 🤝 Contributing

Contributions are warmly welcomed! Whether it's a bug fix, new feature, improved calibration formula, or documentation update — please feel free to open an issue or pull request.

**Guidelines:**

1. **Fork** the repository and create your branch from `main`.
2. Follow the existing code style — especially the non-blocking `millis()` pattern in firmware (no `delay()` in `loop()`).
3. For dashboard changes, test in both **Demo Mode** and **Firebase Mode**.
4. For hardware changes, verify in KiCad 10.x and update the BOM accordingly.
5. **Never commit real Wi-Fi SSIDs, passwords, or Firebase API keys** — always use placeholder strings.
6. Open a **Pull Request** with a clear description of what was changed and why.

**Reporting Issues:**

Please include your ESP32 core version, Arduino IDE version, and relevant serial output when reporting firmware bugs.

---

## 📄 License

This project is licensed under the **MIT License** — free for personal and commercial use, modification, and distribution.

Licensed under the MIT License.
See [LICENSE](LICENSE) for the full license text.

Copyright © 2026 Sachin Rathnayaka (WRSCN)

---

<div align="center">

Made with ❤️ by **[Sachin Rathnayaka (WRSCN)](https://github.com/SachinRathnayaka)**

⭐ If this project helped you, please consider giving it a star on GitHub!

[![GitHub Stars](https://img.shields.io/github/stars/SachinRathnayaka/Smart-Aquarium-Monitor-Control-System?style=social)](https://github.com/SachinRathnayaka/Smart-Aquarium-Monitor-Control-System/stargazers)

</div>
