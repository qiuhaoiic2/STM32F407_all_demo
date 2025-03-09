#ifndef _DRIVER_LED_LED_H_
#define _DRIVER_LED_LED_H_

#include "main.h"

// 定义 LED 描述符结构体
typedef struct led_desc
{
    uint32_t clk_source; // 时钟源
    GPIO_TypeDef * gpio; // GPIO 端口
    uint16_t pin;        // GPIO 引脚
    BitAction led_on;    // LED 打开时的状态
    BitAction led_off;   // LED 关闭时的状态
} led_desc_t;

// 初始化 LED
void Led_init(led_desc_t *led);

// 打开 LED
void led_on(led_desc_t *led);

// 关闭 LED
void led_off(led_desc_t *led);

// 声明 LED0 描述符
extern led_desc_t led0;

// 声明 LED1 描述符
extern led_desc_t led1;

// 声明 LED2 描述符
extern led_desc_t led2;


#endif // _DRIVER_LED_LED_H_
