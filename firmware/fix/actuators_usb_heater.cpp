#include "actuators.h"
#include "config.h"

// USB heating pad variant — uses a P-channel MOSFET on the USB VBUS line.
//
// Wiring difference from the JST/N-channel version:
//   5V ─── P-MOSFET source
//           P-MOSFET gate  ─── 1 kΩ ─── GPIO12
//           P-MOSFET drain ─── USB pad red wire (VBUS)
//           USB pad black wire ─── GND
//           1N4007 flyback diode across pad terminals (cathode toward 5V)
//
// P-channel logic is inverted: GPIO HIGH = MOSFET off, GPIO LOW = MOSFET on.
// All safety logic (temperature cycling, time cutoff) is identical to actuators.cpp.

static bool     s_heater_requested = false;
static bool     s_heater_active    = false;
static uint32_t s_heater_on_time   = 0;

void actuators_init() {
    pinMode(PIN_HEATER, OUTPUT);
    digitalWrite(PIN_HEATER, HIGH);  // HIGH = off for P-channel
}

void actuators_set_heater(bool on) {
    if (on && !s_heater_requested) {
        s_heater_on_time = millis();
    }
    s_heater_requested = on;
    if (!on) {
        s_heater_active = false;
        digitalWrite(PIN_HEATER, HIGH);  // HIGH = off for P-channel
    }
}

void actuators_update(float temp_c) {
    if (!s_heater_requested) return;

    if (millis() - s_heater_on_time >= HEATER_MAX_ON_MS) {
        actuators_set_heater(false);
        Serial.println("Heater: session time limit reached, re-hug to re-enable");
        return;
    }

    if (temp_c >= TEMP_MAX_SAFE_C) {
        actuators_set_heater(false);
        Serial.printf("Heater: over-temp %.1f C (limit %.1f C), re-hug to re-enable\n",
                      temp_c, TEMP_MAX_SAFE_C);
        return;
    }

    bool should_be_active = s_heater_active;
    if (temp_c >= 0.0f) {
        if      (temp_c < TEMP_TARGET_C - TEMP_HYSTERESIS_C) should_be_active = true;
        else if (temp_c >= TEMP_TARGET_C)                     should_be_active = false;
    } else {
        should_be_active = true;
    }

    if (should_be_active != s_heater_active) {
        s_heater_active = should_be_active;
        digitalWrite(PIN_HEATER, s_heater_active ? LOW : HIGH);  // inverted vs N-channel
    }
}
