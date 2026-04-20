# CutiePI

Two stuffed animals connected over the air. Hug one and the other bear's LED pulses with a red heartbeat. Hug both at the same time and they warm up too — like sending a hug through the mail.

Each bear contains an ESP32-S3, a force-sensitive resistor, a red LED, and a small heating pad. The two bears talk directly to each other over **ESP-NOW** (peer-to-peer WiFi, no router required).

---

## How It Works

| Who is hugging | What happens |
|----------------|--------------|
| Neither bear | Both bears are quiet |
| Bear A only | Bear B's LED pulses a red heartbeat |
| Bear B only | Bear A's LED pulses a red heartbeat |
| Both bears | Both LEDs pulse **and** both heating pads warm up |

The firmware is identical on both bears — only `BEAR_ID` differs.

---

## Repository Layout

```
CutiePI/
├── firmware/
│   ├── platformio.ini        # PlatformIO project config (ESP32-S3, Arduino)
│   ├── include/
│   │   └── config.h          # All pins, thresholds, BPM, safety limits
│   ├── src/
│   │   ├── main.cpp           # State machine + main loop
│   │   ├── comms.{h,cpp}      # ESP-NOW peer-to-peer communication
│   │   ├── sensors.{h,cpp}    # Pressure sensor reading + debounce
│   │   ├── heartbeat.{h,cpp}  # Lub-dub PWM LED animation
│   │   └── actuators.{h,cpp}  # Heating pad control + safety cutoff
│   └── fix/
│       ├── README.md                  # When and how to use these variants
│       ├── heartbeat_3x.cpp           # Arduino ESP32 3.x LEDC API
│       ├── comms_3x.cpp               # Arduino ESP32 3.x ESP-NOW callback
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

This project has **no external libraries**. Every API used ships inside the `espressif32` Arduino core:

| Header | Source |
|--------|--------|
| `Arduino.h` | Arduino framework (bundled with platform) |
| `esp_now.h` | ESP-IDF ESP-NOW driver (bundled with platform) |
| `WiFi.h` | ESP32 Arduino WiFi library (bundled with platform) |
| LEDC / ADC | ESP32 Arduino peripheral drivers (bundled with platform) |

The platform version is pinned in `platformio.ini`. PlatformIO downloads it once on the first `pio run`; all subsequent builds work **fully offline**.

---

## Setup

### Prerequisites

- **PlatformIO** — pick whichever install method suits you:
  - VS Code extension (recommended): [marketplace.visualstudio.com/items?itemName=platformio.platformio-ide](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)
  - CLI only: [docs.platformio.org/en/latest/core/installation](https://docs.platformio.org/en/latest/core/installation/index.html)
- Two ESP32-S3 DevKitC-1 boards
- Hardware assembled per `hardware/bom.md`

### Step 1 — Discover MAC addresses

Each bear needs to know the other's WiFi MAC address. Flash both boards with the default config (MACs left as `0xFF`) and open the serial monitor at 115200 baud. On boot, each bear prints:

```
This bear's MAC: AA:BB:CC:DD:EE:FF
```

Note both addresses.

### Step 2 — Configure each bear

Open `firmware/include/config.h` and set:

```cpp
// On Bear A (BEAR_ID 0): paste Bear B's MAC here
#define BEAR_ID 0
static const uint8_t PEER_MAC[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
```

```cpp
// On Bear B (BEAR_ID 1): paste Bear A's MAC here
#define BEAR_ID 1
static const uint8_t PEER_MAC[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
```

### Step 3 — Build and flash

```bash
cd firmware

# Flash Bear A (set BEAR_ID=0 in config.h first)
pio run --target upload --upload-port /dev/ttyUSB0

# Flash Bear B (set BEAR_ID=1 in config.h first)
pio run --target upload --upload-port /dev/ttyUSB1
```

Or use the PlatformIO sidebar in VS Code: **Build → Upload**.

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

## Safety

- The heating pad must be rated **5 V / ≤500 mA**.
- The firmware cuts the heater off automatically after `HEATER_MAX_ON_MS` — a re-hug is required to re-enable it.
- Place heat-resistant material between the heating pad and stuffing to prevent hot spots.
- Never leave the bears unattended while charging.
