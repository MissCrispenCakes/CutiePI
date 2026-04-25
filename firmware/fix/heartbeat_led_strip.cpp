#include "heartbeat.h"
#include "config.h"
#include <FastLED.h>

// Addressable LED strip variant (WS2812B or compatible).
//
// Hardware difference from the single-LED version:
//   GPIO13 ─── 330 Ω ─── strip data-in
//   5V    ─────────────── strip power (use a dedicated 5V rail, not the ESP32 3.3V pin)
//   GND   ─────────────── strip GND
//
// platformio.ini — add to lib_deps:
//   fastled/FastLED @ ^3.7.0
//
// PIN_LED (config.h) is reused as the data pin.
// NUM_LEDS controls how many LEDs pulse together as one heartbeat unit.
// Color is red to match the single-LED design intent.

#define NUM_LEDS 8

static CRGB s_leds[NUM_LEDS];

struct Step { uint8_t brightness; uint16_t duration_ms; };

static const Step BEAT[] = {
    { 255, 100 },
    {   0,  80 },
    { 200,  80 },
    {   0, 150 },
};
static const size_t   N_STEPS        = sizeof(BEAT) / sizeof(BEAT[0]);
static const uint32_t BEAT_PERIOD_MS = 60000UL / HEARTBEAT_BPM;

static bool     s_active       = false;
static size_t   s_step         = N_STEPS;
static uint32_t s_step_start   = 0;
static uint32_t s_beat_start   = 0;
static uint8_t  s_start_bright = 0;

static void set_brightness(uint8_t b) {
    fill_solid(s_leds, NUM_LEDS, CRGB(b, 0, 0));
    FastLED.show();
}

void heartbeat_init() {
    FastLED.addLeds<WS2812B, PIN_LED, GRB>(s_leds, NUM_LEDS);
    FastLED.setBrightness(255);
    set_brightness(0);
}

void heartbeat_set_active(bool active) {
    if (active == s_active) return;
    s_active = active;
    if (!active) {
        set_brightness(0);
        s_step       = N_STEPS;
        s_beat_start = 0;
    }
}

void heartbeat_update() {
    if (!s_active) return;

    uint32_t now = millis();

    if (s_beat_start == 0) {
        s_beat_start   = now;
        s_step_start   = now;
        s_step         = 0;
        s_start_bright = 0;
    }

    while (s_step < N_STEPS && (now - s_step_start) >= BEAT[s_step].duration_ms) {
        s_start_bright  = BEAT[s_step].brightness;
        s_step_start   += BEAT[s_step].duration_ms;
        s_step++;
    }

    if (s_step >= N_STEPS) {
        set_brightness(0);
        if (now - s_beat_start >= BEAT_PERIOD_MS) {
            s_beat_start = 0;
        }
        return;
    }

    float   t      = (float)(now - s_step_start) / BEAT[s_step].duration_ms;
    uint8_t bright = (uint8_t)(s_start_bright + t * ((int)BEAT[s_step].brightness - (int)s_start_bright));
    set_brightness(bright);
}
