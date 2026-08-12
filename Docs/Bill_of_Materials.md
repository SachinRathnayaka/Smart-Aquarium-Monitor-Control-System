# 📋 Bill of Materials (BOM)

**Project:** IoT-Based Smart Aquarium Monitor & Control System  
**Designed by:** Sachin Rathnayaka (WRSCN)  
**Institution:** Sri Lanka Institute of Information Technology (SLIIT)  
**Module:** Fundamentals of Computing (IT1140) — Y1S1  
**Schematic Rev:** 1.0  
**EDA Tool:** KiCad 10.0.3  
**License:** CERN-OHL-S-2.0  
**Repository:** [github.com/SachinRathnayaka/Smart-Aquarium-Monitor-Control-System](https://github.com/SachinRathnayaka/Smart-Aquarium-Monitor-Control-System)  

---

> **Summary:** 41 line items — **74 total components**

> ⚠️ **Safety Warning:** This board includes 240 V AC mains wiring (J17, J18, K5 relay). All high-voltage sections must be handled by a qualified person. Use appropriate insulation, sleeving, and creepage clearances.

---

## 🧠 Microcontrollers  *(×1 total)*

| Reference | Qty | Description | Value / Specs | Package | Notes | Datasheet |
|-----------|-----|-------------|---------------|---------|-------|-----------|
| `LEsp32, Resp32` | 1 | ESP32 Development Board (38-pin) | ESP32-WROOM-32 / ESP32-DevKitC | DIP-38 / Module (fits 2× 1×19 female pin socket rows) | Dual-core 240 MHz, Wi-Fi + BT. Two 19-pin female socket rows on PCB. | [link](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) |

## 🔧 Transistors  *(×4 total)*

| Reference | Qty | Description | Value / Specs | Package | Notes | Datasheet |
|-----------|-----|-------------|---------------|---------|-------|-----------|
| `Q1, Q2, Q3, Q4` | 4 | NPN General-Purpose Transistor | BC547 — 45 V, 100 mA | TO-92 Inline | Relay driver stages; Q1=heater relay, Q2=pump relay, Q3=feeder relay, Q4=fan relay. | [link](https://www.onsemi.com/pub/Collateral/BC550-D.pdf) |

## ⚡ Diodes  *(×9 total)*

| Reference | Qty | Description | Value / Specs | Package | Notes | Datasheet |
|-----------|-----|-------------|---------------|---------|-------|-----------|
| `D1, D9, D10, D11` | 4 | Schottky Rectifier Diode | SR560 — 60 V, 5 A | DO-41 (SOD81) P10.16mm Horizontal | Flyback / freewheeling diodes across relay coils & motor loads. | [link](http://www.vishay.com/docs/88503/1n4001.pdf) |
| `D2, D12, D13, D14` | 4 | General-Purpose Rectifier Diode | 1N4007 — 1000 V, 1 A | DO-41 (SOD81) P10.16mm Horizontal | Reverse-polarity protection & flyback suppression. | [link](http://www.vishay.com/docs/88503/1n4001.pdf) |
| `D3` | 1 | Bridge Rectifier Module | RS505 — 50 V PIV, 5 A | Diode Bridge 19.0×3.5×10.0mm P5.0mm | Full-wave bridge for AC→DC conversion on main power input. | [link](https://diotec.com/tl_files/diotec/files/pdf/datasheets/b40c2300.pdf) |

## 🔋 Capacitors  *(×9 total)*

| Reference | Qty | Description | Value / Specs | Package | Notes | Datasheet |
|-----------|-----|-------------|---------------|---------|-------|-----------|
| `C1, C5` | 2 | Ceramic Disc Capacitor | 100 nF (0.1 µF) — 50 V | C Disc D5.0mm W2.5mm P5.00mm | Bypass / decoupling on power rails. | — |
| `C4, C8` | 2 | Ceramic Disc Capacitor | 100 nF (0.1 µF) — 50 V | C Disc D5.0mm W2.5mm P5.00mm | High-frequency decoupling near relay driver circuits. | — |
| `C2` | 1 | Electrolytic Capacitor (Polarized) | 470 µF — 25 V | CP Radial D8.0mm P3.50mm | Bulk filtering on regulated supply output. | — |
| `C3, C6, C7` | 3 | Electrolytic Capacitor (Polarized) | 1000 µF — 25 V | CP Radial D10.0mm P5.00mm | Main bulk filtering on 12 V and 5 V power rails. | — |
| `C9` | 1 | Electrolytic Capacitor (Polarized) | 10 µF — 25 V | CP Radial D5.0mm P2.00mm | Decoupling / soft-start capacitor. | — |

## 〰️ Resistors  *(×22 total)*

| Reference | Qty | Description | Value / Specs | Package | Notes | Datasheet |
|-----------|-----|-------------|---------------|---------|-------|-----------|
| `R5, R7, R9, R10, R20` | 5 | Carbon Film Resistor | 330 Ω — ¼ W, 5% | R Axial DIN0207 L6.3mm D2.5mm P10.16mm Horizontal | Current-limiting resistors for status / indicator LEDs. | — |
| `R3` | 1 | Carbon Film Resistor | 470 Ω — ¼ W, 5% | R Axial DIN0207 L6.3mm D2.5mm P10.16mm Horizontal | Base resistor / LED current limit. | — |
| `R1, R4, R8, R11, R17, R18` | 6 | Carbon Film Resistor | 1 kΩ — ¼ W, 5% | R Axial DIN0207 L6.3mm D2.5mm P10.16mm Horizontal | Transistor base resistors & pull-ups. (Schematic lists as '1Kom' and '1KOHM' — same value.) | — |
| `R6, R12` | 2 | Carbon Film Resistor | 2 kΩ — ¼ W, 5% | R Axial DIN0207 L6.3mm D2.5mm P10.16mm Horizontal | Voltage divider arms (sensor signal conditioning). | — |
| `R2` | 1 | Carbon Film Resistor | 2.2 kΩ — ¼ W, 5% | R Axial DIN0207 L6.3mm D2.5mm P10.16mm Horizontal | pH signal bias / divider. | — |
| `R13, R15, R16` | 3 | Carbon Film Resistor | 3.3 kΩ — ¼ W, 5% | R Axial DIN0207 L6.3mm D2.5mm P10.16mm Horizontal | I²C pull-up resistors (SDA/SCL lines to LCD & RTC). | — |
| `4.7kom1` | 1 | Carbon Film Resistor | 4.7 kΩ — ¼ W, 5% | R Axial DIN0207 L6.3mm D2.5mm P10.16mm Horizontal | DS18B20 temperature sensor 1-Wire pull-up resistor. | — |
| `R14, R19` | 2 | Carbon Film Resistor | 10 kΩ — ¼ W, 5% | R Axial DIN0207 L6.3mm D2.5mm P10.16mm Horizontal | EN/boot pull-up (R14) and BMS sense divider (R19). | — |
| `R21` | 1 | Carbon Film Resistor | 30 kΩ — ¼ W, 5% | R Axial DIN0207 L6.3mm D2.5mm P10.16mm Horizontal | Voltage divider upper arm (battery / solar level sense). | — |

## 🔌 Relays  *(×3 total)*

| Reference | Qty | Description | Value / Specs | Package | Notes | Datasheet |
|-----------|-----|-------------|---------------|---------|-------|-----------|
| `K4, K5, K6` | 3 | SPDT Relay — 5 V Coil | SANYOU SRD Series Form-C — 5 V coil, 10 A / 250 VAC contacts | Relay THT SPDT SANYOU SRD Form-C | K4 = Water pump, K5 = Heater (240 V), K6 = Auxiliary fan / cooling. | [link](http://www.sanyourelay.ca/public/products/pdf/SRD.pdf) |

## 🛡️ Fuses  *(×2 total)*

| Reference | Qty | Description | Value / Specs | Package | Notes | Datasheet |
|-----------|-----|-------------|---------------|---------|-------|-----------|
| `F1, F2` | 2 | Fuse Holder + Fuse — 5×20 mm | 5 A / 250 V slow-blow (or as per load; see notes) | Fuseholder Cylinder 5×20mm Schurter 0031.8201 Horizontal Open | F1 = Main DC input (12 V line). F2 = Secondary / sensor supply. Use appropriate ratings. | — |

## 🔘 Switchs  *(×1 total)*

| Reference | Qty | Description | Value / Specs | Package | Notes | Datasheet |
|-----------|-----|-------------|---------------|---------|-------|-----------|
| `SW1` | 1 | Momentary Push Button (Reset / Boot) | 6 mm × 6 mm tactile push button, SPST-NO | SW_PUSH_6mm THT | Reset button for ESP32. Also used as manual boot trigger (hold during power-on → download mode). | — |

## ⚙️ Power Modules  *(×1 total)*

| Reference | Qty | Description | Value / Specs | Package | Notes | Datasheet |
|-----------|-----|-------------|---------------|---------|-------|-----------|
| `J3` | 1 | DC-DC Buck Converter Module (12 V → 5 V) | LM2596 or MP1584 based — Input 12 V, Output 5 V / ≥2 A | 4-pin header 2.54 mm (mounts as sub-module) | Supplies 5 V rail for ESP32, relays, sensors & LCD. Pre-set output before installing. | — |

## 🔗 Connectors  *(×22 total)*

| Reference | Qty | Description | Value / Specs | Package | Notes | Datasheet |
|-----------|-----|-------------|---------------|---------|-------|-----------|
| `J1, J4, J12` | 3 | Screw Terminal Block — 3-pin | Pitch 5.08 mm, 15 A / 300 V | TerminalBlock Phoenix MKDS-1.5-3 P5.08mm Horizontal | J1 = Main power in/out bus, J4 = Water pump wiring, J12 = Auxiliary load wiring. | — |
| `J2, J11, J16` | 3 | DC Barrel Jack Terminal — 12 V Input | 2-pin screw terminal, 5.08 mm pitch | TerminalBlock Phoenix MKDS-1.5-2 P5.08mm Horizontal | 12 V DC supply connection points (J2 = main, J11 = backup/pump, J16 = fan/heatsink). | — |
| `J13PSW1` | 1 | 12 V Power Switch / Jack Terminal | 2-pin screw terminal, 5.08 mm pitch | TerminalBlock Phoenix MKDS-1.5-2 P5.08mm Horizontal | Operation mode selector input (Auto / Manual / Emergency OFF switch). | — |
| `J5` | 1 | LCD I²C Header — 4-pin | 2.54 mm pitch pin header, 1×4 | PinHeader 1×04 P2.54mm Vertical | Connects to 16×2 LCD with I²C backpack (PCF8574). Pins: GND, VCC(5V), SDA(G21), SCL(G22). | — |
| `J6` | 1 | Buzzer Header — 2-pin | 2.54 mm pitch pin header, 1×2 | PinHeader 1×02 P2.54mm Vertical | Active buzzer for alarm alerts (5 V). Driven via GPIO G26. | — |
| `J7` | 1 | pH Sensor Header — 3-pin | 2.54 mm pitch pin header, 1×3 | PinHeader 1×03 P2.54mm Vertical | Connects analog pH sensor module (5 V). Signal → G34 (ADC). | — |
| `J8` | 1 | Turbidity Sensor Header — 3-pin | 2.54 mm pitch pin header, 1×3 | PinHeader 1×03 P2.54mm Vertical | Connects turbidity sensor module (5 V). Signal → G32 (ADC). | — |
| `J9` | 1 | Temperature Sensor Header — 3-pin | 2.54 mm pitch pin header, 1×3 | PinHeader 1×03 P2.54mm Vertical | DS18B20 waterproof temperature sensor (3.3 V 1-Wire). Data → G4. | — |
| `J10` | 1 | TDS Sensor Header — 3-pin | 2.54 mm pitch pin header, 1×3 | PinHeader 1×03 P2.54mm Vertical | Total Dissolved Solids sensor module (3.3 V). Analog signal → G35 (ADC). | — |
| `J13` | 1 | Fish Feeder Stepper Motor Terminal — 3-pin | 5.08 mm pitch screw terminal, 1×3 | TerminalBlock Phoenix MKDS-1.5-3 P5.08mm Horizontal | Connects stepper/servo motor for automated fish feeder. Controlled via G17/G16. | — |
| `J14` | 1 | BMS / Battery Monitor Terminal — 2-pin | 5.08 mm pitch screw terminal, 1×2 | TerminalBlock Phoenix MKDS-1.5-2 P5.08mm Horizontal | Battery Management System connection. Battery level sense via G33 (ADC). | — |
| `J15` | 1 | Solar Panel Input Terminal — 2-pin | 5.08 mm pitch screw terminal, 1×2 | TerminalBlock Phoenix MKDS-1.5-2 P5.08mm Horizontal | Solar panel DC input (feeds BMS / battery charging circuit). | — |
| `J17` | 1 | 240 V AC Output Terminal — 2-pin | 5.08 mm pitch screw terminal, 1×2 — rated ≥250 VAC | TerminalBlock Phoenix MKDS-1.5-2 P5.08mm Horizontal | ⚠ HIGH VOLTAGE — Heater controlled by relay K5. Use insulated wiring rated for 240 V AC. | — |
| `J18` | 1 | 240 V AC Input Terminal — 2-pin | 5.08 mm pitch screw terminal, 1×2 — rated ≥250 VAC | TerminalBlock Phoenix MKDS-1.5-2 P5.08mm Horizontal | ⚠ HIGH VOLTAGE — Mains AC input feed through fuse F1 to K5 relay. | — |
| `J19` | 1 | ARGB Lighting Header — 3-pin | 2.54 mm pitch pin header, 1×3 | PinHeader 1×03 P2.54mm Vertical | Stress-free ARGB LED strip connector (5 V). Data → G27. | — |
| `J20` | 1 | Heat Dissipation / Fan Header — 2-pin | 2.54 mm pitch pin header, 1×2 | PinHeader 1×02 P2.54mm Vertical | Auxiliary 12 V cooling fan for heat dissipation unit. Relay K6 controlled. | — |
| `J21` | 1 | General Purpose 8-pin Header | 2.54 mm pitch pin header, 1×8 | PinHeader 1×08 P2.54mm Vertical | Status & power indicator LED array connector (multi-LED bar for 5 V, 12 V, pump, fan, etc.). | — |
| `J22` | 1 | General Purpose 6-pin Socket | 2.54 mm pitch pin socket, 1×6 | PinSocket 1×06 P2.54mm Vertical | RTC DS3231 module socket (3.3 V, I²C). SDA/SCL shared with LCD. | — |

---

## 📦 Consolidated Shopping List

Grouped by value for easy ordering:

### Resistors (all ¼ W, 5%, axial THT)

| Value | Qty | Ref |
|-------|-----|-----|
| 330 Ω | 5 | R5, R7, R9, R10, R20 |
| 470 Ω | 1 | R3 |
| 1 kΩ | 6 | R1, R4, R8, R11, R17, R18 |
| 2 kΩ | 2 | R6, R12 |
| 2.2 kΩ | 1 | R2 |
| 3.3 kΩ | 3 | R13, R15, R16 |
| 4.7 kΩ | 1 | 4.7kom1 |
| 10 kΩ | 2 | R14, R19 |
| 30 kΩ | 1 | R21 |

### Capacitors (THT)

| Value | Type | Qty | Ref |
|-------|------|-----|-----|
| 100 nF (0.1 µF) | Ceramic disc | 2 | `C1, C5` |
| 100 nF (0.1 µF) | Ceramic disc | 2 | `C4, C8` |
| 470 µF | Electrolytic (polarized) | 1 | `C2` |
| 1000 µF | Electrolytic (polarized) | 3 | `C3, C6, C7` |
| 10 µF | Electrolytic (polarized) | 1 | `C9` |

### Other Components

| Component | Value / Part No. | Qty |
|-----------|-----------------|-----|
| ESP32 Development Board (38-pin) | ESP32-WROOM-32 / ESP32-DevKitC | 1 |
| NPN General-Purpose Transistor | BC547 | 4 |
| Schottky Rectifier Diode | SR560 | 4 |
| General-Purpose Rectifier Diode | 1N4007 | 4 |
| Bridge Rectifier Module | RS505 | 1 |
| SPDT Relay — 5 V Coil | SANYOU SRD Series Form-C | 3 |
| Fuse Holder + Fuse — 5×20 mm | 5 A / 250 V slow-blow (or as per load; see notes) | 2 |
| Momentary Push Button (Reset / Boot) | 6 mm × 6 mm tactile push button, SPST-NO | 1 |
| DC-DC Buck Converter Module (12 V → 5 V) | LM2596 or MP1584 based | 1 |

---

*Generated from KiCad schematic. Cross-check with the latest `.kicad_sch` file before ordering.*
