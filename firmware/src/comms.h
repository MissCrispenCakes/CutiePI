#pragma once
#include <Arduino.h>

// Initialise ESP-NOW and register the receive callback.
// on_receive is called (from the WiFi task) whenever a message arrives.
void comms_init(void (*on_receive)(bool remote_is_hugged));

// Send this bear's hug state to its configured peer (unicast, not broadcast).
void comms_send(bool is_hugged);
