#include "sensors.h"
#include "config.h"

#ifdef USE_TOUCH_SENSOR
static_assert(TOUCH_THRESHOLD_RATIO > 0.1f && TOUCH_THRESHOLD_RATIO < 0.99f,
    "TOUCH_THRESHOLD_RATIO must be between 0.1 and 0.99");
#endif

static bool     s_hugged         = false;
static uint32_t s_last_change_ms = 0;

#ifdef USE_TOUCH_SENSOR
static uint32_t s_touch_baseline = 0;
#endif

void sensors_init() {
#ifdef USE_TOUCH_SENSOR
    // Discard the first several reads — the touch peripheral needs a few cycles to settle.
    for (int i = 0; i < 10; i++) touchRead(PIN_PRESSURE_SENSOR);

    // Calibrate: average 16 untouched readings as the baseline.
    // The bear must not be held during boot for this to be accurate.
    uint32_t sum = 0;
    for (int i = 0; i < 16; i++) sum += touchRead(PIN_PRESSURE_SENSOR);
    s_touch_baseline = sum / 16;

    if (s_touch_baseline < 10) {
        Serial.println("WARNING: touch baseline is suspiciously low — bear may have been held during boot.");
        Serial.println("         Power-cycle without touching the bear to recalibrate.");
    } else {
        Serial.printf("Touch baseline: %u (threshold at %u)\n",
                      s_touch_baseline,
                      (uint32_t)(s_touch_baseline * TOUCH_THRESHOLD_RATIO));
    }
#else
    // FSR wired as a voltage divider to an ADC pin; no pull configuration needed.
    pinMode(PIN_PRESSURE_SENSOR, INPUT);
#endif
}

bool sensors_is_hugged() {
#ifdef USE_TOUCH_SENSOR
    // Average 4 reads per call to reduce single-sample spikes caused by
    // ESP-NOW transmissions coupling into the touch electrode.
    uint32_t sum = 0;
    for (int i = 0; i < 4; i++) sum += touchRead(PIN_PRESSURE_SENSOR);
    uint32_t raw     = sum / 4;
    bool     pressed = (raw < (uint32_t)(s_touch_baseline * TOUCH_THRESHOLD_RATIO));
#else
    int  raw     = analogRead(PIN_PRESSURE_SENSOR);
    bool pressed = (raw > PRESSURE_THRESHOLD);
#endif

    if (pressed != s_hugged && (millis() - s_last_change_ms) > PRESSURE_DEBOUNCE_MS) {
        s_hugged         = pressed;
        s_last_change_ms = millis();
    }

    return s_hugged;
}
