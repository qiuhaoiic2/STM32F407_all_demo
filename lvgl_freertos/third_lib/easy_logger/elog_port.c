/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */

#include <elog.h>



static SemaphoreHandle_t elog_mutex = NULL;
static SemaphoreHandle_t elog_async_sem = NULL;
/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {


    ElogErrCode result = ELOG_NO_ERR;
    elog_async_sem = xSemaphoreCreateBinary();
    elog_mutex = xSemaphoreCreateMutex();
    if (elog_mutex == NULL) {
        result = ELOG_ERR;
    }
    if(elog_async_sem == NULL){
        result = ELOG_ERR;
    }
    /* add your code here */
    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) {

    /* add your code here */

}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {

    printf("%.*s", size, log);
    /* add your code here */

}

/**
 * output lock
 */
void elog_port_output_lock(void) {
    //如果elog_mutex不为空，则获取互斥锁，这样保证只有一个任务可以输出日志
    if (elog_mutex != NULL) {
        xSemaphoreTake(elog_mutex, portMAX_DELAY);
    }
    /* add your code here */

}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    //如果elog_mutex不为空，则释放互斥锁，这样其他任务就可以获取互斥锁
    if (elog_mutex != NULL) {
        xSemaphoreGive(elog_mutex);
    }
    /* add your code here */
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    //定义一个静态的时间缓冲区
    static char time_buff[16] = {0};
    uint32_t tick = xTaskGetTickCount();

    // 转换为可读时间格式
    uint32_t milliseconds = tick % configTICK_RATE_HZ;
    uint32_t seconds = (tick / configTICK_RATE_HZ) % 60;
    uint32_t minutes = (tick / (configTICK_RATE_HZ * 60)) % 60;
    uint32_t hours = (tick / (configTICK_RATE_HZ * 3600)) % 24;

    // 格式化时间字符串为 "HH:MM:SS.mmm"
    snprintf(time_buff, sizeof(time_buff), "%02lu:%02lu:%02lu.%03lu",
             hours, minutes, seconds,
             milliseconds * 1000 / configTICK_RATE_HZ); // 转换为毫秒

    return time_buff;
    /* add your code here */

}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {

    /* add your code here */
    return "";
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {

    /* add your code here */
    return "";
}

void elog_async_output_notice(void) {

    if (elog_async_sem != NULL) {
        xSemaphoreGive(elog_async_sem);
    }
    /* add your code here */
}
