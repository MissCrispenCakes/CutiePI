# Bill of Materials — CutiePI Bear (×2)

> **This BOM is identical for both build environments** (`bear` ESP-NOW and `bear-mqtt` MQTT).
> The difference between the two builds is software only — no extra hardware is needed for internet connectivity beyond the ESP32-S3's built-in WiFi.


| Qty | Component | Notes |
|-----|-----------|-------|
| 1 | Waveshare ESP32-S3-Touch-LCD-1.46B | Main microcontroller (ESP32-S3R8: 16 MB flash, 8 MB PSRAM, 1.46" round display) |
| 1 | Force-Sensitive Resistor (FSR 402 or similar) **with 2-pin JST-PH connector** | Hug detection; JST pins are the two FSR terminals — wire into voltage divider on the board |
| 1 | Red LED (5 mm, ~2V Vf) **with 2-pin JST-PH connector** | Heartbeat glow; JST red → board-side 100 Ω resistor → GPIO13, JST black → GND |
| 1 | 100 Ω resistor | Current limiter for LED (on the board, between GPIO13 and the LED JST red wire) |
| 1 | Heating pad (5 V, ≤500 mA) **with 2-pin JST-PH connector** | Warmth; JST red → board 5V pin (see ⚠ note in wiring section), JST black → MOSFET drain |
| 1 | TMP36 analog temperature sensor **with 2-pin JST-PH connector** | Heater temperature safety; VOUT wired directly to GPIO1 — no voltage divider needed |
| 1 | N-channel MOSFET (e.g. 2N7000 or IRLZ44N) | Controls heater from 3.3 V GPIO |
| 1 | 1 kΩ resistor | MOSFET gate resistor |
| 1 | 1N4007 flyback diode | Across heater JST terminals on the board (cathode toward 5 V) |
| 1 | 10 kΩ resistor | FSR voltage divider pull-down |
| 1 | LiPo battery (1S, 1000–2000 mAh) with JST-PH 2-pin connector | Portable power — plugs directly into the board's onboard battery port; no separate charger board needed. Charging is handled via USB-C on the board. |
| — | 2-pin JST-PH connectors + crimps (or pre-crimped pigtails) | For LED, FSR, heater pad, and TMP36 |
| — | Hookup wire, heat-shrink tubing, hot glue | Assembly |

## Wiring Summary

All four external components (FSR, LED, heater pad, TMP36) connect via **2-pin JST-PH** (red = +/signal, black = GND).
The voltage dividers, current-limiting resistor, and MOSFET circuit all live on the board/breadboard side of each connector.

```
── Pressure sensor (FSR) ──────────────────────────────────────
3.3 V ──── [JST red] ──── FSR ──── [JST black] ──── GPIO3 (ADC)
                                                         │
                                                       10 kΩ
                                                         │
                                                        GND
  GPIO3 ADC (ADC1_CH2) is 12-bit (0–4095). PRESSURE_THRESHOLD in config.h defaults
  to 500, meaning the FSR needs to drop the voltage divider below ~0.4 V to register
  a hug.

── LED ────────────────────────────────────────────────────────
GPIO13 (PWM) ─── 100 Ω ─── [JST red / LED anode]
                            [JST black / LED cathode] ─── GND

── Temperature sensor (TMP36) ─────────────────────────
3.3 V ──── TMP36 pin 1 (VS)
GPIO1 ──── TMP36 pin 2 (VOUT)
GND   ──── TMP36 pin 3 (GND)
  No voltage divider or pull-up needed — VOUT connects directly to GPIO1.
  Place the TMP36 against or inside the heating pad so it reads pad temperature.
  Sew/tape it in place so it can't shift during a hug.

── Heating pad ────────────────────────────────────────────────
GPIO12 ─── 1 kΩ ─── MOSFET gate
                     MOSFET source ──────────────────── GND
                     MOSFET drain  ─── [JST black]
                     [JST red]     ─── 5 V pin (see ⚠ below)
                     1N4007 flyback diode across JST pins on board
                     (diode cathode toward 5 V / JST red)

⚠ VERIFY THE 5V PIN BEFORE ASSEMBLING
The board's external header has a 5V pin labelled "VBUS / VSYS".
On USB power this is always 5V. On battery it may be dead if the pin is
VBUS only (no onboard boost converter).
Before soldering: plug in the LiPo with USB disconnected and measure the
5V header pin with a multimeter.
  - Reads ~5V → safe to use for the heater.
  - Reads 0V  → the pin is VBUS only. Options:
      (a) add a small 5V boost module (e.g. MT3608) between the battery
          and the heater rail, or
      (b) swap to a 3.3V-rated heating pad and wire it to the 3.3V pin.
```

## GPIO Pins Reserved by the Waveshare ESP32-S3-Touch-LCD-1.46B

The following GPIO pins are used internally by the board's onboard hardware and **must not be used** for external wiring:

| GPIO | Used by |
|------|---------|
| 2 | Microphone (I2S WS) |
| 4 | Touch panel interrupt (TP_INT) |
| 5 | LCD backlight (LCD_BL) |
| 6 | Power button input (PWR_KEY) — firmware use only, see `power_mgr.cpp` |
| 7 | Board power latch (PWR_CTRL) — must stay HIGH or board shuts off |
| 8 | Battery ADC (BAT_ADC) — 1:3 voltage divider to battery+ |
| 9 | RTC interrupt |
| 10 | I2C SCL (touch/IMU/RTC) |
| 11 | I2C SDA (touch/IMU/RTC) |
| 14 | SD card clock |
| 15 | Microphone clock (I2S SCK) |
| 16 | SD card MISO |
| 17 | SD card MOSI |
| 18 | LCD TE signal |
| 19 | USB D− |
| 20 | USB D+ |
| 21 | LCD chip-select |
| 38 | Speaker LRCK (I2S) |
| 39 | Microphone data (I2S SD) |
| 40 | LCD QSPI clock |
| 41–42, 45–46 | LCD QSPI data lines |
| 47 | Speaker DIN (I2S) |
| 48 | Speaker BCK (I2S) |
| 26–32 | Internal flash/PSRAM (never use) |

**Free user GPIO available on the board's external header: GPIO1, GPIO3, GPIO12, GPIO13.**

---

## Sensor Selection — FSR vs. Capacitive Touch Pad

The firmware supports two hug-detection methods. Choose based on what you have available.
Toggle by uncommenting `#define USE_TOUCH_SENSOR` in `firmware/include/config.h`.

### Option A — FSR voltage divider (default, recommended)

| | |
|---|---|
| **Hardware** | FSR 402 + 10 kΩ pull-down resistor (see wiring above) |
| **How it works** | Squeezing the bear lowers the FSR resistance, raising the ADC voltage on GPIO3 |
| **Strengths** | Immune to WiFi/RF interference; measures actual squeeze force through stuffing |
| **Weaknesses** | Needs two extra components; threshold may need tuning per FSR batch |

### Option B — ESP32-S3 capacitive touch pad

| | |
|---|---|
| **Hardware** | Conductive pad (copper tape or conductive fabric) wired to GPIO3 (= touch channel T3) |
| **How it works** | Hand contact/proximity on the pad changes capacitance; firmware reads `touchRead(GPIO3)` |
| **Strengths** | No resistors needed; single wire from GPIO3 to the pad; simpler assembly |
| **Weaknesses** | See warning below |

> **⚠ Known interference problem with this project**
>
> The ESP32-S3 capacitive touch peripheral is sensitive to RF noise from the WiFi radio.
> Because this project uses ESP-NOW (WiFi-based), every time a bear sends or receives a
> hug message the touch readings spike. This can cause false hug detections or missed
> releases.
>
> The firmware mitigates this by averaging 4 reads per sample, but it cannot eliminate
> the interference entirely. The 200 ms debounce in `config.h` (`PRESSURE_DEBOUNCE_MS`)
> helps — **raise it to 500 ms or more** if you see phantom triggers.
>
> Additionally, touch calibration happens at boot (`sensors_init()`). **Do not hold or
> touch the bear during power-on** or the baseline will be set wrong and the sensor will
> either never trigger or trigger constantly.
>
> If you have an FSR available, use Option A. Use Option B only if you don't have an FSR
> and are comfortable tuning the sensitivity.

### Touch pad wiring (Option B only)

```
GPIO3 (T3) ──── conductive pad sewn or taped inside the bear
                (no other connections needed — no power, no GND, no resistors)
```

Remove the FSR and the 10 kΩ resistor from your build entirely when using Option B.

---

## Power Management (LiPo battery)

The Waveshare board has an onboard battery circuit that the firmware must interact with. **Three GPIOs are reserved for this — do not wire anything to them.**

| GPIO | Role | Firmware behavior |
|------|------|-------------------|
| 7 (PWR_CTRL) | Power latch | Must be driven HIGH within ~100 ms of boot or the board shuts itself off. `power_mgr_init()` does this as the very first thing in `setup()`. |
| 6 (PWR_KEY) | Power button | Read each loop. Holding for 3 s triggers a graceful shutdown (GPIO7 goes LOW). |
| 8 (BAT_ADC) | Battery voltage | 1:3 voltage divider; `analogReadMilliVolts(8) × 3 / 1000` gives battery voltage in volts. |

### LiPo wiring

Connect the LiPo **JST-PH 2-pin** connector directly to the board's onboard battery port. The board's built-in charge circuit handles charging via USB-C automatically — no separate charger board is needed.

> ⚠ **Check JST polarity before plugging in.** Many generic LiPo batteries have reversed polarity compared to Waveshare boards (red = positive, black = GND is correct). Plugging in backwards can damage the board instantly. Confirm with a multimeter: the red wire from the battery should read positive voltage relative to black before inserting the connector.

Battery voltage is logged to Serial every 60 seconds:
```
Battery: 3.94 V (78%)
```
It is also printed at startup (USB required to see serial output — see boot note below).

### Booting on battery vs USB

| Power source | How to boot |
|---|---|
| USB-C | Board boots automatically when cable is plugged in |
| LiPo (no USB) | Press and hold the power button until the display lights up |

Serial output via `pio device monitor` requires a USB connection (the board uses USB CDC, not a USB-UART chip). On battery-only power the bear operates normally but produces no serial output.

### Battery capacity

With a 1100 mAh 1S LiPo and a 5 V heating pad drawing ~500 mA:
- Heating pad needs ~5 V at up to 500 mA; the board's boost converter (~87% efficient) draws ~800 mA from the 3.7 V cell when the heater is on.
- In a typical mutual-hug session (heater on ~50% of the time via temperature cycling), current draw is roughly 450–500 mA total.
- **Estimated runtime: ~2 hours** on a 1100 mAh cell, which is comfortable for testing.

For long-term daily use, a 2000 mAh cell doubles that to ~4 hours.

---

## Safety Notes

- The heating pad **must** be rated for 5 V / ≤500 mA.
- Mount the TMP36 **against the heating pad surface** so it reads pad temperature — not floating in stuffing where it can't detect hot spots.
- The firmware uses three stacked safety layers: temperature hysteresis cycling (42 °C target), over-temperature hard cutoff (48 °C), and a session time cutoff (5 min). All are tunable in `config.h`. See the Safety section in the main README for details.
- Use heat-resistant material between the heating pad and stuffing to prevent hot spots.
