#ifndef _DRIVER_UART_UART_H_
#define _DRIVER_UART_UART_H_


#include "main.h"

typedef void (*uart1_callback_t)(uint8_t data);
void uart_init(void);
void uart1_callback_resigter(uart1_callback_t callback);
void uart1_send_data(uint8_t data);
void uart1_send_string(char *str);
void uart1_dma_write(const uint8_t *buff,uint16_t length);
#endif  // _DRIVER_UART_UART_H_
