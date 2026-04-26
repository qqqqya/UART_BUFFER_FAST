#ifndef __BSP_UART_DRIVER_H__
#define __BSP_UART_DRIVER_H__

#define queue_size 2
#include "FreeRTOS.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"  //UART_HandleTypeDef huart1;
#include "elog.h"


#include <string.h>  // memset
#include <stdint.h>//uint8_t uint32_t
#include <stdio.h>//null
#include "task.h"   // 任务通知函数  xTaskNotifyFromISR  MAX_DELAY
#include "queue.h" 	//队列
#include "mid_circle_buffer.h"//循环缓冲区

void dma_half_callback      (uint32_t numofbytes);
void dma_complete_callback  (uint32_t numofbytes);
void uart_irq_idle_callback (uint32_t numofbytes);

void uart_driver_func(void *argument);

#endif // end __BSP_UART_DRIVER_H__
