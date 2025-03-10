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
    board_lowlevel_init();                          // ��ʼ���弶֧�ְ�
    uart_init();                                    // ��ʼ������
    buff_init();                                    // ��ʼ��������
    delay_init();                                   // ��ʼ����ʱ����
    Led_init(&led0);                                // ��ʼ�� LED0
    Led_init(&led1);                                // ��ʼ�� LED1
    Led_init(&led2);                                // ��ʼ�� LED2
    LCD_Init();                                     // ��ʼ�� LCD
    dht11_init(&dht11, GPIOB, GPIO_Pin_11);         // ��ʼ�� DHT11
    my_elog_init();                                    // ��ʼ�� EasyLogger
    user_create_task();                             // �����û�����
    while (1);;
}



void usart1_receive_handle(uint8_t data)
{
    if (rb8_full(rb) != true) // ��� ringbuffer8 ����
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
     lv_obj_t *screen = lv_screen_active(); // ��ȡ��ǰ�����Ļ
     lv_obj_t *label = lv_label_create(screen);

     lv_obj_set_style_bg_color(screen, lv_color_make(0x00, 0x00, 0x00), 0);// ���õ�ǰ��Ļ������ɫ
     // ����ǩ������ɫ����Ϊ��ɫ
     lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_RED), 0);// ���ñ�ǩ������ɫ

     lv_obj_set_align(label, LV_ALIGN_TOP_LEFT);
     lv_obj_set_width(label, 240);
     lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
     while (1)
     {
         vTaskDelay(pdMS_TO_TICKS(100));

        //  ��ʾ��ʪ��
         sprintf(buff, " This is a sys for FreeRTOS and LVGL\ntemperature: %d\nhumidity: %d\n", dht11.temperature, dht11.humidity);
        lv_label_set_text(label, buff);//��ʾ�ڵڶ���

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
    // ������ʼ��
    if (elog_init() != ELOG_NO_ERR)
    {
        printf("elog init failed\n");
        return;
    }
    // ���ø�����������ʽ - ��� INFO ��������
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);

    // ������־����
    elog_set_filter_lvl(ELOG_LVL_INFO);    // ֻ��ʾ INFO �����ϼ�����־

    // ��������ն˲�֧����ɫ�����Խ�����
    elog_set_text_color_enabled(false);

    // ������־
    elog_start();

    // ������־
    elog_i("my_elog_init", "EasyLogger initialized");
}
