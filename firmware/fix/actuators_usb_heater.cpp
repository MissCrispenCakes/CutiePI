#include "actuators.h"
#include "config.h"

// USB heating pad variant — uses a P-channel MOSFET on the USB VBUS line.
//
// Wiring difference from the JST/N-channel version:
//   5V ─── P-MOSFET source
//           P-MOSFET gate  ─── 1 kΩ ─── GPIO6
//           P-MOSFET drain ─── USB pad red wire (VBUS)
//           USB pad black wire ─── GND
//           1N4007 flyback diode across pad terminals (cathode toward 5V)
//
// P-channel logic is inverted: GPIO HIGH = MOSFET off, GPIO LOW = MOSFET on.
// Everything else (safety cutoff, API) is identical to actuators.cpp.

static bool     s_heater_on      = false;
static uint32_t s_heater_on_time = 0;

void actuators_init() {
    pinMode(PIN_HEATER, OUTPUT);
    digitalWrite(PIN_HEATER, HIGH);  // HIGH = off for P-channel
}

void actuators_set_heater(bool on) {
    if (on && !s_heater_on) {
        s_heater_on_time = millis();
    }
    s_heater_on = on;
    digitalWrite(PIN_HEATER, on ? LOW : HIGH);  // inverted vs N-channel
}

void actuators_update() {
    if (s_heater_on && (millis() - s_heater_on_time) >= HEATER_MAX_ON_MS) {
        actuators_set_heater(false);
        Serial.println("Heater safety cutoff — re-hug to re-enable");
    }
}
