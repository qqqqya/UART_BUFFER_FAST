#ifndef __BSP_UART_DRIVER_H__
#define __BSP_UART_DRIVER_H__

#define queue_size 2
#include "FreeRTOS.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"  //UART_HandleTypeDef huart1;
#include "elog.h"


#include <string.h>  // memset
// #include <stdlib.h>
#include "task.h"   // 任务通知函数  xTaskNotifyFromISR  MAX_DELAY
#include "queue.h" 	//队列
#include "mid_circle_buffer.h"//循环缓冲区


void uart_driver_func(void *argument);

#endif // end __BSP_UART_DRIVER_H__
