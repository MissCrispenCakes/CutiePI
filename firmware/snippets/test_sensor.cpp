#include <Arduino.h>

// ── Test: FSR pressure sensor ADC readings ───────────────────────────────────
// Connect: FSR voltage divider on GPIO3 (ADC1_CH2).
//   3.3V → FSR → GPIO3 → 10 kΩ → GND
// Open Serial Monitor at 115200 baud.
// Idle (not pressed) reads close to 0. Press the FSR — value climbs toward 4095.
// A reading above 500 counts as a hug in the full firmware.

#define PIN_SENSOR    3
#define HUG_THRESHOLD 500

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("─────────────────────────────");
    Serial.println("CutiePI sensor test — OK");
    Serial.print("Hug threshold: ");
    Serial.println(HUG_THRESHOLD);
    Serial.println("Press the FSR to test…");
    Serial.println("─────────────────────────────");
}

void loop() {
    int raw = analogRead(PIN_SENSOR);
    bool hugging = raw > HUG_THRESHOLD;

    Serial.print("ADC: ");
    Serial.print(raw);
    Serial.print(" / 4095   ");
    Serial.println(hugging ? "← HUG DETECTED" : "");

    delay(100);
}
