#pragma once
#include <Arduino.h>

// Initialise comms (ESP-NOW or MQTT depending on build) and register callback.
// on_receive is called whenever a hug state arrives from the other bear.
void comms_init(void (*on_receive)(bool remote_is_hugged));

// Send this bear's hug state to the other bear.
void comms_send(bool is_hugged);

// Must be called every loop(). No-op for ESP-NOW; drives MQTT client for MQTT.
void comms_update();
