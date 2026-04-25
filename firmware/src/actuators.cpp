#include "actuators.h"
#include "config.h"

// s_heater_requested: state machine's intent (set by actuators_set_heater)
// s_heater_active:    actual GPIO state — driven by temperature cycling in actuators_update
static bool     s_heater_requested = false;
static bool     s_heater_active    = false;
static uint32_t s_heater_on_time   = 0;

void actuators_init() {
    pinMode(PIN_HEATER, OUTPUT);
    digitalWrite(PIN_HEATER, LOW);
}

void actuators_set_heater(bool on) {
    if (on && !s_heater_requested) {
        s_heater_on_time = millis();
    }
    s_heater_requested = on;
    if (!on) {
        s_heater_active = false;
        digitalWrite(PIN_HEATER, LOW);
    }
    // When turning on, do not immediately drive the GPIO — actuators_update() will
    // engage the heater once it reads a temperature below the target band.
}

void actuators_update(float temp_c) {
    if (!s_heater_requested) return;

    // Belt-and-suspenders: session time cutoff in case thermistor fails
    if (millis() - s_heater_on_time >= HEATER_MAX_ON_MS) {
        actuators_set_heater(false);
        Serial.println("Heater: session time limit reached, re-hug to re-enable");
        return;
    }

    // Hard over-temperature cutoff
    if (temp_c >= TEMP_MAX_SAFE_C) {
        actuators_set_heater(false);
        Serial.printf("Heater: over-temp %.1f C (limit %.1f C), re-hug to re-enable\n",
                      temp_c, TEMP_MAX_SAFE_C);
        return;
    }

    // Hysteresis cycling.
    // On sensor error (temp_c < 0): skip cycling, run heater continuously —
    // time cutoff above is the remaining safety net.
    bool should_be_active = s_heater_active;
    if (temp_c >= 0.0f) {
        if      (temp_c < TEMP_TARGET_C - TEMP_HYSTERESIS_C) should_be_active = true;
        else if (temp_c >= TEMP_TARGET_C)                     should_be_active = false;
        // In the hysteresis band: maintain current state
    } else {
        should_be_active = true;
    }

    if (should_be_active != s_heater_active) {
        s_heater_active = should_be_active;
        digitalWrite(PIN_HEATER, s_heater_active ? HIGH : LOW);
    }
}
