#include "uart.h"



uart1_callback_t uart1_callback = NULL;


void uart1_callback_resigter(uart1_callback_t callback)
{
    uart1_callback = callback;
}

// 初始化 UART 引脚
static void uart_pin_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置 GPIOA 的引脚 9 和 10 为复用功能
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 将 GPIOA 的引脚 9 和 10 复用为 USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
}

// 初始化 UART 的低级配置
static void uart_lowlevel_init(uint32_t bound)
{
    USART_InitTypeDef USART_InitStructure;

    // 配置 USART1 的波特率、字长、停止位、校验位、模式和硬件流控制
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); // 使能串口1的DMA发送
    // 使能 USART1
    USART_Cmd(USART1, ENABLE);
}



// 初始化 UART 的中断
static void uart_irq_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    // 配置 USART1 中断
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能 USART1 的接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

/**
 * @brief 串口DMA发送中断配置
 *
 */
static void uart_dma_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //初始化DMA串口发送，DMA2，通道4，流7 根据DMA2映射表来确定
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = 0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);
    DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA2_Stream7, DISABLE);


}

/**
 * @brief 串口初始化
 *
 */
void uart_init(void)
{
    uart_pin_init();       // 初始化 UART 引脚
    uart_lowlevel_init(115200);  // 初始化 UART 的低级配置
    uart_dma_init();       // 初始化 UART 的 DMA发送

    uart_irq_init();       // 初始化 UART 的中断
}
/**
 * @brief 串口DMA发送中断
 *
 * @param buff 发送数据缓冲区
 * @param length 发送数据长度
 */
void uart1_dma_write(const uint8_t *buff,uint16_t length)
{
    
    DMA_SetCurrDataCounter(DMA2_Stream7, length);
    DMA_MemoryTargetConfig(DMA2_Stream7, (uint32_t) buff, DMA_Memory_0);
    DMA_Cmd(DMA2_Stream7, ENABLE);

}
void uart1_send_data(uint8_t data)
{
    // 发送一个字节的数据
    USART_SendData(USART1, data);

    // 等待发送完成
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//TC等于1，表示发送完成
}

void uart1_send_string(char *str)
{
    // 发送字符串
    while(*str)
    {
        uart1_send_data(*str++);
    }
}
int fputc(int ch, FILE *f)
{

    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); //txe等于0，表示发送缓冲区为空
    // 发送一个字节的数据
    USART_SendData(USART1, (uint8_t) ch);

    // 等待发送完成
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//TC等于1，表示发送完成

    return ch;
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        // 接收到数据
        uint8_t data = USART_ReceiveData(USART1);
        if (uart1_callback != NULL)
        {
            uart1_callback(data);
        }
    }
}


