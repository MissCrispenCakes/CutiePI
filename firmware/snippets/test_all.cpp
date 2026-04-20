#include <Arduino.h>
#include <WiFi.h>

// ── Test: All components together ────────────────────────────────────────────
// Connect everything:
//   GPIO4 → FSR voltage divider (3.3V → FSR → GPIO4 → 10 kΩ → GND)
//   GPIO5 → LED + 100 Ω resistor → GND
//   GPIO6 → MOSFET gate → heating pad circuit
// Open Serial Monitor at 115200 baud.
//
// Behaviour:
//   Not pressing FSR → LED off, heater off
//   Pressing FSR     → LED pulses heartbeat, heater turns on
//   Release FSR      → LED off, heater off

#define PIN_SENSOR    4
#define PIN_LED       5
#define PIN_HEATER    6
#define LEDC_CH       0
#define HUG_THRESHOLD 500

static bool  s_hugging   = false;
static float s_phase     = 0.0f;   // heartbeat phase 0–1

static void led_set(uint8_t brightness) {
    ledcWrite(LEDC_CH, brightness);
}

static uint8_t heartbeat_brightness() {
    // Simple lub-dub: two quick bumps per beat cycle
    float p = s_phase;
    uint8_t b = 0;
    if      (p < 0.08f) b = (uint8_t)(p / 0.08f * 255);
    else if (p < 0.16f) b = (uint8_t)((0.16f - p) / 0.08f * 255);
    else if (p < 0.24f) b = (uint8_t)((p - 0.16f) / 0.08f * 200);
    else if (p < 0.32f) b = (uint8_t)((0.32f - p) / 0.08f * 200);
    return b;
}

void setup() {
    Serial.begin(115200);
    delay(500);

    WiFi.mode(WIFI_STA);
    ledcSetup(LEDC_CH, 5000, 8);
    ledcAttachPin(PIN_LED, LEDC_CH);
    led_set(0);

    pinMode(PIN_HEATER, OUTPUT);
    digitalWrite(PIN_HEATER, LOW);

    Serial.println("─────────────────────────────");
    Serial.println("CutiePI all-components test — OK");
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    Serial.println("Squeeze the FSR to activate LED + heater");
    Serial.println("─────────────────────────────");
}

void loop() {
    int raw = analogRead(PIN_SENSOR);
    bool pressing = raw > HUG_THRESHOLD;

    if (pressing != s_hugging) {
        s_hugging = pressing;
        Serial.print("FSR: ");
        Serial.print(raw);
        Serial.println(pressing ? " → HUG ON" : " → HUG OFF");
        if (!pressing) {
            led_set(0);
            digitalWrite(PIN_HEATER, LOW);
            s_phase = 0.0f;
        }
    }

    if (s_hugging) {
        s_phase += 0.01f;
        if (s_phase >= 1.0f) s_phase = 0.0f;
        led_set(heartbeat_brightness());
        digitalWrite(PIN_HEATER, HIGH);
    }

    delay(16);   // ~60 Hz update
}
