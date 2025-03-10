#include "led.h"



void Led_init(led_desc_t *led)
{
    // ���� GPIOE ��ʱ��
    RCC_AHB1PeriphClockCmd(led->clk_source, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    // �� GPIO_InitStructure �ṹ������
    memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitStructure));
    // ���� GPIO ģʽΪ���ģʽ
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    // ���� GPIO ����
    GPIO_InitStructure.GPIO_Pin     = led->pin;
    // ���� GPIO �ٶ�Ϊ 50MHz
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    // ���� GPIO �������Ϊ�������
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    // ���� GPIO ����
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;

    // ��ʼ�� GPIO
    GPIO_Init(led->gpio, &GPIO_InitStructure);

    led_off(led);
}

void led_on(led_desc_t *led)
{
    // ���� GPIO ����Ϊ�ߵ�ƽ������ LED
    GPIO_WriteBit(led->gpio, led->pin, led->led_on);
}
void led_off(led_desc_t *led)
{
    // ���� GPIO ����Ϊ�͵�ƽ��Ϩ�� LED
    GPIO_WriteBit(led->gpio, led->pin, led->led_off);
}

