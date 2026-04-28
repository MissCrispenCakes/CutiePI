#include <Arduino.h>
#include "config.h"
#include "power_mgr.h"
#include "comms.h"
#include "sensors.h"
#include "heartbeat.h"
#include "actuators.h"
#include "temp_sensor.h"
#include "display_mgr.h"

// ── Bear State Machine ────────────────────────────────────────────────────────
enum BearState {
    STATE_IDLE,        // nobody is hugging
    STATE_LOCAL_HUG,   // only this bear is being hugged
    STATE_REMOTE_HUG,  // only the other bear is being hugged  → show heartbeat
    STATE_MUTUAL_HUG,  // both bears are being hugged          → heartbeat + warmth
};

static BearState s_state          = STATE_IDLE;
static bool      s_prev_local_hug = false;

// volatile: in ESP-NOW mode this is written by the WiFi task and read by loop().
// In MQTT mode both writes and reads happen on the main thread (via comms_update),
// but volatile is harmless and keeps the declaration correct for both builds.
static volatile bool s_remote_hugged = false;

static void on_remote_hug(bool remote_is_hugged) {
    s_remote_hugged = remote_is_hugged;
}

static const char* state_name(BearState s) {
    switch (s) {
        case STATE_IDLE:       return "IDLE";
        case STATE_LOCAL_HUG:  return "LOCAL_HUG";
        case STATE_REMOTE_HUG: return "REMOTE_HUG";
        case STATE_MUTUAL_HUG: return "MUTUAL_HUG";
        default:               return "UNKNOWN";
    }
}

static BearState resolve_state(bool local, bool remote) {
    if (local && remote) return STATE_MUTUAL_HUG;
    if (local)           return STATE_LOCAL_HUG;
    if (remote)          return STATE_REMOTE_HUG;
    return STATE_IDLE;
}

static void apply_state(BearState state) {
    switch (state) {
        case STATE_IDLE:
            heartbeat_set_active(false);
            actuators_set_heater(false);
            break;

        case STATE_LOCAL_HUG:
            // This bear is squeezed — signal sent, waiting for mutual hug.
            // No local feedback yet; the other bear will show its heartbeat.
            heartbeat_set_active(false);
            actuators_set_heater(false);
            break;

        case STATE_REMOTE_HUG:
            // Someone is hugging the other bear — pulse the LED as a heartbeat.
            heartbeat_set_active(true);
            actuators_set_heater(false);
            break;

        case STATE_MUTUAL_HUG:
            // Both bears are hugged — heartbeat + heating pad warmth.
            heartbeat_set_active(true);
            actuators_set_heater(true);
            break;
    }
}

// ── Arduino Entry Points ──────────────────────────────────────────────────────
// GPIO5 is the LCD backlight pin. The 3-flash here is a low-level boot indicator
// that fires before LVGL loads — if the display never shows the heart but you
// see the flashes, the problem is in display_init(), not power_mgr_init().
static constexpr uint8_t LCD_BL_PIN = 5;

void setup() {
    power_mgr_init();  // first: latch board power on (GPIO7 HIGH)

    // Boot indicator: flash backlight 3× immediately after power latch.
    // Proves the board is alive on battery before any complex init runs.
    pinMode(LCD_BL_PIN, OUTPUT);
    for (int i = 0; i < 3; i++) {
        digitalWrite(LCD_BL_PIN, HIGH); delay(200);
        digitalWrite(LCD_BL_PIN, LOW);  delay(200);
    }

    Serial.begin(115200);
    delay(500);

    sensors_init();
    heartbeat_init();
    actuators_init();
    temp_sensor_init();
    display_init();    // LCD + LVGL + heart animation
    comms_init(on_remote_hug);
    Serial.printf("CutiePI Bear %d ready! Battery: %.2f V (%d%%)\n",
                  BEAR_ID, power_mgr_battery_volts(), power_mgr_battery_pct());
}

void loop() {
    power_mgr_update();
    float temp_c       = temp_sensor_read_celsius();
    bool local_hugged  = sensors_is_hugged();
    bool remote_hugged = s_remote_hugged;  // snapshot volatile once per iteration

    // Transmit only on state change to avoid flooding
    if (local_hugged != s_prev_local_hug) {
        comms_send(local_hugged);
        s_prev_local_hug = local_hugged;
    }

    BearState new_state = resolve_state(local_hugged, remote_hugged);
    if (new_state != s_state) {
        Serial.printf("State: %s → %s\n", state_name(s_state), state_name(new_state));
        s_state = new_state;
        apply_state(s_state);
    }

    heartbeat_update();
    actuators_update(temp_c);
    comms_update();
    display_update();
}
