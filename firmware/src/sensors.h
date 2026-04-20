#pragma once
#include <Arduino.h>

void sensors_init();

// Returns true when sustained pressure above PRESSURE_THRESHOLD is detected.
bool sensors_is_hugged();
