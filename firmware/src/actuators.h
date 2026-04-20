#pragma once
#include <Arduino.h>

void actuators_init();
void actuators_set_heater(bool on);
void actuators_update();  // must be called every loop() — enforces safety cutoff
