#include "main.h"


ringbuffer8_t rb;
uint8_t rb_buff[128];
dht11_t dht11;

SemaphoreHandle_t uart1_dma_txdone;





SemaphoreHandle_t uart1_dma_mutex;
void led_task(void *para);
void usart_task(void *para);
void gui_task(void * para);
void dht11_task(void *para);
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
    //������ֵ�ź���������ͬ��DMA2�жϺ������ź�
    uart1_dma_txdone = xSemaphoreCreateBinary();
    configASSERT(uart1_dma_txdone);
    //���������źţ���������������ʹ�ô��ڷ��Ͷ��������ݶ�ʧ������
    uart1_dma_mutex = xSemaphoreCreateMutex();
    configASSERT(uart1_dma_mutex);

    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t)usart_task, "task1", 			128,  NULL,  6, NULL);
    xTaskCreate((TaskFunction_t)usart_task, "task2", 			128,  NULL,  6, NULL);
    xTaskCreate((TaskFunction_t)led_task,   "led0",     	    128,  &led0, 5, NULL);
    xTaskCreate((TaskFunction_t)gui_task,   "gui_task",         1024, NULL,  5, NULL);
    xTaskCreate((TaskFunction_t)dht11_task, "dht11_task",       128,  NULL,  7, NULL);
    taskEXIT_CRITICAL();
    vTaskStartScheduler();

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
    lv_obj_t *screen = lv_screen_active(); // ��ȡ��ǰ�����Ļ
    lv_obj_t *label = lv_label_create(screen);
    gui_ili9341_init();
    lv_obj_set_style_bg_color(screen, lv_color_make(0x00, 0x00, 0x00), 0);// ���õ�ǰ��Ļ������ɫ
    // ����ǩ������ɫ����Ϊ��ɫ
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_RED), 0);// ���ñ�ǩ������ɫ

    lv_obj_set_align(label, LV_ALIGN_TOP_LEFT);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, "this is a system to display the temperature and humidity");
    lv_obj_set_pos(label, 0, 32);
    while (1)
    {
        vTaskDelay(100);
        //��ʾ��ʪ��
        sprintf(buff, "temperature: %d\nhumidity: %d\n", dht11.temperature, dht11.humidity);
        lv_label_set_text(label, buff);//��ʾ�ڵڶ���
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
/**
 * @brief FreeRTOS ����������
 *
 * @param para
 */
void usart_task(void *para)
{
    uint8_t send_buff[128];
    while (1)
    {

        vTaskDelay(pdMS_TO_TICKS(1000));
        xSemaphoreTake(uart1_dma_mutex, portMAX_DELAY);
        sprintf((char *)send_buff,"[%u] %s is running\n",xTaskGetTickCount(),pcTaskGetName(NULL));
        uart1_dma_write(send_buff,strlen((char *)send_buff));
        xSemaphoreTake(uart1_dma_txdone, portMAX_DELAY);
				xSemaphoreGive(uart1_dma_mutex);

    }
}

int main(void)
{
    board_lowlevel_init();                          // ��ʼ���弶֧�ְ�
    uart_init();
    delay_init();                                   // ��ʼ����ʱ����
    Led_init(&led0);                                // ��ʼ�� LED0
    Led_init(&led1);                                // ��ʼ�� LED1
    Led_init(&led2);                                // ��ʼ�� LED2
    LCD_Init();                                     // ��ʼ�� LCD
    dht11_init(&dht11, GPIOB, GPIO_Pin_11);         // ��ʼ�� DHT11
    user_create_task();                             // �����û�����
    while (1);;
}
/**
 * @brief DMA2_Stream7 �жϴ�����
 *
 */
void DMA2_Stream7_IRQHandler()
{
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7))
    {
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
        DMA_Cmd(DMA2_Stream7, DISABLE);
        xSemaphoreGiveFromISR(uart1_dma_txdone,&pxHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
    }
}

