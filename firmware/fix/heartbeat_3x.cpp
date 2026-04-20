#include "heartbeat.h"
#include "config.h"

// Heartbeat animation: two-pulse "lub-dub" pattern.
// Each step defines the target brightness and how long to spend reaching it.
struct Step { uint8_t brightness; uint16_t duration_ms; };

static const Step BEAT[] = {
    { 255, 100 },  // lub — rise to full brightness
    {   0,  80 },  // lub — fast fall
    { 200,  80 },  // dub — second peak (slightly softer)
    {   0, 150 },  // dub — fade to off
    // silence fills the remainder of the beat period
};
static const size_t   N_STEPS        = sizeof(BEAT) / sizeof(BEAT[0]);
static const uint32_t BEAT_PERIOD_MS = 60000UL / HEARTBEAT_BPM;

static bool     s_active       = false;
static size_t   s_step         = N_STEPS;   // N_STEPS = in rest phase
static uint32_t s_step_start   = 0;
static uint32_t s_beat_start   = 0;
static uint8_t  s_start_bright = 0;

// Arduino ESP32 3.x LEDC API — ledcAttach uses the pin directly as the identifier.
void heartbeat_init() {
    ledcAttach(PIN_LED, 5000, 8);
    ledcWrite(PIN_LED, 0);
}

void heartbeat_set_active(bool active) {
    if (active == s_active) return;
    s_active = active;
    if (!active) {
        ledcWrite(PIN_LED, 0);
        s_step       = N_STEPS;
        s_beat_start = 0;
    }
}

void heartbeat_update() {
    if (!s_active) return;

    uint32_t now = millis();

    // Kick off a new beat cycle
    if (s_beat_start == 0) {
        s_beat_start   = now;
        s_step_start   = now;
        s_step         = 0;
        s_start_bright = 0;
    }

    // Advance through completed steps
    while (s_step < N_STEPS && (now - s_step_start) >= BEAT[s_step].duration_ms) {
        s_start_bright  = BEAT[s_step].brightness;
        s_step_start   += BEAT[s_step].duration_ms;
        s_step++;
    }

    if (s_step >= N_STEPS) {
        // Rest until the next beat period begins
        ledcWrite(PIN_LED, 0);
        if (now - s_beat_start >= BEAT_PERIOD_MS) {
            s_beat_start = 0;
        }
        return;
    }

    // Interpolate brightness within the current step
    float    t      = (float)(now - s_step_start) / BEAT[s_step].duration_ms;
    uint8_t  bright = (uint8_t)(s_start_bright + t * ((int)BEAT[s_step].brightness - (int)s_start_bright));
    ledcWrite(PIN_LED, bright);
}
