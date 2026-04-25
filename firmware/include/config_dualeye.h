#pragma once

// ── DualEye 1.28 Pin Overrides ────────────────────────────────────────────────
// This file is included automatically by config.h when building with -DBOARD_DUALEYE.
//
// ⚠ BOARDS NOT YET IN HAND — verify every pin below with the schematic and a
// multimeter before assembling. Known conflicts with the Touch LCD 1.46B pinout
// are called out inline.
//
// Key differences from Touch LCD 1.46B:
//   • GPIO1  = BAT_ADC (hardwired 200K/100K divider) — cannot use for TMP36
//   • GPIO2  = LCD backlight — do not use
//   • GPIO6  = I2C SDA (touch + IMU bus) — do not use
//   • GPIO7  = I2C SCL (touch + IMU bus) — do not use
//   • GPIO4  = Touch panel RST (Half-A) / MOS switch — check schematic before use
//   • GPIO5  = Touch panel INT (Half-A) / MOS switch — check schematic before use
//   • No power-latch circuit (GPIO6/7 are I2C) — power_mgr skips PWR_CTRL/PWR_KEY
//   • CH343P USB-to-UART chip — Serial works normally, no USB CDC tricks needed
//
// ADC1-capable pins (GPIO1–10) that appear free on the 14-pin header:
//   GPIO3  — shared with FSR, so only one of TMP36/FSR can live here
//   GPIO4, GPIO5 — reserved for touch panel; check variant before using
//   GPIO8, GPIO9, GPIO10 — not on the main header; look for test pads or solder points
//
// Suggested assignments (TBD — fill in once boards arrive):
//   TMP36 temp sensor : needs ADC1 (GPIO1–10) — GPIO3 or GPIO8–10 (verify)
//   FSR pressure      : GPIO3 (ADC1_CH2) or keep as touch T3 (no ADC needed)
//   Heater MOSFET     : GPIO4 or GPIO5 if built-in MOS switch current rating
//                       is sufficient (≤500 mA); otherwise find another free pin
//   LED PWM           : GPIO41 or GPIO42 (free on header, not ADC-constrained)

// ── Compile gate ─────────────────────────────────────────────────────────────
// Remove this error once pin assignments below are filled in and verified.
#error "DualEye pin assignments not yet verified. Fill in config_dualeye.h and remove this line."

// ── Pin Definitions (fill in after verifying with schematic / multimeter) ────
// #define PIN_TEMP_SENSOR       ??   // Must be ADC1 (GPIO1-10); GPIO1 is BAT_ADC — conflict
// #define PIN_PRESSURE_SENSOR   3    // Tentative — same as 1.46B; verify on DualEye header
// #define PIN_HEATER            ??   // GPIO4/5 (built-in MOS) or another free pin
// #define PIN_LED               ??   // GPIO41 or GPIO42 (non-ADC GPIO, fine for PWM)

// ── Power management ─────────────────────────────────────────────────────────
// No power-latch pins available (GPIO6/7 are I2C). power_mgr_init() skips the
// latch on BOARD_DUALEYE builds — board powers on normally from USB or battery.
// BAT_ADC is GPIO1 with a 1:3 divider (200K / 100K) — same ratio as Touch 1.46B.
#define DUALEYE_BAT_ADC_PIN   1
