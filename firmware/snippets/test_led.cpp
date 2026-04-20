#include <Arduino.h>

// ── Test: LED PWM heartbeat pulse ────────────────────────────────────────────
// Connect: LED + 100 Ω resistor between GPIO5 and GND.
// You should see a slow lub-dub pulse (two quick flashes, then pause).
// Open Serial Monitor at 115200 baud to watch brightness values.

#define PIN_LED 5
#define LEDC_CH 0

// Lub-dub timing (milliseconds)
static const uint16_t LUB_RISE   = 80;
static const uint16_t LUB_FALL   = 80;
static const uint16_t DUB_RISE   = 80;
static const uint16_t DUB_FALL   = 80;
static const uint16_t PAUSE      = 600;

static void fade(uint8_t from, uint8_t to, uint16_t ms) {
    int steps = abs((int)to - (int)from);
    if (steps == 0) { delay(ms); return; }
    uint16_t step_ms = ms / steps;
    int dir = (to > from) ? 1 : -1;
    for (int v = from; v != to; v += dir) {
        ledcWrite(LEDC_CH, v);
        Serial.print("brightness: ");
        Serial.println(v);
        delay(step_ms);
    }
    ledcWrite(LEDC_CH, to);
}

void setup() {
    Serial.begin(115200);
    delay(500);
    ledcSetup(LEDC_CH, 5000, 8);   // 5 kHz, 8-bit (0–255)
    ledcAttachPin(PIN_LED, LEDC_CH);
    ledcWrite(LEDC_CH, 0);
    Serial.println("─────────────────────────────");
    Serial.println("CutiePI LED test — OK");
    Serial.println("Watch for lub-dub pulse on GPIO5");
    Serial.println("─────────────────────────────");
}

void loop() {
    Serial.println("lub —");
    fade(0, 220, LUB_RISE);
    fade(220, 30, LUB_FALL);

    Serial.println("dub —");
    fade(30, 255, DUB_RISE);
    fade(255, 0, DUB_FALL);

    delay(PAUSE);
}
