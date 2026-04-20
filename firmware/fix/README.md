# API Compatibility Fix Reference

These files are **not compiled** — they're reference copies for debugging API mismatches between Arduino ESP32 core versions.

## Which version are you on?

Check `platformio.ini`:
- `espressif32 @ 6.x.x` (up to ~6.8.x) → **Arduino ESP32 2.x** → use the files in `firmware/src/`
- `espressif32 @ 6.9.0+` or no version pin → **Arduino ESP32 3.x** → use the files here

Not sure which you have? Run `pio pkg list` in the firmware directory and look for the `framework-arduinoespressif32` version number.

---

## Differences at a glance

### heartbeat.cpp — LEDC (LED PWM)

| | 2.x (`firmware/src/`) | 3.x (`firmware/fix/`) |
|---|---|---|
| Init | `ledcSetup(CH, freq, res)` + `ledcAttachPin(pin, CH)` | `ledcAttach(pin, freq, res)` |
| Write | `ledcWrite(CH, duty)` | `ledcWrite(pin, duty)` |

2.x needs an explicit channel number (`LEDC_CH 0`). 3.x uses the pin number directly as the identifier — no channel needed.

### comms.cpp — ESP-NOW receive callback

| | 2.x (`firmware/src/`) | 3.x (`firmware/fix/`) |
|---|---|---|
| ESP-NOW callback | `(const uint8_t *mac_addr, ...)` | `(const esp_now_recv_info_t *info, ...)` |
| `comms_update()` | `{}` no-op (ESP-NOW) / full impl (MQTT) | identical |
| MQTT section | full `#ifdef USE_MQTT` block | identical — PubSubClient has no 3.x dependency |

The **only** difference between 2.x and 3.x comms is the ESP-NOW callback signature.
The MQTT section and `comms_update()` are byte-for-byte identical across both versions.

---

## To switch to 3.x

```bash
# Run from the firmware/ directory
cp fix/heartbeat_3x.cpp src/heartbeat.cpp
cp fix/comms_3x.cpp     src/comms.cpp
```

Then in `platformio.ini` change:
```ini
platform = espressif32 @ 6.5.0
```
to:
```ini
platform = espressif32 @ ~6.9.0
```

---

## Hardware variant: USB heating pad (`actuators_usb_heater.cpp`)

Use this instead of `src/actuators.cpp` when the heating pad has a USB-A connector and is switched via a **P-channel MOSFET** on the VBUS line.

| | N-channel / JST (`src/actuators.cpp`) | P-channel / USB (`fix/actuators_usb_heater.cpp`) |
|---|---|---|
| Init state | `digitalWrite(PIN_HEATER, LOW)` | `digitalWrite(PIN_HEATER, HIGH)` |
| Heater on | `digitalWrite(PIN_HEATER, HIGH)` | `digitalWrite(PIN_HEATER, LOW)` |
| Heater off | `digitalWrite(PIN_HEATER, LOW)` | `digitalWrite(PIN_HEATER, HIGH)` |

The P-channel gate is driven inverted: HIGH = off, LOW = on. Everything else — safety cutoff, timing, API — is identical.

**To use:** Copy `fix/actuators_usb_heater.cpp` → `src/actuators.cpp`. No other file changes needed.

---

## Hardware variant: Addressable LED strip (`heartbeat_led_strip.cpp`)

Use this instead of `src/heartbeat.cpp` when using a WS2812B strip (or compatible) instead of a single red LED.

| | Single LED (`src/heartbeat.cpp`) | LED strip (`fix/heartbeat_led_strip.cpp`) |
|---|---|---|
| Library | Arduino LEDC (built-in) | FastLED |
| Init | `ledcSetup` / `ledcAttachPin` | `FastLED.addLeds<WS2812B, PIN_LED, GRB>` |
| Brightness | `ledcWrite(CH, value)` | `fill_solid` + `FastLED.show()` |
| Color | Physical red LED | RGB set to `CRGB(brightness, 0, 0)` |
| `NUM_LEDS` | n/a | Set at top of file (default: 8) |

`PIN_LED` (config.h, currently GPIO5) is reused as the strip data pin. Power the strip from a dedicated 5V rail — not the ESP32's 3.3V or onboard 5V pin if the strip has more than ~3 LEDs.

**To use:**
1. Copy `fix/heartbeat_led_strip.cpp` → `src/heartbeat.cpp`
2. Add to `platformio.ini` under `lib_deps`:
   ```
   fastled/FastLED @ ^3.7.0
   ```
