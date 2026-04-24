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
#include "uart_task.h"
#include "bsp_uart_driver.h"
#include "mid_circle_buffer.h"


#if 0
#include "usart.h"  //UART_HandleTypeDef huart1;
#include "elog.h"
#include <string.h>  // memset
// #include <stdlib.h>
#include "task.h"   // 任务通知函数  xTaskNotifyFromISR  MAX_DELAY
#include "queue.h" 	//队列
#endif


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

//#define half_size 2
//#define full_size 2*half_size
//#define half_notify (1<<0)
//#define full_notify (1<<1)

//uint8_t *p_g_buf = NULL;
  
  
#if 0
// 环形buffer---简易版
  uint8_t g_buffer[8]={0};
  uint8_t g_input_buffer[8]={1,2,3,4,5,6,7,8};//输入数据
  uint8_t head=0;
  uint8_t tail=0;
#define buffer_size 8

/**环形buffer 重要三个函数
 * put：将数据写入buffer
 * get：从buffer中读取数�?
 * isEmpty：判断buffer是否为空
 * 
 * put：将数据写入buffer--从head�?始写�?
 * get：从buffer中读取数�?--从tail�?始读�?
 */
void buffer_put(uint8_t val){
  // log_a("val %d,head %d",val,head);
    g_buffer[head]=val;//this head g_buffer[head]=val then ++(incremented)
//    if(head==buffer_size){
//        head=0;
//    }
	head=(head+1)%buffer_size;
    // g_buffer[(head++)%buffer_size]=val;

}
uint8_t is_empty(void){
  if(head==tail){ //收尾相等，说明buffer为空
    log_a("buffer full");
    return 1;
  }
  else{return 0;}
}
uint8_t buffer_get(uint8_t *val){
  if(!is_empty()){
    // log_a("buffer_get %d",g_buffer[tail]);
//    *val=g_buffer[tail++];//从tail�?始读取数�?  尾部取出
//    if(tail==buffer_size){
//      tail=0;
//    }
	*val=g_buffer[tail];//从tail�?始读取数�?  尾部取出
    tail=(tail+1)%buffer_size;
	return 1;
  }
  return 0;
}
void my_main(void){
  uint8_t i,val=0;
  for(i=0;i<buffer_size;i++){
    buffer_put(g_input_buffer[i]);
    log_d("i: %d ;head: %d ;val: %d",i,head,g_input_buffer[i]);//this head has been ++(incremented)

    if(i%2==0 && !is_empty()){  //偶数次读取数�?
      buffer_get(&val);
      log_i("tail: %d val: %d",tail,val);
    }
  }
  //打印剩余的
  while(!is_empty()){
    buffer_get(&val);
    log_i("tail: %d val: %d",tail,val);
  }
}
#endif
void  my_main(void){
  uint8_t g_input_buffer[8]={1,2,3,4,5,6,7,8};//输入数据
  circle_buffer_t * buffer = mid_circle_buffer_create();
  uint8_t val=0;
  if(buffer == NULL){
    log_a("mid_circle_buffer_create failed");
    return;
  }
  for(int i=0;i<CIRCLE_BUFFER_SIZE;i++){
    circle_buf_put(buffer,g_input_buffer[i]);//这里直接传入结构体就好-内部自动指向arry
    log_d("i: %d ;head: %d ;val: %d",i,buffer->head,buffer->data[i]);
    if(i%2==0 && !circle_buf_is_empty(buffer)){ //偶数次读取数
      circle_buf_get(buffer,&val);
      log_i("tail: %d val: %d",buffer->tail,val);
    }
  }

}
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
 /**变换buffer+通知另一个线程 */
osThreadId_t rec_A_TaskHandle;
const osThreadAttr_t rec_A_Task_attributes = {
  .name = "rec_A_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
 /**取出buffer+打印 */
osThreadId_t uart_driver_TaskHandle;
const osThreadAttr_t uart_driver_Task_attributes = {
  .name = "uart_driver_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,//靠近内核，优先级高，先执行
};
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  /* USER CODE BEGIN RTOS_THREADS */
  /**应当有三个线程，
   * 
   * */
  ///接收A线程--
  rec_A_TaskHandle = osThreadNew(uart_rec_A_func, NULL, &rec_A_Task_attributes);

 ///bsp_uart_driver---开中断-中断回调--切换ab-buffer
  uart_driver_TaskHandle = osThreadNew(uart_driver_func, NULL, &uart_driver_Task_attributes);
  // ConvertVoltageTaskHandle = osThreadNew(ConvertVoltageTask, NULL, &ConvertVoltageTask_attributes);

  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
//  my_main();
  /* Infinite loop */
	for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
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
#endif

/* USER CODE END Application */

