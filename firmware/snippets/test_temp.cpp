#include <Arduino.h>

// ── Test: TMP36 temperature sensor ───────────────────────────────────────────
// Wiring:
//   TMP36 pin 1 (VS)   → 3.3 V
//   TMP36 pin 2 (VOUT) → GPIO1
//   TMP36 pin 3 (GND)  → GND
// Open Serial Monitor at 115200 baud.
// Expected: room temperature (~20–25 °C). Hold the sensor body to see it rise.
// Verify it reads ~42 °C when the heating pad is at comfort temperature.

#define PIN_TEMP   1

static float read_celsius() {
    float mV = analogReadMilliVolts(PIN_TEMP);
    if (mV < 100.0f) return -1.0f;
    return (mV - 500.0f) / 10.0f;
}

void setup() {
    Serial.begin(115200);
    delay(500);
    pinMode(PIN_TEMP, INPUT);
    Serial.println("─────────────────────────────");
    Serial.println("CutiePI temperature sensor test — OK");
    Serial.println("Readings every 1 s. Hold sensor to watch it rise.");
    Serial.println("─────────────────────────────");
}

void loop() {
    float t = read_celsius();
    if (t < -40.0f) {
        Serial.println("Sensor error — check wiring on GPIO1");
    } else {
        Serial.print("Temp: ");
        Serial.print(t, 1);
        Serial.print(" C");
        if      (t >= 48.0f) Serial.print("  ← OVER MAX (48 C)");
        else if (t >= 42.0f) Serial.print("  ← above target (42 C)");
        else if (t >= 40.0f) Serial.print("  ← in hysteresis band (40–42 C)");
        else                 Serial.print("  ← below target, heater would run");
        Serial.println();
    }
    delay(1000);
}
