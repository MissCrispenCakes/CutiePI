#include <Arduino.h>

// ── Test: MOSFET heater switching ────────────────────────────────────────────
// Connect: N-channel MOSFET circuit with heating pad on GPIO6.
//   GPIO6 → MOSFET gate (via 100 Ω resistor)
//   MOSFET drain → heating pad → 5V
//   MOSFET source → GND
//   10 kΩ pull-down from gate to GND
// Open Serial Monitor at 115200 baud.
// The heater turns ON for 3 seconds, OFF for 3 seconds, repeating.
// You should feel warmth during the ON phase.
//
// SAFETY: Do not leave the heater running unattended.
//         The full firmware has a 5-minute automatic cutoff.

#define PIN_HEATER 6

void setup() {
    Serial.begin(115200);
    delay(500);
    pinMode(PIN_HEATER, OUTPUT);
    digitalWrite(PIN_HEATER, LOW);   // start OFF
    Serial.println("─────────────────────────────");
    Serial.println("CutiePI heater test — OK");
    Serial.println("Cycling: 3s ON / 3s OFF");
    Serial.println("─────────────────────────────");
}

void loop() {
    Serial.println("Heater ON");
    digitalWrite(PIN_HEATER, HIGH);
    delay(3000);

    Serial.println("Heater OFF");
    digitalWrite(PIN_HEATER, LOW);
    delay(3000);
}
