#ifndef __MID_CIRCLE_BUFFER_H__
#define __MID_CIRCLE_BUFFER_H__

#include <stdint.h>//uint8_t uint32_t
#include <stdio.h>//uint8_t uint32_t

#define CIRCLE_BUFFER_SIZE 100

typedef struct{
  uint8_t data[CIRCLE_BUFFER_SIZE];
  uint32_t head;
  uint32_t tail;
}circle_buffer_t;//结构体内直接包含循环数组
/**独立内存，大小 = 成员总和 + 对齐填充，所有成员共存；
 * --总大小：1+3+4+2 = 10 字节，不是最大成员 4 的整数倍，再填充 2 字节；
联合体：共享内存，大小 =最大成员大小，同一时间只用一个；
 */

 /***#### 相关数据类型--结构体{头  尾  大小}

#### 这几个函数-创建空buffer--判断empyt-full---put get data。 */
circle_buffer_t* mid_circle_buffer_create(void);//创建空buffer--返回指针

uint8_t circle_buf_is_empty(circle_buffer_t* buffer);//判断是否为空
uint8_t circle_buf_is_full(circle_buffer_t* buffer);//判断是否为满

uint8_t circle_buf_put(circle_buffer_t* buffer, uint8_t data);//入队
uint8_t circle_buf_get(circle_buffer_t* buffer, uint8_t* data);//出队

#endif // end __MID_CIRCLE_BUFFER_H__
