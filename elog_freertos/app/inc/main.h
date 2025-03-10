#ifndef _MAIN_H_
#define _MAIN_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

//STM32头文件
#include "stm32f4xx.h"

//用户头文件
#include "delay.h"
#include "led.h"
#include "board.h"
#include "uart.h"

//TLSF头文件
#include "tlsf.h"

//letter shell头文件
#include "shell.h"
#include "shell_command.h"

//环形缓冲区头文件
#include "ringbuffer8.h"

//LCD 和 DHT11 头文件
#include "dht11.h"
#include "lcd.h"

//FreeRTOS头文件
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


//lvgl头文件
#include "lvgl.h"
#include "src/drivers/display/ili9341/lv_ili9341.h"
#include "user_lvgl.h"

//EasyLogger头文件
#include "elog.h"

#endif /*_MAIN_H_ */
