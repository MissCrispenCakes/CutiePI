# CutiePI

Two stuffed animals connected over the air. Hug one and the other bear's LED pulses with a red heartbeat. Hug both at the same time and they warm up too — like sending a hug through the mail.

Each bear contains an ESP32-S3, a force-sensitive resistor, a red LED, a small heating pad, and a temperature sensor for heater safety. Two builds are available depending on where the bears will live:

| Build | How they talk | Range |
|-------|--------------|-------|
| `bear` (default) | ESP-NOW — direct peer-to-peer WiFi, no router | Same room / building |
| `bear-mqtt` | MQTT over internet via a shared broker | Anywhere in the world |

---

## How It Works

| Who is hugging | What happens |
|----------------|--------------|
| Neither bear | Both bears are quiet |
| Bear A only | Bear B's LED pulses a red heartbeat |
| Bear B only | Bear A's LED pulses a red heartbeat |
| Both bears | Both LEDs pulse **and** both heating pads warm up |

The firmware is identical on both bears — only `BEAR_ID` and network credentials differ.

---

## Repository Layout

```
CutiePI/
├── firmware/
│   ├── platformio.ini        # Two build environments: bear (ESP-NOW) and bear-mqtt (MQTT)
│   ├── include/
│   │   └── config.h          # All pins, thresholds, BPM, safety limits, WiFi/MQTT credentials
│   ├── src/
│   │   ├── main.cpp              # State machine + main loop
│   │   ├── comms.{h,cpp}         # ESP-NOW or MQTT — selected at build time
│   │   ├── sensors.{h,cpp}       # Pressure sensor reading + debounce
│   │   ├── heartbeat.{h,cpp}     # Lub-dub PWM LED animation
│   │   ├── actuators.{h,cpp}     # Heating pad control + temperature cycling + safety cutoff
│   │   ├── temp_sensor.{h,cpp}   # TMP36 analog temperature sensor read
│   │   └── power_mgr.{h,cpp}     # Battery voltage + power button + board power latch
│   └── fix/
│       ├── README.md                  # When and how to use these variants
│       ├── heartbeat_3x.cpp           # Arduino ESP32 3.x LEDC API
│       ├── comms_3x.cpp               # Arduino ESP32 3.x (ESP-NOW callback + full MQTT support)
│       ├── actuators_usb_heater.cpp   # P-channel MOSFET for USB heating pad
│       └── heartbeat_led_strip.cpp    # WS2812B addressable LED strip
└── hardware/
    └── bom.md                 # Parts list, wiring diagram, and sensor selection guide
```

---

## Hardware

Full parts list and wiring diagram: [`hardware/bom.md`](hardware/bom.md)

Quick overview per bear:

- **Waveshare ESP32-S3-Touch-LCD-1.46B** — microcontroller (ESP32-S3R8, 16 MB flash, 8 MB PSRAM, 1.46" round display)
- **FSR 402** (or similar) — force-sensitive resistor for hug detection
- **Red LED** + 100 Ω resistor — heartbeat glow
- **5 V flexible heating pad** (≤500 mA) + N-channel MOSFET — warmth
- **TMP36** analog temperature sensor — heater temperature safety
- **LiPo battery** + TP4056 charger board — portable power

Default pin assignments (change in `config.h`):

| Signal | GPIO | Notes |
|--------|------|-------|
| Pressure sensor (ADC / touch) | 3 | ADC1_CH2 / touch T3 |
| LED (PWM) | 13 | LEDC PWM |
| Heater (MOSFET gate) | 12 | digital output |
| Temperature sensor (ADC) | 1 | ADC1_CH0 |

GPIO 6, 7, and 8 are reserved by the board's power management hardware (power button, power latch, battery ADC). See `hardware/bom.md` for the full reserved-pin table.

---

## Dependencies

The ESP-NOW build (`bear`) has **no external libraries** — everything ships inside the `espressif32` Arduino core.

The MQTT build (`bear-mqtt`) adds one library, downloaded automatically by PlatformIO:

| Library | Version |
|---------|---------|
| `knolleary/PubSubClient` | ^2.8 |

Both builds share the same core APIs:

| Header | Source |
|--------|--------|
| `Arduino.h` | Arduino framework (bundled with platform) |
| `WiFi.h` | ESP32 Arduino WiFi library (bundled with platform) |
| LEDC / ADC / Touch | ESP32 Arduino peripheral drivers (bundled with platform) |

The platform version is pinned in `platformio.ini`. PlatformIO downloads everything once on the first `pio run`; all subsequent builds work **fully offline** (ESP-NOW build only — MQTT requires internet at runtime).

---

## Coming from another setup?

If you're joining this project with existing hardware — for example a heating pad already wired to an Arduino Nano for interim testing — here's what changes:

- **Swap the Nano for a Waveshare ESP32-S3-Touch-LCD-1.46B.** The Arduino code won't port directly; use this repo instead.
- **The ESP32-S3 runs at 3.3 V.** You cannot drive a heating pad directly from a GPIO pin. You need the MOSFET circuit in `hardware/bom.md` — the same one your Nano demo likely uses, just wired to GPIO12 instead of whatever pin you had before.
- **Several GPIO pins are reserved by onboard hardware** (display, touch, IMU, speaker, mic, and power management). This project uses GPIO1 (TMP36 temp sensor), GPIO3 (pressure sensor), GPIO12 (heater), and GPIO13 (LED). GPIO6/7/8 are owned by the board's battery circuit. See the full reserved-pin table in `hardware/bom.md`.
- **PlatformIO replaces the Arduino IDE.** Install the VS Code extension, clone this repo, and `pio run` handles everything. You don't need to manually install board support or libraries.
- **Two bears need to know each other's MAC address** before they can communicate. Flash first with the placeholder config, grab the MAC from the serial monitor, then reflash with the real address. The README steps below walk through this.

Everything else — pressure sensor, LED, heating pad logic — works the same way you'd expect from Arduino. The full wiring diagram is in [`hardware/bom.md`](hardware/bom.md).

---

## Setup

### Prerequisites

- **PlatformIO** — pick whichever install method suits you:
  - VS Code extension (recommended): [marketplace.visualstudio.com/items?itemName=platformio.platformio-ide](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)
  - CLI only: [docs.platformio.org/en/latest/core/installation](https://docs.platformio.org/en/latest/core/installation/index.html)
- Two Waveshare ESP32-S3-Touch-LCD-1.46B boards
- Hardware assembled per `hardware/bom.md`

---

### Option A — ESP-NOW (same location)

#### Step 1 — Discover MAC addresses

Flash both boards with the default config (MACs left as `0xFF`) and open the serial monitor at 115200 baud. Each bear prints its MAC on boot:

```
This bear's MAC: AA:BB:CC:DD:EE:FF
```

Note both addresses.

#### Step 2 — Configure each bear

Open `firmware/include/config.h` and set:

```cpp
// Bear A
#define BEAR_ID 0
static const uint8_t PEER_MAC[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}; // Bear B's MAC
```

```cpp
// Bear B
#define BEAR_ID 1
static const uint8_t PEER_MAC[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66}; // Bear A's MAC
```

#### Step 3 — Build and flash

```bash
cd firmware

# Bear A
pio run -e bear --target upload --upload-port /dev/ttyUSB0

# Bear B
pio run -e bear --target upload --upload-port /dev/ttyUSB1
```

---

### Option B — MQTT (long distance)

#### Step 1 — Choose a broker

For testing, the free public broker `broker.hivemq.com` requires no account. For a permanent setup, consider a private broker (self-hosted Mosquitto, HiveMQ Cloud free tier, etc.).

#### Step 2 — Configure each bear

Create a local credentials file from the example (this file is gitignored — safe to put real values in):

```bash
cp firmware/include/secrets.h.example firmware/include/secrets.h
```

Edit `secrets.h` with your network and broker details:

```cpp
#define WIFI_SSID     "your_network_name"
#define WIFI_PASSWORD "your_network_password"
#define MQTT_BROKER   "broker.hivemq.com"  // or your own broker
#define MQTT_PORT     1883
```

Then set `BEAR_ID` in `firmware/include/config.h` (0 for Bear A, 1 for Bear B).

Each bear automatically publishes to `cutiepie/BEAR_ID/hug` and subscribes to the other bear's topic — no manual topic configuration needed.

#### Step 3 — Build and flash

```bash
cd firmware

# Bear A
pio run -e bear-mqtt --target upload --upload-port /dev/ttyUSB0

# Bear B
pio run -e bear-mqtt --target upload --upload-port /dev/ttyUSB1
```

---

### Step 4 — Test before assembly

Use the component snippets in `firmware/snippets/` to verify each part before sewing anything up — especially the TMP36:

1. Flash `test_temp.cpp` with the TMP36 VOUT wired to GPIO1. Confirm it reads a plausible room temperature (~20–25 °C) and rises when you hold the sensor body. The serial output labels each reading against the 40/42/48 °C thresholds so you can verify the safety limits are reachable.
2. Flash `test_all.cpp` with everything connected. Hold the FSR and watch the heater cycle on and off as temperature crosses 40–42 °C.

Then flash the full firmware and confirm:
- Squeezing one bear's FSR makes the other bear's LED pulse
- Squeezing both FSRs simultaneously activates both LEDs and both heaters

Then sew everything inside the bears.

---

## Customization

All tunable parameters live in `firmware/include/config.h`:

| Parameter | Default | Effect |
|-----------|---------|--------|
| `PRESSURE_THRESHOLD` | `500` | ADC value (0–4095) that counts as a hug. Lower = more sensitive. |
| `PRESSURE_DEBOUNCE_MS` | `200` | Milliseconds before a press/release is registered. |
| `HEARTBEAT_BPM` | `70` | Speed of the LED heartbeat animation. |
| `HEATER_MAX_ON_MS` | `300000` (5 min) | Session time cutoff — backup if temp sensor disconnects. |
| `TEMP_MAX_SAFE_C` | `48.0` | Hard over-temperature shutoff. Re-hug to re-enable. |
| `TEMP_TARGET_C` | `42.0` | Comfort temperature target; heater cycles off here. |
| `TEMP_HYSTERESIS_C` | `2.0` | Heater cycles back on below `TARGET − HYSTERESIS` (40 °C default). |

---

## Roadmap

- **Captive portal setup** — on first boot (or when PEER_MAC is still placeholder), the bear starts a temporary WiFi access point. Connect on your phone and a simple webpage shows the MAC address, current config, and connection status. Eliminates the need to open a serial monitor during setup.

---

## Safety

### Heater safety — three independent layers

| Layer | Mechanism | Default |
|-------|-----------|---------|
| **Temperature cycling** | Heater turns off at `TEMP_TARGET_C`, back on below `TEMP_TARGET_C − TEMP_HYSTERESIS_C` | 42 °C target, 40 °C lower band |
| **Over-temperature cutoff** | Hard shutoff above `TEMP_MAX_SAFE_C`; re-hug to re-enable | 48 °C |
| **Session time cutoff** | Hard shutoff after `HEATER_MAX_ON_MS` regardless of temperature; re-hug to re-enable. Acts as a backup if the thermistor is disconnected | 5 min |

All three are enforced inside `actuators_update()`, called every `loop()`. Releasing a hug always turns the heater off immediately regardless of temperature.

### Hardware requirements

- The heating pad must be rated **5 V / ≤500 mA**.
- Mount the TMP36 **against the heating pad surface**, not floating in stuffing — it needs to read pad temperature, not air temperature.
- Use heat-resistant material between the heating pad and stuffing to prevent hot spots.
- Never leave the bears unattended while charging.
