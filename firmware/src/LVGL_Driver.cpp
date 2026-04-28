#include "LVGL_Driver.h"
#include <esp_heap_caps.h>

static lv_disp_draw_buf_t draw_buf;

// Allocate draw buffers in PSRAM — frees ~67 KB of internal SRAM.
// heap_caps_malloc is called in Lvgl_Init so the PSRAM heap is ready.
static lv_color_t* buf1 = nullptr;
static lv_color_t* buf2 = nullptr;

void Lvgl_port_rounder_callback(struct _lv_disp_drv_t * disp_drv, lv_area_t * area)
{
  uint16_t x1 = area->x1;
  uint16_t x2 = area->x2;
  area->x1 = (x1 >> 2) << 2;
  area->x2 = ((x2 >> 2) << 2) + 3;
}

void Lvgl_Display_LCD( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p )
{
  LCD_addWindow(area->x1, area->y1, area->x2, area->y2, (uint16_t *)&color_p->full);
  lv_disp_flush_ready(disp_drv);
}

void Lvgl_Touchpad_Read( lv_indev_drv_t * indev_drv, lv_indev_data_t * data )
{
  bool tp_pressed = false;
  uint16_t tp_x = 0;
  uint16_t tp_y = 0;
  uint8_t tp_cnt = 0;
  tp_pressed = Touch_Get_xy(&tp_x, &tp_y, NULL, &tp_cnt, CONFIG_ESP_LCD_TOUCH_MAX_POINTS);
  if (tp_pressed && (tp_cnt > 0)) {
    data->point.x = tp_x;
    data->point.y = tp_y;
    data->state = LV_INDEV_STATE_PR;
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void example_increase_lvgl_tick(void *arg)
{
  lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

void Lvgl_Init(void)
{
  buf1 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
  buf2 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);

  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LVGL_BUF_LEN);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = LCD_WIDTH;
  disp_drv.ver_res = LCD_HEIGHT;
  disp_drv.flush_cb = Lvgl_Display_LCD;
  disp_drv.rounder_cb = Lvgl_port_rounder_callback;
  disp_drv.full_refresh = 1;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = Lvgl_Touchpad_Read;
  lv_indev_drv_register(&indev_drv);

  const esp_timer_create_args_t lvgl_tick_timer_args = {
    .callback = &example_increase_lvgl_tick,
    .name = "lvgl_tick"
  };
  esp_timer_handle_t lvgl_tick_timer = NULL;
  esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
  esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);
}

void Lvgl_Loop(void)
{
  lv_timer_handler();
}
