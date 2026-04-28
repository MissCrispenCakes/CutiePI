# Bill of Materials — CutiePI Bear (×2)

> **This BOM is identical for both build environments** (`bear` ESP-NOW and `bear-mqtt` MQTT).
> The difference between the two builds is software only — no extra hardware is needed for internet connectivity beyond the ESP32-S3's built-in WiFi.


| Qty | Component | Notes |
|-----|-----------|-------|
| 1 | Waveshare ESP32-S3-Touch-LCD-1.46B | Current test board (ESP32-S3R8: 16 MB flash, 8 MB PSRAM, 1.46" round display) |
| 1 | Force-Sensitive Resistor (FSR 402 or similar) | Hug detection; wire into voltage divider — see wiring section |
| 1 | Red LED (5 mm, ~2V Vf) | Heartbeat glow; 100 Ω series resistor → GPIO13 |
| 1 | 100 Ω resistor | Current limiter for LED |
| 1 | SparkFun Heating Pad 5×10 cm | Warmth; ~6.5 Ω, rated 5V/750mA (runs at LiPo voltage ~3.7V → ~570mA, ~2.1W); wire leads direct to MOSFET board output |
| 1 | TMP36 analog temperature sensor | Heater temperature safety; VOUT wired directly to GPIO1 — no voltage divider needed |
| 1 | SparkFun MOSFET Power Controller | Switches heater from GPIO12; FDS6630A (logic-level, 3.3V gate OK, 6.5A max); built-in gate resistor, flyback diode, and 10kΩ pull-down (heater stays off when GPIO floats) |
| 1 | 10 kΩ resistor | FSR voltage divider pull-down |
| 1 | LiPo battery (1S, 1500–2500 mAh) **with MX1.25 2-pin connector** | Portable power — plugs directly into the board's onboard MX1.25 battery port; no separate charger needed. Charging via USB-C on the board. |
| 1 | MX1.25 Y-splitter cable (1× female → 2× male) | Splits LiPo between ESP32 battery port and MOSFET board power input |
| — | JST-PH connectors + crimps (or pre-crimped pigtails) | For LED, FSR, and TMP36 wiring harness (not battery — battery uses MX1.25) |
| — | Hookup wire, heat-shrink tubing, hot glue | Assembly |

### Optional components

| Qty | Component | Purpose |
|-----|-----------|---------|
| 1 | 5V boost converter module ≥1A out (e.g. MT3608) | Run heater at full rated 5V/750mA instead of LiPo voltage — more heat, shorter battery life |
| 1 | 470 µF electrolytic capacitor | Across heater power branch — smooths inrush current when MOSFET switches on |
| 1 | Inline polyfuse or fuse (750mA–1A) | Heater branch overcurrent protection |
| — | Heat-resistant fabric pocket or insulating sheet | Between heating pad and stuffing — prevents hot spots |

## Wiring Summary

FSR, LED, and TMP36 connect via **2-pin JST-PH** (red = +/signal, black = GND).
The heating pad connects directly to the SparkFun MOSFET board output — no JST connector on the pad side.
The LiPo battery uses **MX1.25** (different pitch from JST-PH — do not mix up).

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

── Heating pad (via SparkFun MOSFET Power Controller) ─────────
LiPo (+) ──→ MOSFET board VIN   (solder direct or via JST)
LiPo (−) ──→ MOSFET board GND

GPIO12   ──→ MOSFET board control +   (0.1" header or sewable pad)
GND      ──→ MOSFET board control −   (0.1" header or sewable pad)

MOSFET board OUT+ ──→ heating pad red wire
MOSFET board OUT− ──→ heating pad black wire

  The SparkFun board's built-in 10kΩ pull-down holds the gate LOW when
  the ESP32 is off or GPIO12 is floating — heater cannot switch on
  accidentally. Gate resistor and flyback diode are also built in.

  The LiPo splits to the ESP32 and the MOSFET board via a MX1.25
  Y-splitter cable. The heater is powered at battery voltage (~3.7V
  nominal) rather than 5V, drawing ~570mA and producing ~2.1W of warmth.
  The TMP36 safety cutoffs apply regardless of supply voltage.

── Optional boost converter (for full-rated heater power) ─────
LiPo (+) ──→ boost converter IN+
LiPo (−) ──→ boost converter IN−
Boost 5V out ──→ MOSFET board VIN   (replaces direct LiPo feed above)
Boost GND    ──→ MOSFET board GND
470µF cap across MOSFET board VIN/GND (smooths switching inrush)
Polyfuse (750mA–1A) in series on the VIN line

  With boost: heater runs at 5V/750mA/3.75W — full rated power.
  Without boost: heater runs at ~3.7V/570mA/2.1W — still warm, longer
  battery life.
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

Connect the LiPo **MX1.25 2-pin** connector directly to the board's onboard battery port. The board's built-in charge circuit handles charging via USB-C automatically — no separate charger board is needed.

The same LiPo also powers the SparkFun MOSFET board via a **MX1.25 Y-splitter cable** — one female end on the battery, two male ends: one to the ESP32 port, one to the MOSFET board VIN/GND.

> ⚠ **Test JST polarity with a multimeter before plugging in — do not skip this.**
> Many cheap LiPo batteries ship with the JST pins reversed. Plugging in backwards damages the board instantly.
>
> **How to check:**
> 1. Set multimeter to DC voltage (20 V range)
> 2. Red probe → battery red wire, black probe → battery black wire
> 3. A reading of **+3.5 V to +4.2 V** = correct polarity, safe to plug in
> 4. A negative reading = wires are reversed — do not plug in
>
> **If reversed:** use a toothpick or safety pin to press the locking tab on each pin inside the JST housing, slide the pins out, swap them, push back in until they click, then re-measure.

Battery voltage is logged to Serial every 60 seconds:
```
Battery: 3.94 V (78%)
```
It is also printed at startup (USB required to see serial output — see boot note below).

### Booting on battery vs USB

| Power source | How to boot |
|---|---|
| USB-C | Board boots automatically when cable is plugged in |
| LiPo (no USB) | Press and hold the power button — release when the display flashes 3 times |

The 3-flash sequence is a boot indicator built into the firmware (`setup()` in `main.cpp`). It pulses the display backlight three times immediately after the power latch succeeds, giving visible confirmation that the board is alive without needing a serial monitor. If the display does not flash, see the troubleshooting notes below.

**Battery boot troubleshooting:**
- No flashes at all → button not held long enough, battery too flat (<3 V), or JST polarity reversed
- Flashes once then goes dark → GPIO7 power latch is failing; this should not happen with the current firmware but would indicate `power_mgr_init()` did not run first in `setup()`
- Board was working on USB but not battery → check that the power button is actually being pressed; the board does not auto-start on battery the way it does on USB

Serial output via `pio device monitor` requires a USB connection (the board uses USB CDC, not a USB-UART chip). On battery-only power the bear operates normally but produces no serial output.

### Battery capacity

SparkFun heating pad resistance: ~6.5 Ω. ESP32 current draw: ~240 mA (WiFi active).

| Config | Heater draw | Total (heater + ESP32) | Runtime on 1500 mAh (50% heater duty) |
|--------|------------|------------------------|---------------------------------------|
| No boost, 3.7 V nominal | ~570 mA | ~810 mA | ~2.8 h |
| No boost, 4.2 V full charge | ~646 mA | ~886 mA | ~2.5 h |
| With 5 V boost (~87% efficient) | ~750 mA load → ~1030 mA from cell | ~1270 mA | ~2.0 h |

Temperature cycling (heater on ~50% of the time) roughly doubles runtime compared to continuous heating. A 2500 mAh cell adds another ~65% on top of those figures.

**Recommendation:** 1500–2000 mAh cell without boost gives the best balance of warmth and runtime for testing.

---

## Safety Notes

- The SparkFun heating pad is rated 5 V / ~750 mA (6.5 Ω). Running it at LiPo voltage (~3.7 V) draws ~570 mA and produces ~2.1 W — lower than rated but safe and warm enough for a bear. The TMP36 safety cutoffs apply regardless of supply voltage.
- Mount the TMP36 **against the heating pad surface** so it reads pad temperature — not floating in stuffing where it can't detect hot spots.
- The firmware uses three stacked safety layers: temperature hysteresis cycling (42 °C target), over-temperature hard cutoff (48 °C), and a session time cutoff (5 min). All are tunable in `config.h`. See the Safety section in the main README for details.
- Use heat-resistant material between the heating pad and stuffing to prevent hot spots.
