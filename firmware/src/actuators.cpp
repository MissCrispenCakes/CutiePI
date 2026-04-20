#include "actuators.h"
#include "config.h"

static bool     s_heater_on      = false;
static uint32_t s_heater_on_time = 0;

void actuators_init() {
    pinMode(PIN_HEATER, OUTPUT);
    digitalWrite(PIN_HEATER, LOW);
}

void actuators_set_heater(bool on) {
    if (on && !s_heater_on) {
        s_heater_on_time = millis();
    }
    s_heater_on = on;
    digitalWrite(PIN_HEATER, on ? HIGH : LOW);
}

void actuators_update() {
    if (s_heater_on && (millis() - s_heater_on_time) >= HEATER_MAX_ON_MS) {
        actuators_set_heater(false);
        Serial.println("Heater safety cutoff — re-hug to re-enable");
    }
}
