/*
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_uart_driver.c
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
#define which_buf1 0
#define which_buf2 1

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
extern xQueueHandle xQueue_A;
static uint32_t send_que_recv_A=0x12;  
static uint8_t g_recv_data=0;  
uint8_t index=0;

circle_buffer_t * g_cirle_buffer = NULL;

/**创建双ABbuffer */
uint8_t g_buf1[1]={0};
uint8_t g_buf2[1]={0};
uint8_t bufferflag=which_buf2;

void uart_driver_func(void){

  g_cirle_buffer = mid_circle_buffer_create();
  if(g_cirle_buffer == NULL){
    log_a("mid_circle_buffer_create failed");
    return;
  }
 HAL_UART_Receive_IT(&huart1, &g_recv_data, 1);

 while(1){
  vTaskDelay(1);
 }
}


/** @brief 接收完成回调---发送消息队列并切换buffer--ABbuffer
 * @param huart 
 * @return 
 * */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  /**接收完成回调 */
  log_i("HAL_UART_RxCpltCallback");
  xQueueSendFromISR(xQueue_A, &send_que_recv_A, NULL);
  log_d("HAL_UART_Receive_IT send notify: %d",send_que_recv_A);
  
circle_buf_put(g_cirle_buffer,g_recv_data);//将接收到的数据写入循环缓冲区
HAL_UART_Receive_IT(&huart1, &g_recv_data, 1);

  // //持续接收(which_buf2==bufferflag)
  // if(which_buf1==bufferflag) {
  //   log_d("g_data_buffer_1 = [%c]",g_buf1[0]);
  //   HAL_UART_Receive_IT(&huart1, g_buf2, 1);}///切换到buffer2  交替buf接收
  // else {
  //   log_d("g_data_buffer_2 = [%c]",g_buf2[0]);
  //   HAL_UART_Receive_IT(&huart1, g_buf1, 1);}
               
}

#if 0
void uart_driver_func(void){
  // bufferflag=which_buf1;
 HAL_UART_Receive_IT(&huart1, g_buf1, 1);
 while(1){
  vTaskDelay(1);
 }
}
st

/** @brief 接收完成回调---发送消息队列并切换buffer--ABbuffer
 * @param huart 
 * @return 
 * */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  /**接收完成回调 */
  log_i("HAL_UART_RxCpltCallback");
  xQueueSendFromISR(xQueue_A, &send_que_recv_A, NULL);
  log_d("HAL_UART_Receive_IT send notify: %d",send_que_recv_A);
  bufferflag=(bufferflag==which_buf1)?which_buf2:which_buf1;
  // bufferflag==which_buf1?which_buf2:which_buf1;-------判断完要进行赋值
  
  //持续接收(which_buf2==bufferflag)
  if(which_buf1==bufferflag) {
    log_d("g_data_buffer_1 = [%c]",g_buf1[0]);
    HAL_UART_Receive_IT(&huart1, g_buf2, 1);}///切换到buffer2  交替buf接收
  else {
    log_d("g_data_buffer_2 = [%c]",g_buf2[0]);
    HAL_UART_Receive_IT(&huart1, g_buf1, 1);}
               
}
#endif

