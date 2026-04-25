# Component Test Snippets

Use these to test one component at a time before flashing the full CutiePI firmware.
Each file is a complete standalone sketch — safe to run with nothing else connected.

## How to use

1. Copy the snippet you want to test into `firmware/src/` as the **only** `.cpp` file
   (rename or move `main.cpp` out first)
2. Run `pio run -e bear --target upload`
3. Open Serial Monitor at **115200 baud** to see output

## Snippets

| File | Tests | What to have connected |
|------|-------|----------------------|
| `test_board.cpp` | Board alive, MAC address prints | Nothing — just the ESP32-S3 via USB |
| `test_led.cpp` | LED PWM, lub-dub pulse | LED + 100 Ω resistor on GPIO13 |
| `test_sensor.cpp` | FSR/pressure ADC readings | FSR voltage divider on GPIO3 |
| `test_heater.cpp` | MOSFET switching, heater on/off | Full MOSFET circuit + heating pad on GPIO12 |
| `test_temp.cpp` | TMP36 readings, threshold labels | TMP36 VOUT on GPIO1 |
| `test_all.cpp` | All components together | Everything wired up |

## Tip

Start with `test_board.cpp` — no hardware needed, just confirms the board
is alive and PlatformIO is talking to it correctly.
