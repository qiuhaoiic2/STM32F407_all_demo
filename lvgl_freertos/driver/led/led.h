#ifndef _DRIVER_LED_LED_H_
#define _DRIVER_LED_LED_H_

#include "main.h"

// ���� LED �������ṹ��
typedef struct led_desc
{
    uint32_t clk_source; // ʱ��Դ
    GPIO_TypeDef * gpio; // GPIO �˿�
    uint16_t pin;        // GPIO ����
    BitAction led_on;    // LED ��ʱ��״̬
    BitAction led_off;   // LED �ر�ʱ��״̬
} led_desc_t;

// ��ʼ�� LED
void Led_init(led_desc_t *led);

// �� LED
void led_on(led_desc_t *led);

// �ر� LED
void led_off(led_desc_t *led);

// ���� LED0 ������
extern led_desc_t led0;

// ���� LED1 ������
extern led_desc_t led1;

// ���� LED2 ������
extern led_desc_t led2;


#endif // _DRIVER_LED_LED_H_
