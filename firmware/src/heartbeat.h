#pragma once
#include <Arduino.h>

void heartbeat_init();
void heartbeat_set_active(bool active);
void heartbeat_update();  // must be called every loop()
