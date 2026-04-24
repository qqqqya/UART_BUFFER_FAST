#ifndef __BSP_UART_TASK_H__
#define __BSP_UART_TASK_H__

//帧头 帧尾
#define FRAME_HEAD 0xFE
#define FRAME_TAIL 0xFF

//帧状态
#define FRAME_HEAD_STATUSE 0x01
#define FRAME_TAIL_STATUSE 0x02
#define FRAME_NOT_DETECTED 0x03

void uart_rec_A_func(void *arg);

#endif // end __BSP_UART_TASK_H__
