/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
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

#define half_size 2
#define full_size 2*half_size
#define half_notify (1<<0)
#define full_notify (1<<1)

uint8_t *p_g_buf = NULL;

xQueueHandle xQueue_A=NULL;
uint32_t send_que_recv_A=0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  /**接收完成回调 */
  log_i("HAL_UART_RxCpltCallback");
  xQueueSendFromISR(xQueue_A, &send_que_recv_A, NULL);
  log_d("HAL_UART_Receive_IT send notify: %d",send_que_recv_A);

  //持续接收
  HAL_UART_Receive_IT(&huart1, p_g_buf, 1);

}
void ChangeBufTask(void *arg){
  /**切换buffer线程 */
  log_i("ChangeBufTask running---------------");
  uint8_t buff[]="hello world\r\n";
  uint32_t recv_notify=0;
 HAL_UART_Transmit(&huart1,buff,14,100);//HAL_UART_Transmit_DMA_IT  

  xQueue_A=xQueueCreate(half_size, sizeof(uint8_t));
  log_i("queue_irq_rec_A Init Success");
  log_i("queue address: %p",xQueue_A);
  /**创建双ABbuffer */
  p_g_buf = (uint8_t *)malloc(sizeof(uint8_t)*full_size);//创建双ABbuffer-返回首地址 指针
	memset(p_g_buf,0,full_size);

/** HAL_UART_Receive_DMA_IT(&huart1, p_g_buf, full_size);//启动dma 接收
    重新启动DMA接收  --要在if里面不然任何一个uart触发中断之后都要重新启动 */
    // HAL_UARTEx_ReceiveToIdle_DMA(&huart1, p_g_buf, full_size);


	  if(HAL_OK==HAL_UART_Receive_IT(&huart1, p_g_buf, 1)){  /////uart 只支持8bit  full_size
//    HAL_UART_Transmit_IT(&huart1, p_g_buf, 1);
		  memset(p_g_buf,0,full_size);
		}
  while(1){
    osDelay(1);

    if(xQueueReceive(xQueue_A, &recv_notify, portMAX_DELAY) == pdTRUE){
      // printf("%c",recv_notify);
      log_d("i will notify OutputTask& changebuffer");
      log_w("recv_notify: %d",recv_notify);
    }
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

