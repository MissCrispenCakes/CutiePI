#pragma once

// ── Device Identity ───────────────────────────────────────────────────────────
// Set BEAR_ID to 0 for Bear A, 1 for Bear B before flashing each device.
// Run once with Serial.println(WiFi.macAddress()) in setup() to discover MACs.
#define BEAR_ID 0

// ── Peer MAC Address (ESP-NOW mode only) ──────────────────────────────────────
// Not used in MQTT mode — skip this section if building with USE_MQTT.
// REQUIRED for ESP-NOW: replace with the actual MAC of the OTHER bear.
// To find the MAC: flash with BEAR_ID set, open Serial Monitor — it prints on boot.
// All-0xFF is the ESP-NOW broadcast address and will NOT reach a specific peer.
#ifndef USE_MQTT
  #if BEAR_ID == 0
  static const uint8_t PEER_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // ← Bear B MAC here
  #else
  static const uint8_t PEER_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // ← Bear A MAC here
  #endif
#endif

// ── WiFi + MQTT (MQTT mode only) ──────────────────────────────────────────────
// Only used when building with USE_MQTT (pio run -e bear-mqtt).
// Credentials live in secrets.h (gitignored) — never hardcode them here.
// Copy firmware/include/secrets.h.example → firmware/include/secrets.h first.
#ifdef USE_MQTT
  #if __has_include("secrets.h")
    #include "secrets.h"
  #endif
  // Fallback placeholders — active only if secrets.h is missing.
  #ifndef WIFI_SSID
    #define WIFI_SSID     "your_network_name"
    #define WIFI_PASSWORD "your_network_password"
    #define MQTT_BROKER   "broker.hivemq.com"
    #define MQTT_PORT     1883
    #define MQTT_USER     ""
    #define MQTT_PASS     ""
  #endif
#endif

// ── Sensor Mode ───────────────────────────────────────────────────────────────
// By default the hug sensor is an FSR (Force-Sensitive Resistor) read via ADC.
// Uncomment the line below to switch to the ESP32-S3 capacitive touch pad instead.
// Read the "Sensor Selection" section in hardware/bom.md before choosing —
// there is a known WiFi/touch interference issue that affects this project.
// #define USE_TOUCH_SENSOR

// ── Pin Definitions ───────────────────────────────────────────────────────────
#define PIN_PRESSURE_SENSOR   4   // ADC input (FSR mode) or touch pad T4 (touch mode) — same GPIO
#define PIN_LED               5   // PWM output — red LED (via current-limiting resistor)
#define PIN_HEATER            6   // Digital output — MOSFET gate controlling heating pad

// ── Sensor Thresholds ─────────────────────────────────────────────────────────
// FSR mode: ADC value (0–4095) above which a hug is detected.
#define PRESSURE_THRESHOLD    500

// Touch mode: touch fires when a reading drops below (baseline × ratio).
// 0.6 = requires a 40% drop from the untouched baseline. Raise toward 0.8 if
// the bear is hard to trigger through thick fabric; lower toward 0.4 to reduce
// false triggers from nearby hands without contact.
#define TOUCH_THRESHOLD_RATIO 0.6f

// Debounce applies to both modes. Touch is noisier due to WiFi interference —
// if using USE_TOUCH_SENSOR, consider raising this to 500.
#define PRESSURE_DEBOUNCE_MS  200

// ── Heartbeat Parameters ──────────────────────────────────────────────────────
#define HEARTBEAT_BPM         70   // Beats per minute for LED animation

// ── Heater Safety ─────────────────────────────────────────────────────────────
#define HEATER_MAX_ON_MS      (5 * 60 * 1000UL)  // Hard cutoff after 5 min; UL prevents overflow on 16-bit int targets

// ── Compile-time Sanity Checks ────────────────────────────────────────────────
#if BEAR_ID != 0 && BEAR_ID != 1
  #error "BEAR_ID must be 0 (Bear A) or 1 (Bear B)"
#endif
#if PRESSURE_THRESHOLD < 1 || PRESSURE_THRESHOLD > 4094
  #error "PRESSURE_THRESHOLD must be between 1 and 4094 (12-bit ADC range is 0–4095)"
#endif
