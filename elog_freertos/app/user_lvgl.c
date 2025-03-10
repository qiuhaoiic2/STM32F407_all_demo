#include "user_lvgl.h"


#define DISP_HOR_RES    240
#define DISP_VER_RES    320
#define DISP_BUF_SIZE   (DISP_HOR_RES * DISP_VER_RES * sizeof(lv_color_t) / 8)

// 确认链接脚本中正确定义了 .ccmram 段
// 在 STM32F4xx_flash.ld 文件中应有类似代码:


static uint8_t gui_buff[DISP_BUF_SIZE];

// 直接使用 CCM RAM 地址映射
// #define CCM_RAM_BASE    0x10000000
// #define CCM_RAM_SIZE    0x10000  // 64KB

// // 创建指针，指向 CCM RAM
// static uint8_t* const gui_buff = (uint8_t*)CCM_RAM_BASE;

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

    lv_display_t *ili9341_disp;
    lv_color_t *color_buf1 = NULL, *color_buf2 = NULL;
    lv_init();
    lv_tick_set_cb(xTaskGetTickCount);
    lv_delay_set_cb(vTaskDelay);

    ili9341_disp = lv_ili9341_create(DISP_HOR_RES, DISP_VER_RES, LV_LCD_FLAG_NONE, ili9341_send_cmd, ili9341_send_color);

    color_buf1 = (lv_color_t *)gui_buff;
    lv_display_set_buffers(ili9341_disp, color_buf1, color_buf2, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
}


typedef struct {
    lv_obj_t *screen;
    lv_obj_t *container;
    lv_obj_t *plus_btn;
    lv_obj_t *minus_btn;
    lv_obj_t *counter_label;
} lv_ui;

void counter_plus_event_cb(lv_event_t *e);
void counter_minus_event_cb(lv_event_t *e);
lv_ui ui;

void create_counter_ui(void) {
    // 创建屏幕
    ui.screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(ui.screen, lv_color_hex(0x599cc7), 0);

    // 创建容器
    ui.container = lv_obj_create(ui.screen);
    lv_obj_set_size(ui.container, 200, 200);
    lv_obj_center(ui.container);
    lv_obj_set_style_radius(ui.container, 10, 0);
    lv_obj_set_style_bg_color(ui.container, lv_color_hex(0xFFFFFF), 0);

    // 创建加号按钮
    ui.plus_btn = lv_btn_create(ui.container);
    lv_obj_set_size(ui.plus_btn, 40, 40);
    lv_obj_align(ui.plus_btn, LV_ALIGN_TOP_RIGHT, -20, 20);
    lv_obj_t *plus_label = lv_label_create(ui.plus_btn);
    lv_label_set_text(plus_label, "+");
    lv_obj_center(plus_label);

    // 创建减号按钮
    ui.minus_btn = lv_btn_create(ui.container);
    lv_obj_set_size(ui.minus_btn, 40, 40);
    lv_obj_align(ui.minus_btn, LV_ALIGN_BOTTOM_RIGHT, -20, -20);
    lv_obj_t *minus_label = lv_label_create(ui.minus_btn);
    lv_label_set_text(minus_label, "-");
    lv_obj_center(minus_label);

    // 创建计数器标签
    ui.counter_label = lv_label_create(ui.container);
    lv_obj_set_style_text_font(ui.counter_label, &lv_font_montserrat_16, 0);
    lv_label_set_text(ui.counter_label, "0");
    lv_obj_center(ui.counter_label);

    // 添加事件处理
    lv_obj_add_event_cb(ui.plus_btn, counter_plus_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui.minus_btn, counter_minus_event_cb, LV_EVENT_CLICKED, NULL);

    // 显示屏幕
    lv_screen_load(ui.screen);
}

static void counter_plus_event_cb(lv_event_t *e) {
    int count = atoi(lv_label_get_text(ui.counter_label));
    count++;
    char buf[16];
    sprintf(buf, "%d", count);
    lv_label_set_text(ui.counter_label, buf);
}

static void counter_minus_event_cb(lv_event_t *e) {
    int count = atoi(lv_label_get_text(ui.counter_label));
    if (count > 0) count--;
    char buf[16];
    sprintf(buf, "%d", count);
    lv_label_set_text(ui.counter_label, buf);
}
