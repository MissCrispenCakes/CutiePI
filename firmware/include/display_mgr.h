#pragma once

// Initialises the SPD2010 LCD, LVGL, and the SquareLine heart-animation UI.
// No-op on BOARD_DUALEYE (different display hardware, not yet wired up).
void display_init();

// Must be called every loop iteration — drives the LVGL timer handler.
void display_update();
