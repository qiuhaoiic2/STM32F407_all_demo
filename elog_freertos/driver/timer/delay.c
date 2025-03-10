#include "delay.h"


void Timer3_Init(uint32_t arr,uint32_t pre)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = arr - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseStructure.TIM_Prescaler = pre - 1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
}

// TIM频率计算公式  F = Fpclk / ((TIM_Prescaler + 1) * (TIM_Period + 1))
// 对于STM32F407ZGT6，定时器的频率为84Mhz 1us = 1MHZ ;;;
// F = 84 000 000 / 84 / 1 = 1 000 000 = 1MHZ(1us)
void delay_init(void)
{
    Timer3_Init(1000, 84);
}
void delay_us(uint32_t nus)
{
    TIM_SetAutoreload(TIM3, nus);//设置自动重装载寄存器周期的值
    TIM_SetCounter(TIM3, nus);//设置计数器的值
    TIM_Cmd(TIM3, ENABLE);
    while (TIM_GetCounter(TIM3) != 0);
    TIM_Cmd(TIM3, DISABLE);
}

void delay_ms(uint16_t nms)
{
    while (nms--)
    {
        delay_us(1000);
    }
}
