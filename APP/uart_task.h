#ifndef __BSP_UART_TASK_H__
#define __BSP_UART_TASK_H__

//帧头 帧尾
#define FRAME_HEAD 0xFE
#define FRAME_TAIL 0xFF

//帧状态
#define FRAME_HEAD_STATUS 0x01
#define FRAME_TAIL_STATUS 0x02
#define FRAME_NOT_DETECTED 0x03
#include "FreeRTOS.h"
#include "usart.h"  //UART_HandleTypeDef huart1;
#include "elog.h"

#include <string.h>  // memset
// #include <stdlib.h>
#include "task.h"   // 任务通知函数  xTaskNotifyFromISR  MAX_DELAY
#include "queue.h" 	//队列
#include "mid_circle_buffer.h"//循环缓冲区
#include "bsp_uart_driver.h"

void uart_rec_A_func(void *arg);

#endif // end __BSP_UART_TASK_H__
