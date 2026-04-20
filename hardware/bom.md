# Bill of Materials — CutiePI Bear (×2)

| Qty | Component | Notes |
|-----|-----------|-------|
| 1 | ESP32-S3 DevKitC-1 | Main microcontroller |
| 1 | Force-Sensitive Resistor (FSR 402 or similar) **with 2-pin JST-PH connector** | Hug detection; JST pins are the two FSR terminals — wire into voltage divider on the board |
| 1 | Red LED (5 mm, ~2V Vf) **with 2-pin JST-PH connector** | Heartbeat glow; JST red → board-side 100 Ω resistor → GPIO5, JST black → GND |
| 1 | 100 Ω resistor | Current limiter for LED (on the board, between GPIO5 and the LED JST red wire) |
| 1 | Heating pad (5 V, ≤500 mA) **with 2-pin JST-PH connector** | Warmth; JST red → MOSFET drain side / 5 V rail, JST black → MOSFET drain |
| 1 | N-channel MOSFET (e.g. 2N7000 or IRLZ44N) | Controls heater from 3.3 V GPIO |
| 1 | 1 kΩ resistor | MOSFET gate resistor |
| 1 | 1N4007 flyback diode | Across heater JST terminals on the board (cathode toward 5 V) |
| 1 | 10 kΩ resistor | FSR voltage divider pull-down (on the board) |
| 1 | LiPo battery (1S, 1000–2000 mAh) + TP4056 charger board | Portable power |
| — | 2-pin JST-PH connectors + crimps (or pre-crimped pigtails) | For LED, FSR, and heater pad |
| — | Hookup wire, heat-shrink tubing, hot glue | Assembly |

## Wiring Summary

All three components plug in via **2-pin JST-PH** (red = +/signal, black = GND).
The voltage divider, current-limiting resistor, and MOSFET circuit all live on the board/breadboard side of each connector.

```
── Pressure sensor (FSR) ──────────────────────────────────────
3.3 V ──── [JST red] ──── FSR ──── [JST black] ──── GPIO4 (ADC)
                                                         │
                                                       10 kΩ
                                                         │
                                                        GND
  GPIO4 ADC is 12-bit (0–4095). PRESSURE_THRESHOLD in config.h defaults to 500,
  meaning the FSR needs to drop the voltage divider below ~0.4 V to register a hug.

── LED ────────────────────────────────────────────────────────
GPIO5 (PWM) ─── 100 Ω ─── [JST red / LED anode]
                           [JST black / LED cathode] ─── GND

── Heating pad ────────────────────────────────────────────────
GPIO6 ─── 1 kΩ ─── MOSFET gate
                    MOSFET source ──────────────────── GND
                    MOSFET drain  ─── [JST black]
                    [JST red]     ─── 5 V
                    1N4007 flyback diode across JST pins on board
                    (diode cathode toward 5 V / JST red)
```

## Sensor Selection — FSR vs. Capacitive Touch Pad

The firmware supports two hug-detection methods. Choose based on what you have available.
Toggle by uncommenting `#define USE_TOUCH_SENSOR` in `firmware/include/config.h`.

### Option A — FSR voltage divider (default, recommended)

| | |
|---|---|
| **Hardware** | FSR 402 + 10 kΩ pull-down resistor (see wiring above) |
| **How it works** | Squeezing the bear lowers the FSR resistance, raising the ADC voltage on GPIO4 |
| **Strengths** | Immune to WiFi/RF interference; measures actual squeeze force through stuffing |
| **Weaknesses** | Needs two extra components; threshold may need tuning per FSR batch |

### Option B — ESP32-S3 capacitive touch pad

| | |
|---|---|
| **Hardware** | Conductive pad (copper tape or conductive fabric) wired to GPIO4 (= touch channel T4) |
| **How it works** | Hand contact/proximity on the pad changes capacitance; firmware reads `touchRead(GPIO4)` |
| **Strengths** | No resistors needed; single wire from GPIO4 to the pad; simpler assembly |
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
GPIO4 (T4) ──── conductive pad sewn or taped inside the bear
                (no other connections needed — no power, no GND, no resistors)
```

Remove the FSR and the 10 kΩ resistor from your build entirely when using Option B.

---

## Safety Notes

- The heating pad **must** be rated for 5 V / ≤500 mA.
- The firmware enforces a **5-minute heater cutoff** (`HEATER_MAX_ON_MS` in `config.h`).
- Use heat-resistant material between the heating pad and stuffing to prevent hot spots.
