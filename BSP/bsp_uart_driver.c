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
void uart_driver_func(void){
//  HAL_UART_Receive_IT(&huart1, p_g_buf, 1);
}

#if 0
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  /**发送完成回调 */
  log_i("HAL_UART_TxCpltCallback");
}
// void HAL_UART_halfCpltCallback(UART_HandleTypeDef *huart){
//   /**接收完成回调 */
//   // log_i("HAL_UART_RxCpltCallback");
// }

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size)
{//这里面只是中断写队列
//    struct uart_data *Priv_data;
//    Priv_data=g_cur_uart_dev->priv_data;
// int len = Size;
//    if(huart->Instance == USART1){
//     HAL_UART_Receive_DMA(hhuart, p_g_buf, full_size);//启动dma 接收
//    }
}
#endif

//xQueueHandle xQueue_A=NULL;
//uint32_t send_que_recv_A=0;
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//  /**接收完成回调 */
////  log_i("HAL_UART_RxCpltCallback");
////  xQueueSendFromISR(xQueue_A, &send_que_recv_A, NULL);
////  log_d("HAL_UART_Receive_IT send notify: %d",send_que_recv_A);

////  //持续接收
//////  HAL_UART_Receive_IT(&huart1, p_g_buf, 1);

//}
