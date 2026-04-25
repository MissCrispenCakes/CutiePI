#include <Arduino.h>
#include "temp_sensor.h"
#include "config.h"

// TMP36 output: 10 mV/°C with 500 mV offset at 0 °C
// Formula: temp_C = (Vout_mV - 500) / 10

void temp_sensor_init() {
    pinMode(PIN_TEMP_SENSOR, INPUT);
}

float temp_sensor_read_celsius() {
    float mV = analogReadMilliVolts(PIN_TEMP_SENSOR);
    if (mV < 100.0f) return -1.0f;  // open circuit / wiring error guard
    return (mV - 500.0f) / 10.0f;
}
