/*
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file uart_task.c
 * 
 * @par dependencies 
 * - stdio.h
 * - stdint.h
 * - "usart.h"
 * - elog.h
 * 
 * 
#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "queue.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
 * @author yan | R&D Dept. | EternalChip ?????
 *
 * @brief Provides HAL APIs for LED control and operations.
 * 
 * Usage:
 * Call functions directly.
 * 
 * @version V1.0 2026年4月24日
 *
 * @note 1 tab == 4 spaces
 * 
 *****************************************************************************/
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"  //UART_HandleTypeDef huart1;
#include "elog.h"

#include <string.h>  // memset
// #include <stdlib.h>
#include "task.h"   // 任务通知函数  xTaskNotifyFromISR  MAX_DELAY
#include "queue.h" 	//队列
#include "mid_circle_buffer.h"//循环缓冲区
// #define half_size 2
// #define full_size 2*half_size
// #define half_notify (1<<0)
// #define full_notify (1<<1)
#define all_size 10
#define half_notify 0
#define full_notify 1


xQueueHandle xQueue_A=NULL;
uint32_t send_que_recv_A=0;

extern uint8_t g_buf1[1];
extern uint8_t g_buf2[1];
extern circle_buffer_t * g_cirle_buffer;

void uart_rec_A_func(void *arg){
  /**接收A线程 */
  log_i("uart_rec_A_task running---------------");

  uint32_t recv_notify=0;
  uint8_t cirle_data=0;
 uint8_t buff[]="hello world\r\n"; 
	HAL_UART_Transmit(&huart1,buff,14,100);//HAL_UART_Transmit_DMA_IT  
      
  //创建消息队列
  xQueue_A=xQueueCreate(all_size, sizeof(uint8_t));
  log_i("queue_irq_rec_A Init Success");
  log_i("queue address: %p",xQueue_A);
// HAL_UART_Transmit_IT(&huart1,"abc",3);
/** HAL_UART_Receive_DMA_IT(&huart1, p_g_buf, full_size);//启动dma 接收,100
    重新启动DMA接收  --要在if里面不然任何一个uart触发中断之后都要重新启动 */
    // HAL_UARTEx_ReceiveToIdle_DMA(&huart1, p_g_buf, full_size);

  while(1){

    
  if(circle_buf_get(g_cirle_buffer,&cirle_data)){
    HAL_UART_Transmit_IT(&huart1, &cirle_data, 1);
    log_w("circle_running_g_cirle_buffer = [%c]",cirle_data);
  }
    if(xQueueReceive(xQueue_A, &recv_notify, portMAX_DELAY) == pdTRUE){
      // printf("%c",recv_notify);
      // while(circle_buf_get(g_cirle_buffer, &cirle_data)){
        
      //   log_w("circle_running_g_cirle_buffer = [%c]", cirle_data);}
      HAL_UART_Transmit_IT(&huart1, g_buf1, 1);
      log_d("i will notify OutputTask& changebuffer");
      log_w("recv_notify: %d",recv_notify);
    }
      osDelay(1);

      
}
    /***uint8_t i=sizeof(p_g_buf);HAL_UART_Transmit_IT(&huart1, &i, 1);//这个地方只会是0x04---发的是ascii码
	  printf("heeeeeello\r\n");
//	HAL_UART_Transmit(&huart1, &i, 1, 100); // 先用阻塞发送测试

// HAL_UART_Transmit_IT(&huart1, p_g_buf, sizeof(p_g_buf));
      //sizeof(p_g_buf)，这只会返回指针的大小（4字节） */
  /* USER CODE END StartDefaultTask */
}
void OutputTask(void *arg){
  /**取出buffer+打印 */
  log_i("OutputTask running---------------");
  while(1){
    osDelay(1);
	  // log_d("---------");
  }
}
/* USER CODE END Application */

