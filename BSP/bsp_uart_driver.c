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

#include "bsp_uart_driver.h"
/**发送通知-随便给数据就行 */
#define IRQ_2_FRONT 0xA1A2A3A4
#define FRONT_2_END  0xB1B2B3B4

static uint8_t g_recv_data=0;         //接收串口数据val

extern xQueueHandle xQueue_A;
xQueueHandle xQueue_Front;
circle_buffer_t * g_cirle_buffer = NULL;
#if 0
#define which_buf1 0
#define which_buf2 1/**创建双ABbuffer */
uint8_t g_buf1[1]={0};
uint8_t g_buf2[1]={0};
uint8_t bufferflag=which_buf2;
#endif


void uart_driver_func(void *arg){

  g_cirle_buffer = mid_circle_buffer_create();
  if(g_cirle_buffer == NULL){
    log_a("mid_circle_buffer_create failed");
    return;
  }
  //刚开始没创建这个前端的队列，卡在调度器了
  xQueue_Front = xQueueCreate(queue_size, sizeof(uint32_t));//(2,4)就是两个四字节32bit
  log_i("queue_irq_Front Init Success");
  /*1、启动接收中断 */
  /*1.1 接收一个字节数据*/
//  HAL_UART_Receive_IT(&huart1, &g_recv_data, 1);
 /*1.2 dma多个字节数据*///直接将数据写入循环缓冲区 循环接收
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, g_cirle_buffer->data, CIRCLE_BUFFER_SIZE);//启动dma 接收

 uint32_t recv_notify = 0;
 while(1){

      /*2.中断通知 有数据可读  判断循环缓冲区是否满*/
      if(pdTRUE==xQueueReceive(xQueue_Front, &recv_notify, portMAX_DELAY)){
        log_d("xQueueReceive: %x",recv_notify);
      }
      if(recv_notify == IRQ_2_FRONT){//中断通知 有数据可读
   
        /*3.通知接收完成事件到xQueue_A end端进行数据解析*/
        uint32_t send_notify = FRONT_2_END;
        xQueueSend(xQueue_A, &send_notify, portMAX_DELAY);
    //  if(circle_buf_is_full(g_cirle_buffer)){//判断循环缓冲区是否满
    //       continue;//循环缓冲区已满，等待下一次接收
    //     }
      }

     
      vTaskDelay(1);
 }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

  log_i("HAL_UART_RxCpltCallback");

  // /*1、将接收到的数据写入循环缓冲区*/
  circle_buf_put(g_cirle_buffer,g_recv_data);

  /*2、通知front 数据备好 有数据可读*/
  uint32_t send_notify = IRQ_2_FRONT;
  xQueueSendFromISR(xQueue_Front, &send_notify, NULL);

  // /*3、开启下一次接收中断*/
  HAL_UART_Receive_IT(&huart1, &g_recv_data, 1);
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size)
{
  log_i("event HAL_UARTEx_RxEventCallback");
}
/** @brief 单次字节接收完成回调---将数据写入循环缓冲区
 * @param huart 
 * @return 
 * */
#if 0
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

  log_i("HAL_UART_RxCpltCallback");

  /*1、将接收到的数据写入循环缓冲区*/
  circle_buf_put(g_cirle_buffer,g_recv_data);

  /*2、通知front 数据备好 有数据可读*/
  uint32_t send_notify = IRQ_2_FRONT;
  xQueueSendFromISR(xQueue_Front, &send_notify, NULL);

  // /*3、开启下一次接收中断*/
  // HAL_UART_Receive_IT(&huart1, &g_recv_data, 1);
}
#endif
/**各种回调函数 */
#if 0
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  /**发送完成回调 */
  log_i("HAL_UART_TxCpltCallback");
}
// void HAL_UART_halfCpltCallback(UART_HandleTypeDef *huart){
//   /**接收完成回调 */
//   // log_i("HAL_UART_RxCpltCallback");
// }


#endif


/** @brief 接收完成回调---发送消息队列并切换buffer--ABbuffer
 * circle buffer test
 * @param huart 
 * @return 
 * */
#if 0
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  /**接收完成回调 */
  log_i("HAL_UART_RxCpltCallback");
  xQueueSendFromISR(xQueue_A, &send_que_recv_A, NULL);
  log_d("HAL_UART_Receive_IT send notify: %d",send_que_recv_A);
  
// circle_buf_put(g_cirle_buffer,g_recv_data);//将接收到的数据写入循环缓冲区
// HAL_UART_Receive_IT(&huart1, &g_recv_data, 1);

  //持续接收(which_buf2==bufferflag)
  if(which_buf1==bufferflag) {
    log_d("g_data_buffer_1 = [%c]",g_buf1[0]);
    HAL_UART_Receive_IT(&huart1, g_buf2, 1);}///切换到buffer2  交替buf接收
  else {
    log_d("g_data_buffer_2 = [%c]",g_buf2[0]);
    HAL_UART_Receive_IT(&huart1, g_buf1, 1);}
               
}
#endif

/**ab buffer test */
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

