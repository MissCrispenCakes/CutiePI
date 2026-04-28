#include "display_mgr.h"

#ifndef BOARD_DUALEYE

#include "I2C_Driver.h"
#include "TCA9554PWR.h"
#include "Display_SPD2010.h"
#include "LVGL_Driver.h"
extern "C" {
    #include "ui.h"
}

void display_init() {
    I2C_Init();
    TCA9554PWR_Init(0x00);  // all pins output, all LOW
    Backlight_Init();
    Set_Backlight(70);       // 70 % brightness
    LCD_Init();
    Lvgl_Init();
    ui_init();
    Serial.println("Display: heart animation ready");
}

void display_update() {
    Lvgl_Loop();
}

#else

// DualEye 1.28 has a different display — stubs so main.cpp needs no ifdefs.
void display_init()  {}
void display_update() {}

#endif
