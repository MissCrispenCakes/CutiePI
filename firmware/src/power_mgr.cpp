#include <Arduino.h>
#include "power_mgr.h"

// Waveshare ESP32-S3-Touch-LCD-1.46B power management GPIOs.
// These are fixed by the board's hardware — do not change them in config.h.
static constexpr uint8_t PWR_CTRL_PIN = 7;   // Drive HIGH to stay on; LOW = shutdown
static constexpr uint8_t PWR_KEY_PIN  = 6;   // Power button, active LOW
static constexpr uint8_t BAT_ADC_PIN  = 8;   // Battery sense: 1:3 divider (2 MΩ / 1 MΩ)

// 1S LiPo voltage range
static constexpr float BAT_V_FULL  = 4.2f;
static constexpr float BAT_V_EMPTY = 3.0f;

static uint32_t s_key_down_since = 0;
static bool     s_key_was_down   = false;
static uint32_t s_last_bat_log   = 0;

void power_mgr_init() {
    // Latch board power on — must happen before any significant delay in setup()
    pinMode(PWR_CTRL_PIN, OUTPUT);
    digitalWrite(PWR_CTRL_PIN, HIGH);

    pinMode(PWR_KEY_PIN, INPUT_PULLUP);
}

float power_mgr_battery_volts() {
    // Voltage divider: Vbat → 2 MΩ → ADC pin → 1 MΩ → GND  →  Vadc = Vbat / 3
    return analogReadMilliVolts(BAT_ADC_PIN) * 3.0f / 1000.0f;
}

int power_mgr_battery_pct() {
    float pct = (power_mgr_battery_volts() - BAT_V_EMPTY) /
                (BAT_V_FULL - BAT_V_EMPTY) * 100.0f;
    if (pct < 0.0f)   pct = 0.0f;
    if (pct > 100.0f) pct = 100.0f;
    return (int)pct;
}

void power_mgr_update() {
    bool key_down = (digitalRead(PWR_KEY_PIN) == LOW);

    if (key_down && !s_key_was_down) {
        s_key_down_since = millis();
    }

    // Hold 3 s → graceful shutdown
    if (key_down && (millis() - s_key_down_since >= 3000)) {
        Serial.println("Power: shutting down (long press)");
        Serial.flush();
        digitalWrite(PWR_CTRL_PIN, LOW);
    }

    s_key_was_down = key_down;

    // Log battery voltage every 60 s
    uint32_t now = millis();
    if (now - s_last_bat_log >= 60000) {
        s_last_bat_log = now;
        Serial.printf("Battery: %.2f V (%d%%)\n",
                      power_mgr_battery_volts(), power_mgr_battery_pct());
    }
}
