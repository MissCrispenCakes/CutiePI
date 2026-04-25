#pragma once

// Call power_mgr_init() as the FIRST line of setup().
// GPIO7 (PWR_Control_PIN) must be driven HIGH within ~100 ms of boot or the
// Waveshare board's power circuit will cut power to the ESP32.
void  power_mgr_init();

// Call every loop() iteration. Monitors the power button and logs battery.
void  power_mgr_update();

float power_mgr_battery_volts();
int   power_mgr_battery_pct();   // 0–100, clamped
