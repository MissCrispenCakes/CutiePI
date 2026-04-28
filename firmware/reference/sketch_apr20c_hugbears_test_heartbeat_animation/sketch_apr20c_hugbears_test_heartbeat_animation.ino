/*
 * Heart animation display — minimal sketch
 * Calls the SquareLine-exported UI and lets LVGL run its animations.
 */

#include "Display_SPD2010.h"   // LCD driver for this board
#include "LVGL_Driver.h"       // LVGL init/loop helpers
#include "ui.h"                // SquareLine-exported UI (defines ui_init())

void setup()
{
  Serial.begin(115200);
  delay(200);
  Serial.println("Heart animation boot");

  I2C_Init();
  TCA9554PWR_Init(0x00);
  Backlight_Init();
  Set_Backlight(50);  // 0-100
  Serial.println("I2C + expander + backlight initialised");

  LCD_Init();
  Serial.println("LCD initialised");

  Lvgl_Init();
  Serial.println("LVGL initialised");

  ui_init();
  Serial.println("UI initialised — heart should be beating");
}

void loop()
{
  Lvgl_Loop();
  vTaskDelay(pdMS_TO_TICKS(5));
}