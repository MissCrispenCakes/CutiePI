#pragma once

void  temp_sensor_init();

// Returns temperature in Celsius. Returns -1.0f on sensor error (disconnected
// or ADC returned 0). Caller should treat negative values as "unknown".
float temp_sensor_read_celsius();
