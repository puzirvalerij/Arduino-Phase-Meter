# Arduino Phase Meter

An Arduino UNO-based device for measuring the phase shift between voltage and current signals.

The measured parameters are displayed on a **16x2 I2C LCD**:

* Phase shift angle
* Load type — `L` or `C`
* Current signal frequency
* Power factor `cos φ`

## Features

* 📐 Phase shift measurement: **−180° to +180°**
* ⚡ Load type detection:

  * `L` — inductive
  * `C` — capacitive
* 📊 `cos φ` calculation
* 📡 Current signal frequency measurement in Hz
* 📺 16x2 I2C LCD display
* 🚫 Current signal loss detection
* 🔌 0–5 V rectangular input signals
* 🟢 Startup screen with firmware version

## Hardware

### Required Components

| Component             |    Quantity |
| --------------------- | ----------: |
| Arduino UNO           |           1 |
| 16x2 I2C LCD          |           1 |
| Voltage signal source |           1 |
| Current signal source |           1 |
| Connecting wires      | As required |

## Connections

| Signal         | Arduino Pin |
| -------------- | ----------- |
| Voltage signal | D2          |
| Current signal | D3          |
| LCD SDA        | A4          |
| LCD SCL        | A5          |
| LCD VCC        | 5V          |
| LCD GND        | GND         |

### Input Signals

The project expects rectangular logic-level signals:

* **Voltage signal:** 0–5 V → `D2`
* **Current signal:** 0–5 V → `D3`

> ⚠️ **Safety warning:** Do not connect mains voltage or current directly to the Arduino.
> Input signals must be properly isolated and conditioned to safe 0–5 V logic levels.

## Display

### Startup

```text
PHASE METER
V1.0
```

### Inductive Load

```text
FI:+30.0° L
F:50.0Hz φ:0.866
```

### Capacitive Load

```text
FI:-30.0° C
F:50.0Hz φ:0.866
```

### No Current Signal

```text
FI:0.0°
F:0.0Hz φ:1.000
```

## Measurement Principle

The phase shift is calculated using the time difference between the rising edges of the voltage and current signals.

The Arduino measures:

1. The rising edge timestamp of the voltage signal on `D2`.
2. The rising edge timestamp of the current signal on `D3`.
3. The time difference between the two signals.
4. The current signal period.
5. The phase shift angle.

The phase angle is calculated using:

```text
φ = 360° × Δt / T
```

Where:

* `φ` — phase shift angle
* `Δt` — time difference between voltage and current signals
* `T` — signal period

The power factor is calculated as:

```text
cos(φ)
```

## Phase Convention

The project uses the following convention:

|      Phase Angle | Load Type        |
| ---------------: | ---------------- |
|         Positive | `L` — Inductive  |
|         Negative | `C` — Capacitive |
| Approximately 0° | In Phase         |

Examples:

```text
+30° → L
-30° → C
  0° → In Phase
```

## Software

### Requirements

* Arduino IDE
* Arduino UNO
* `LiquidCrystal_I2C` library

### Installation

1. Clone or download this repository.
2. Open the firmware file:

```text
firmware/V1.0/phase_meter_V1_0.ino
```

3. Install the `LiquidCrystal_I2C` library.
4. Connect the Arduino UNO.
5. Select the board:

```text
Arduino UNO
```

6. Select the correct COM port.
7. Upload the firmware.

## Project Structure

```text
Arduino-Phase-Meter/
│
├── firmware/
│   └── V1.0/
│       └── phase_meter_V1_0.ino
│
├── README.md
│
└── LICENSE
```

## Version

**Current Version: V1.0**

### V1.0 — Initial Release

Implemented:

* Phase shift measurement
* `L/C` load indication
* Frequency measurement
* `cos φ` calculation
* 16x2 I2C LCD support
* Current signal loss detection
* Startup screen

## Future Improvements

Planned features and improvements:

* [ ] Improve measurement stability
* [ ] Add signal filtering
* [ ] Add phase offset calibration
* [ ] Add voltage RMS measurement
* [ ] Add current RMS measurement
* [ ] Add active power measurement
* [ ] Add apparent power measurement
* [ ] Add energy consumption measurement
* [ ] Improve LCD interface
* [ ] Support additional display types



⭐ If you find this project useful, consider giving it a star!

