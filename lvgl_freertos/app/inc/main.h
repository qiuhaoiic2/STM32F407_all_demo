#ifndef _MAIN_H_
#define _MAIN_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "board.h"
#include "uart.h"
#include "shell.h"
#include "tlsf.h"
#include "shell_command.h"
#include "ringbuffer8.h"

#include "dht11.h"
#include "lcd.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "lvgl.h"
#include "src/drivers/display/ili9341/lv_ili9341.h"
#include "user_lvgl.h"


#endif /*_MAIN_H_ */
