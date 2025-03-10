#ifndef _MAIN_H_
#define _MAIN_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

//STM32ͷ�ļ�
#include "stm32f4xx.h"

//�û�ͷ�ļ�
#include "delay.h"
#include "led.h"
#include "board.h"
#include "uart.h"

//TLSFͷ�ļ�
#include "tlsf.h"

//letter shellͷ�ļ�
#include "shell.h"
#include "shell_command.h"

//���λ�����ͷ�ļ�
#include "ringbuffer8.h"

//LCD �� DHT11 ͷ�ļ�
#include "dht11.h"
#include "lcd.h"

//FreeRTOSͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


//lvglͷ�ļ�
#include "lvgl.h"
#include "src/drivers/display/ili9341/lv_ili9341.h"
#include "user_lvgl.h"

//EasyLoggerͷ�ļ�
#include "elog.h"

#endif /*_MAIN_H_ */
