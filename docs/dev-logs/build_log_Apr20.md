# Build Log

## 2026-04-20 — Both environments passing

Platform: `espressif32 @ 6.5.0` (framework-arduinoespressif32 @ 2.0.14)
Hardware: Waveshare ESP32-S3-Touch-LCD-1.46B (ESP32-S3R8 — 16 MB Flash, 8 MB OPI PSRAM) — 240 MHz, 512 KB SRAM

| Environment | Status  | Duration | RAM used       | Flash used      |
|-------------|---------|----------|----------------|-----------------|
| `bear`      | SUCCESS | 3.09s    | 13.4% (44068 B / 327680 B) | 21.1% (705241 B / 3342336 B) |
| `bear-mqtt` | SUCCESS | 3.33s    | 13.9% (45428 B / 327680 B) | 21.6% (721897 B / 3342336 B) |

### Dependency graph

**bear** (ESP-NOW):
- WiFi @ 2.0.0

**bear-mqtt** (MQTT):
- PubSubClient @ 2.8.0
- WiFi @ 2.0.0

### Notes
- No warnings in project source files. One upstream framework warning in `esp32-hal-uart.c` (unrelated to this project).
- MQTT build adds ~1360 B RAM and ~16656 B Flash over the ESP-NOW build — both well within budget.
