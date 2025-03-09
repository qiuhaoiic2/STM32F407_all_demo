#include "led.h"



void Led_init(led_desc_t *led)
{
    // 启用 GPIOE 的时钟
    RCC_AHB1PeriphClockCmd(led->clk_source, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    // 将 GPIO_InitStructure 结构体清零
    memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitStructure));
    // 设置 GPIO 模式为输出模式
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    // 设置 GPIO 引脚
    GPIO_InitStructure.GPIO_Pin     = led->pin;
    // 设置 GPIO 速度为 50MHz
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    // 设置 GPIO 输出类型为推挽输出
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    // 设置 GPIO 上拉
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;

    // 初始化 GPIO
    GPIO_Init(led->gpio, &GPIO_InitStructure);

    led_off(led);
}

void led_on(led_desc_t *led)
{
    // 设置 GPIO 引脚为高电平，点亮 LED
    GPIO_WriteBit(led->gpio, led->pin, led->led_on);
}
void led_off(led_desc_t *led)
{
    // 设置 GPIO 引脚为低电平，熄灭 LED
    GPIO_WriteBit(led->gpio, led->pin, led->led_off);
}

