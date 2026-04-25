#include <Arduino.h>
#include <WiFi.h>

// ── Test: All components together ────────────────────────────────────────────
// Connect everything:
//   GPIO3  → FSR voltage divider (3.3V → FSR → GPIO3 → 10 kΩ → GND)
//   GPIO13 → LED + 100 Ω resistor → GND
//   GPIO12 → MOSFET gate → heating pad circuit
//   GPIO1  → TMP36 VOUT (3.3V → TMP36 VS | GPIO1 → TMP36 VOUT | GND → TMP36 GND)
// Open Serial Monitor at 115200 baud.
//
// Behaviour:
//   Not pressing FSR → LED off, heater off
//   Pressing FSR     → LED pulses heartbeat, heater cycles to 42 °C target
//   Temp ≥ 48 °C     → heater off (over-temp), release and re-press to re-enable
//   Release FSR      → LED off, heater off

#define PIN_SENSOR    3
#define PIN_LED       13
#define PIN_HEATER    12
#define PIN_TEMP      1
#define LEDC_CH       0
#define HUG_THRESHOLD 500

static constexpr float TEMP_MAX_SAFE   = 48.0f;
static constexpr float TEMP_TARGET     = 42.0f;
static constexpr float TEMP_HYSTERESIS =  2.0f;

static bool  s_hugging        = false;
static bool  s_heater_active  = false;
static bool  s_overtemp       = false;
static float s_phase          = 0.0f;

static float read_temp() {
    float mV = analogReadMilliVolts(PIN_TEMP);
    if (mV < 100.0f) return -1.0f;
    return (mV - 500.0f) / 10.0f;
}

static void led_set(uint8_t brightness) {
    ledcWrite(LEDC_CH, brightness);
}

static uint8_t heartbeat_brightness() {
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
    pinMode(PIN_TEMP, INPUT);

    Serial.println("─────────────────────────────");
    Serial.println("CutiePI all-components test — OK");
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    Serial.println("Squeeze the FSR to activate LED + heater");
    Serial.println("─────────────────────────────");
}

void loop() {
    float temp_c = read_temp();
    int   raw    = analogRead(PIN_SENSOR);
    bool  pressing = raw > HUG_THRESHOLD;

    // Reset overtemp flag when FSR is released (same as re-hug in full firmware)
    if (!pressing) s_overtemp = false;

    if (pressing != s_hugging) {
        s_hugging = pressing;
        Serial.print("FSR: ");
        Serial.print(raw);
        Serial.println(pressing ? " → HUG ON" : " → HUG OFF");
        if (!pressing) {
            led_set(0);
            s_heater_active = false;
            digitalWrite(PIN_HEATER, LOW);
            s_phase = 0.0f;
        }
    }

    if (s_hugging && !s_overtemp) {
        // Over-temp hard cutoff
        if (temp_c >= TEMP_MAX_SAFE) {
            s_overtemp = true;
            s_heater_active = false;
            digitalWrite(PIN_HEATER, LOW);
            Serial.printf("OVER-TEMP: %.1f C — heater off, release and re-squeeze to re-enable\n", temp_c);
        } else {
            // Hysteresis cycling
            if (temp_c >= 0.0f) {
                if      (temp_c < TEMP_TARGET - TEMP_HYSTERESIS) s_heater_active = true;
                else if (temp_c >= TEMP_TARGET)                   s_heater_active = false;
            } else {
                s_heater_active = true;  // sensor error: run heater
            }
            digitalWrite(PIN_HEATER, s_heater_active ? HIGH : LOW);
        }

        s_phase += 0.01f;
        if (s_phase >= 1.0f) s_phase = 0.0f;
        led_set(heartbeat_brightness());
    }

    // Print status every ~1 s (every 64 × 16 ms loops)
    static uint8_t tick = 0;
    if (++tick == 0) {
        if (temp_c < 0.0f) {
            Serial.println("Temp: sensor error (check GPIO1 wiring)");
        } else {
            Serial.printf("Temp: %.1f C | Heater GPIO: %s\n",
                          temp_c, s_heater_active ? "HIGH" : "LOW");
        }
    }

    delay(16);
}
