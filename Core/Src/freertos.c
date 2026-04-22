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
#include "elog.h"

#include <string.h>  // memset
// #include <stdlib.h>
#include "task.h"   // 任务通知函数  xTaskNotifyFromISR  MAX_DELAY

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void ChangeBufTask(void *arg);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
#define which_buf1 0
#define which_buf2 1
// #define buffer_size 4

uint8_t g_buf_flag=which_buf1;
  uint32_t *p_g_buf1 = NULL;
  uint32_t *p_g_buf2 = NULL;

  
#if 1
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
    g_buffer[head++]=val;//this head g_buffer[head]=val then ++(incremented)
    if(head==buffer_size){
        head=0;
    }
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
    *val=g_buffer[tail++];//从tail�?始读取数�?  尾部取出
    if(tail==buffer_size){
      tail=0;
    }
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
  while(!is_empty()){
    buffer_get(&val);
    log_i("tail: %d val: %d",tail,val);
  }
}
#endif

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
 /**dma 传输完成回调 */
osThreadId_t ChangeBufTaskHandle;
const osThreadAttr_t ChangeBufTask_attributes = {
  .name = "ChangeBufTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
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
   * �??个默认线程，初始�??
   * �??个切换buffer线程�??
   * �??个处理数�??--convert voltage 线程 
   * */
  ChangeBufTaskHandle = osThreadNew(ChangeBufTask, NULL, &ChangeBufTask_attributes);
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
  log_i("StartDefaultTask runninggggggggggggggg");
  my_main();
  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void ChangeBufTask(void *arg){
  /**切换buffer线程 */
  log_i("ChangeBufTask running---------------");
  uint32_t recvValue = 0;
  while(1){
    osDelay(1);
	  // log_d("---------");
  }
}
/* USER CODE END Application */

