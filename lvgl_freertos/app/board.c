#include "board.h"

/**
 * @brief �弶֧�ְ���ʼ��
 *
 */
void board_lowlevel_init(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // �����ж����ȼ�����4
    // ���� GPIOA �� USART1 ��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
}

led_desc_t led0 = {
    .clk_source = RCC_AHB1Periph_GPIOE,
    .gpio = GPIOE,
    .pin = GPIO_Pin_5,
    .led_on = Bit_RESET,
    .led_off = Bit_SET};


led_desc_t led1 = {
    .clk_source = RCC_AHB1Periph_GPIOE,
    .gpio = GPIOE,
    .pin = GPIO_Pin_6,
    .led_on = Bit_RESET,
    .led_off = Bit_SET};

led_desc_t led2 = {
    .clk_source = RCC_AHB1Periph_GPIOC,
    .gpio = GPIOC,
    .pin = GPIO_Pin_13,
    .led_on = Bit_RESET,
    .led_off = Bit_SET};
