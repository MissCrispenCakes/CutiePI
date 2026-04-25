#pragma once
#include <Arduino.h>

void actuators_init();
void actuators_set_heater(bool on);

// Must be called every loop(). Enforces:
//   • Time-based session cutoff (HEATER_MAX_ON_MS)
//   • Over-temperature hard cutoff (TEMP_MAX_SAFE_C)
//   • Hysteresis cycling between TEMP_TARGET_C and (TEMP_TARGET_C - TEMP_HYSTERESIS_C)
// Pass temp_c from temp_sensor_read_celsius(). Negative values mean sensor error —
// heater still runs on time cutoff alone.
void actuators_update(float temp_c);
