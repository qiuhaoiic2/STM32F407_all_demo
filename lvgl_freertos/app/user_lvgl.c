#include "user_lvgl.h"


#define DISP_HOR_RES    240
#define DISP_VER_RES    320
#define DISP_BUF_SIZE   (DISP_HOR_RES * DISP_VER_RES * sizeof(lv_color_t) / 8)



static lv_display_t *ili9341_disp;
static uint8_t gui_buff[DISP_BUF_SIZE];


static inline void ili9341_write_reg(uint16_t reg) //使用inline关键词，可以减少函数调用的开销
{
    reg = reg;
    LCD->LCD_REG = reg;
}

static inline void ili9341_write_data(uint16_t data)
{
    data = data;
    LCD->LCD_RAM = data;
}

static void ili9341_send_cmd(lv_display_t * disp, const uint8_t * cmd, size_t cmd_size, const uint8_t * param, size_t param_size)
{
    for (uint32_t i = 0; i < cmd_size; i++)
    {
        ili9341_write_reg(cmd[i]);
    }

    for (uint32_t i = 0; i < param_size; i++)
    {
        ili9341_write_data(param[i]);
    }
}

static void ili9341_send_color(lv_display_t * disp, const uint8_t * cmd, size_t cmd_size, uint8_t * param, size_t param_size)
{
    for (uint32_t i = 0; i < cmd_size; i++)
    {
        ili9341_write_reg(cmd[i]);
    }

    for (uint32_t i = 0; i < param_size; i+=2)
    {
        ili9341_write_data(*(uint16_t *)&param[i]);
    }

    lv_display_flush_ready(disp);
}

void gui_ili9341_init(void)
{
    lv_init();
    lv_tick_set_cb(xTaskGetTickCount);
    lv_delay_set_cb(vTaskDelay);

    ili9341_disp = lv_ili9341_create(DISP_HOR_RES, DISP_VER_RES, LV_LCD_FLAG_NONE, ili9341_send_cmd, ili9341_send_color);

    lv_color_t *color_buf1 = NULL, *color_buf2 = NULL;
    color_buf1 = (lv_color_t *)gui_buff;
    lv_display_set_buffers(ili9341_disp, color_buf1, color_buf2, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
}
