# CutiePI

Two stuffed animals connected over the air. Hug one and the other bear's LED pulses with a red heartbeat. Hug both at the same time and they warm up too — like sending a hug through the mail.

Each bear contains an ESP32-S3, a force-sensitive resistor, a red LED, and a small heating pad. Two builds are available depending on where the bears will live:

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
│   │   ├── main.cpp           # State machine + main loop
│   │   ├── comms.{h,cpp}      # ESP-NOW or MQTT — selected at build time
│   │   ├── sensors.{h,cpp}    # Pressure sensor reading + debounce
│   │   ├── heartbeat.{h,cpp}  # Lub-dub PWM LED animation
│   │   └── actuators.{h,cpp}  # Heating pad control + safety cutoff
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

- **ESP32-S3 DevKitC-1** — microcontroller
- **FSR 402** (or similar) — force-sensitive resistor for hug detection
- **Red LED** + 100 Ω resistor — heartbeat glow
- **5 V flexible heating pad** (≤500 mA) + N-channel MOSFET — warmth
- **LiPo battery** + TP4056 charger board — portable power

Default pin assignments (change in `config.h`):

| Signal | GPIO |
|--------|------|
| Pressure sensor (ADC) | 4 |
| LED (PWM) | 5 |
| Heater (MOSFET gate) | 6 |

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

- **Swap the Nano for an ESP32-S3 DevKitC-1.** The Arduino code won't port directly; use this repo instead.
- **The ESP32-S3 runs at 3.3 V.** You cannot drive a heating pad directly from a GPIO pin. You need the MOSFET circuit in `hardware/bom.md` — the same one your Nano demo likely uses, just wired to GPIO6 instead of whatever pin you had before.
- **PlatformIO replaces the Arduino IDE.** Install the VS Code extension, clone this repo, and `pio run` handles everything. You don't need to manually install board support or libraries.
- **Two bears need to know each other's MAC address** before they can communicate. Flash first with the placeholder config, grab the MAC from the serial monitor, then reflash with the real address. The README steps below walk through this.

Everything else — pressure sensor, LED, heating pad logic — works the same way you'd expect from Arduino. The full wiring diagram is in [`hardware/bom.md`](hardware/bom.md).

---

## Setup

### Prerequisites

- **PlatformIO** — pick whichever install method suits you:
  - VS Code extension (recommended): [marketplace.visualstudio.com/items?itemName=platformio.platformio-ide](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)
  - CLI only: [docs.platformio.org/en/latest/core/installation](https://docs.platformio.org/en/latest/core/installation/index.html)
- Two ESP32-S3 DevKitC-1 boards
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

Hold each bear's FSR above the pressure threshold and confirm:
- The other bear's LED starts pulsing
- Pressing both FSRs simultaneously activates both LEDs and both heaters

Then sew everything inside the bears.

---

## Customization

All tunable parameters live in `firmware/include/config.h`:

| Parameter | Default | Effect |
|-----------|---------|--------|
| `PRESSURE_THRESHOLD` | `500` | ADC value (0–4095) that counts as a hug. Lower = more sensitive. |
| `PRESSURE_DEBOUNCE_MS` | `200` | Milliseconds before a press/release is registered. |
| `HEARTBEAT_BPM` | `70` | Speed of the LED heartbeat animation. |
| `HEATER_MAX_ON_MS` | `300000` (5 min) | Hard safety cutoff for the heating pad. |

---

## Roadmap

- **Captive portal setup** — on first boot (or when PEER_MAC is still placeholder), the bear starts a temporary WiFi access point. Connect on your phone and a simple webpage shows the MAC address, current config, and connection status. Eliminates the need to open a serial monitor during setup.

---

## Safety

- The heating pad must be rated **5 V / ≤500 mA**.
- The firmware cuts the heater off automatically after `HEATER_MAX_ON_MS` — a re-hug is required to re-enable it.
- Place heat-resistant material between the heating pad and stuffing to prevent hot spots.
- Never leave the bears unattended while charging.
