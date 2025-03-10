#include "main.h"


ringbuffer8_t rb;
uint8_t rb_buff[128];
dht11_t dht11;

void led_task(void *para);
void gui_task(void * para);
void dht11_task(void *para);
void log_task(void *para);
void user_create_task(void);
void my_elog_init(void);
void buff_init(void);

int main(void)
{
    board_lowlevel_init();                          // 初始化板级支持包
    uart_init();                                    // 初始化串口
    buff_init();                                    // 初始化缓冲区
    delay_init();                                   // 初始化延时函数
    Led_init(&led0);                                // 初始化 LED0
    Led_init(&led1);                                // 初始化 LED1
    Led_init(&led2);                                // 初始化 LED2
    LCD_Init();                                     // 初始化 LCD
    dht11_init(&dht11, GPIOB, GPIO_Pin_11);         // 初始化 DHT11
    my_elog_init();                                    // 初始化 EasyLogger
    user_create_task();                             // 创建用户任务
    while (1);;
}



void usart1_receive_handle(uint8_t data)
{
    if (rb8_full(rb) != true) // 如果 ringbuffer8 不满
        rb8_put(rb, data);

}
void buff_init(void)
{
    rb = rb8_new(rb_buff, sizeof(rb_buff));
}

void user_create_task()
{

    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t)led_task,   "led0",     	    128,  &led0, 5, NULL);
    xTaskCreate((TaskFunction_t)gui_task,   "gui_task",         4096, NULL,  6, NULL);
    xTaskCreate((TaskFunction_t)log_task,   "log_task",         512, NULL,  5, NULL);
    xTaskCreate((TaskFunction_t)dht11_task, "dht11_task",       128,  NULL,  7, NULL);
    taskEXIT_CRITICAL();
    vTaskStartScheduler();

}

void log_task(void *para)
{

    while (1)
    {
				elog_i("log_task","temperature: %d\thumidity: %d", dht11.temperature, dht11.humidity);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void led_task(void *para)
{
    while (1)
    {
        led_on(para);
        vTaskDelay(pdMS_TO_TICKS(1000));
        led_off(para);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}

 void gui_task(void * para)
 {
     char buff[128];
		 gui_ili9341_init();
     lv_obj_t *screen = lv_screen_active(); // 获取当前活动的屏幕
     lv_obj_t *label = lv_label_create(screen);

     lv_obj_set_style_bg_color(screen, lv_color_make(0x00, 0x00, 0x00), 0);// 设置当前屏幕背景颜色
     // 将标签字体颜色设置为红色
     lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_RED), 0);// 设置标签字体颜色

     lv_obj_set_align(label, LV_ALIGN_TOP_LEFT);
     lv_obj_set_width(label, 240);
     lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
     while (1)
     {
         vTaskDelay(pdMS_TO_TICKS(100));

        //  显示温湿度
         sprintf(buff, " This is a sys for FreeRTOS and LVGL\ntemperature: %d\nhumidity: %d\n", dht11.temperature, dht11.humidity);
        lv_label_set_text(label, buff);//显示在第二排

        //   create_counter_ui();
         lv_timer_handler();
     }
 }
void dht11_task(void *para)
{
    while (1)
    {
        dht11_readdata(&dht11);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}





void my_elog_init(void)
{
    // 基础初始化
    if (elog_init() != ELOG_NO_ERR)
    {
        printf("elog init failed\n");
        return;
    }
    // 设置各级别的输出格式 - 添加 INFO 级别设置
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);

    // 设置日志级别
    elog_set_filter_lvl(ELOG_LVL_INFO);    // 只显示 INFO 及以上级别日志

    // 如果您的终端不支持颜色，可以禁用它
    elog_set_text_color_enabled(false);

    // 开启日志
    elog_start();

    // 测试日志
    elog_i("my_elog_init", "EasyLogger initialized");
}
