#include "uart.h"



uart1_callback_t uart1_callback = NULL;


void uart1_callback_resigter(uart1_callback_t callback)
{
    uart1_callback = callback;
}

// ��ʼ�� UART ����
static void uart_pin_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // ���� GPIOA ������ 9 �� 10 Ϊ���ù���
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // �� GPIOA ������ 9 �� 10 ����Ϊ USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
}

// ��ʼ�� UART �ĵͼ�����
static void uart_lowlevel_init(uint32_t bound)
{
    USART_InitTypeDef USART_InitStructure;

    // ���� USART1 �Ĳ����ʡ��ֳ���ֹͣλ��У��λ��ģʽ��Ӳ��������
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); // ʹ�ܴ���1��DMA����
    // ʹ�� USART1
    USART_Cmd(USART1, ENABLE);
}



// ��ʼ�� UART ���ж�
static void uart_irq_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    // ���� USART1 �ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ʹ�� USART1 �Ľ����ж�
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

/**
 * @brief ����DMA�����ж�����
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
    //��ʼ��DMA���ڷ��ͣ�DMA2��ͨ��4����7 ����DMA2ӳ�����ȷ��
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
 * @brief ���ڳ�ʼ��
 *
 */
void uart_init(void)
{
    uart_pin_init();       // ��ʼ�� UART ����
    uart_lowlevel_init(115200);  // ��ʼ�� UART �ĵͼ�����
    uart_dma_init();       // ��ʼ�� UART �� DMA����

    uart_irq_init();       // ��ʼ�� UART ���ж�
}
/**
 * @brief ����DMA�����ж�
 *
 * @param buff �������ݻ�����
 * @param length �������ݳ���
 */
void uart1_dma_write(const uint8_t *buff,uint16_t length)
{
    
    DMA_SetCurrDataCounter(DMA2_Stream7, length);
    DMA_MemoryTargetConfig(DMA2_Stream7, (uint32_t) buff, DMA_Memory_0);
    DMA_Cmd(DMA2_Stream7, ENABLE);

}
void uart1_send_data(uint8_t data)
{
    // ����һ���ֽڵ�����
    USART_SendData(USART1, data);

    // �ȴ��������
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//TC����1����ʾ�������
}

void uart1_send_string(char *str)
{
    // �����ַ���
    while(*str)
    {
        uart1_send_data(*str++);
    }
}
int fputc(int ch, FILE *f)
{

    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); //txe����0����ʾ���ͻ�����Ϊ��
    // ����һ���ֽڵ�����
    USART_SendData(USART1, (uint8_t) ch);

    // �ȴ��������
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//TC����1����ʾ�������

    return ch;
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        // ���յ�����
        uint8_t data = USART_ReceiveData(USART1);
        if (uart1_callback != NULL)
        {
            uart1_callback(data);
        }
    }
}


