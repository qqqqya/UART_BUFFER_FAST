/*
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file mid_circle_buffer.c
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
#include "mid_circle_buffer.h"

circle_buffer_t* mid_circle_buffer_create(void){
  circle_buffer_t * tempbuffer=NULL;
  // 1. alloct the memory space
  tempbuffer = (circle_buffer_t *)malloc(sizeof(circle_buffer_t));
  if(tempbuffer == NULL){         //创建一个单位的结构体空间--包含环形buffer数组
    return NULL;
  }
  // tempbuffer->head = 0;
  // tempbuffer->tail = 0;
  // 2. memory init
    memset ( tempbuffer, 0, sizeof(circle_buffer_t));
  return tempbuffer;
}
uint8_t circle_buf_is_empty(circle_buffer_t* buffer){
  if(buffer == NULL){
    return 0;
  }
  if(buffer->head == buffer->tail){//只有头指针和尾指针相等时，说明buffer为空，没有数据
    return 1;
  }
  return 0;
}
uint8_t circle_buf_is_full(circle_buffer_t* buffer){
  if(buffer == NULL){
    return 0;
  }
  if((buffer->head + 1) % CIRCLE_BUFFER_SIZE == buffer->tail){
    return 1;//头指针指向下一个位置，与尾指针相同，说明满员
  }///判满函数+1取余才行 --需要留取一个大小---与判空区分开来
  return 0;
} 

uint8_t circle_buf_put(circle_buffer_t* buffer, uint8_t data){
  if(circle_buf_is_full(buffer)){
    return 0;
  }
  // buffer->data[buffer->head++] = data;  但是后面还加了这里去掉
                        //这里head++是先赋值，后自增
  buffer->data[buffer->head] = data;
  buffer->head = (buffer->head + 1) % CIRCLE_BUFFER_SIZE;
  //头指针指向下一个位置，循环利用数组
  return 1;
}

uint8_t circle_buf_get(circle_buffer_t* buffer, uint8_t* data){

  if(circle_buf_is_empty(buffer)){
    return 0;
  }
  // *data =  buffer->data[buffer->tail++];///同样后面还加了这里去掉
  *data = buffer->data[buffer->tail];
  buffer->tail = (buffer->tail + 1) % CIRCLE_BUFFER_SIZE;
  return 1;
}
