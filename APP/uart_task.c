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

#include "uart_task.h"

// #define half_size 2
// #define full_size 2*half_size
// #define half_notify (1<<0)
// #define full_notify (1<<1)

#define half_notify 0
#define full_notify 1
extern uint8_t g_buf1[1];
extern uint8_t g_buf2[1];

xQueueHandle xQueue_A=NULL;
uint32_t send_que_recv_A=0;
extern circle_buffer_t * g_cirle_buffer;//他那里用的是函数返回circlebuffer

void uart_rec_A_func(void *arg){
  /**接收A线程 */
  log_i("uart_rec_A_task running---------------");

  uint32_t recv_notify=0;
  uint8_t cirle_data=0;
  uint8_t buff[]="hello world\r\n"; 
	HAL_UART_Transmit(&huart1,buff,14,100);//HAL_UART_Transmit_DMA_IT  

  //创建消息队列
  xQueue_A=xQueueCreate(queue_size, sizeof(uint32_t));
  log_i("queue_app_rec_A Init Success");
  log_i("queue_app address: %p",xQueue_A);

/** HAL_UART_Receive_DMA_IT(&huart1, p_g_buf, full_size);//启动dma 接收,100
    重新启动DMA接收  --要在if里面不然任何一个uart触发中断之后都要重新启动 */
    // HAL_UARTEx_ReceiveToIdle_DMA(&huart1, p_g_buf, full_size);
   
  while(1){
    /*1、接收到前端发来的信号*/
      xQueueReceive(xQueue_A, &recv_notify, portMAX_DELAY);
      log_w("recv_notify: %x",recv_notify);//打印的 发送来的信号    
        /**
         * app后端接收到前端发来的信号后，
         * 0、将环形buffer中的数据读取出来，等待解析
         * 1、寻找帧头-同时打印get的数据---持续输出
         * 2、寻找帧尾-计算校验和---
         * 3、判断校验和是否正确--若正确打印后  输出一个换行
         */
      
        /**对数据进行输出 
         * while1 buffer 读空 输出数据*/
        if ( NULL == g_cirle_buffer )
        {
            log_e("cirle_buffer error pointer");
            return;
        }
        uint16_t frame_state=FRAME_NOT_DETECTED;
        uint8_t data_temp[CIRCLE_BUFFER_SIZE]={0x00};//缓存循环buffer数据的临时数组
        uint8_t i=0;
        uint32_t jinghe_count=0;   
              while(!circle_buf_is_empty(g_cirle_buffer)){
          circle_buf_get(g_cirle_buffer, &cirle_data);
          //一次性读空循环缓冲区中的数据
          log_i("circle_buffer_get success");
          osDelay(2);

        // log_w("circle_buffer_get = [%x]", cirle_data);
        // log_w("head= [%d],tail= [%d]",g_cirle_buffer->head,g_cirle_buffer->tail);      
        /**解包 是一个个数据解析circle_buf_get每次get到一个数据，
         * 下面的根据这一个数据进行动作
         * ----检测到帧尾之前将数据存起来，同时计算校验和
         * ----检测到帧尾后，判断校验和（计算的是否等于发送的ccr）
         * 若正确，打印数据
         * 若错误，打印错误信息
         */
            switch(frame_state){
            case FRAME_NOT_DETECTED:
              if(cirle_data==FRAME_HEAD){
                frame_state=FRAME_HEAD_STATUS;
                log_i("FRAME_HEAD_STATUS");
                log_i("cirle_data: %d",cirle_data);//打印的 帧头
              }//找到帧头  切换到帧头状态
              break;
            case FRAME_HEAD_STATUS:

                /**输出净荷数据 */
                
                // /**检测到帧尾-计算校验和 */
                // if(FRAME_TAIL==cirle_data){
                //   for(i=0;i<jinghe_count-1;i++){
                //     crc_sum+=cirle_data;
                //   }
                // }
                if(FRAME_TAIL==cirle_data){//检测到帧尾 开始计算校验和
                  /**计算校验和 --这个时候count已经定了 转存数据也成功了*/
                  uint32_t crc_sum=0;
                  uint32_t crc_temp=data_temp[jinghe_count-1];//-2 是crc
                  for(i=0;i<jinghe_count-1;i++){
                    crc_sum+=data_temp[i];
                  }
                  log_i("calculated data_sum = [%d]",crc_sum);

                  //打印净荷数据
                  if(crc_sum==crc_temp){
                    log_i("CRC ok!!jinghe_circle_buffer logging");
                    for(i=0;i<jinghe_count-1;i++){
                      log_w("jinghe_circle_buffer = [%x]", data_temp[i]);
                    }
                  //清空缓存数组
                    
                  }///就算校验和错误  也需要清空缓存数组
                  memset(data_temp,0x00,jinghe_count);
                    jinghe_count=0;
                }  
                else{//不是帧尾  则将数据存起来
                  data_temp[jinghe_count]=cirle_data;
                  jinghe_count++;
                }           
              

              break;
            default:
              break;      
            
            }
      }
}

    osDelay(1);  
      // HAL_UART_Transmit_IT(&huart1, g_buf1, 1);
      // log_d("i will notify OutputTask& changebuffer");
}
/*对数据进行输出 test*/      
#if 0
    /*3、对数据进行输出 */
        /**3.1 while1 buffer 读空 输出数据 */
    while(!circle_buf_is_empty(g_cirle_buffer)){
      circle_buf_get(g_cirle_buffer, &cirle_data);
      //一次性读空循环缓冲区中的数据
      log_w("circle_running_g_cirle_buffer = [%c]", cirle_data);
      log_w("head= [%d],tail= [%d]",g_cirle_buffer->head,g_cirle_buffer->tail);      
      // HAL_UART_Transmit_IT(&huart1, &cirle_data, 1);
    }  
      /**3.2 if 读取 队列进一次读出一个数据 */
    // if(circle_buf_get(g_cirle_buffer,&cirle_data)){
    //   HAL_UART_Transmit_IT(&huart1, &cirle_data, 1);
    //   log_w("circle_running_g_cirle_buffer = [%c]",cirle_data);      
    //   log_w("head= [%d],tail= [%d]",g_cirle_buffer->head,g_cirle_buffer->tail);   
    // }
#endif

/*circle buffer get测试*/
#if 0
  if(circle_buf_get(g_cirle_buffer,&cirle_data)){
    HAL_UART_Transmit_IT(&huart1, &cirle_data, 1);
    log_w("circle_running_g_cirle_buffer = [%c]",cirle_data);
  }
    if(xQueueReceive(xQueue_A, &recv_notify, portMAX_DELAY) == pdTRUE){
      // printf("%c",recv_notify);
      HAL_UART_Transmit_IT(&huart1, g_buf1, 1);
      log_d("i will notify OutputTask& changebuffer");
      log_w("recv_notify: %d",recv_notify);
    }
#endif

/*阻塞发送测试*/
#if 0 
	// if(HAL_OK==HAL_UART_Receive_IT(&huart1, g_buf1, 1)){  /////uart 只支持8bit  full_size
	// 	HAL_UART_Transmit_IT(&huart1, g_buf1, 1);
	// }
    /***uint8_t i=sizeof(p_g_buf);HAL_UART_Transmit_IT(&huart1, &i, 1);//这个地方只会是0x04---发的是ascii码
	  printf("heeeeeello\r\n");
//	HAL_UART_Transmit(&huart1, &i, 1, 100); // 先用阻塞发送测试

// HAL_UART_Transmit_IT(&huart1, p_g_buf, sizeof(p_g_buf));
      //sizeof(p_g_buf)，这只会返回指针的大小（4字节） */
#endif




/* USER CODE END Application */

