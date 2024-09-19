/**
 * @file main.cpp
 * @author Monchell
 * @version v1.0.0
 * @brief 串口DMA接受实验，包含一个串口DMA接收，同时DMA中断把接收到的数据转发给指定任务然后在发回给串口，也就是数据转发
 * @copyright free
 */
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "drv_usart.h"
#include "deliver_information.h"
/**
 * @brief Assert failed function by user.
 * @param file The name of the call that failed.
 * @param line The source line number of the call that failed.
 */
#ifdef USE_FULL_ASSERT
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}
#endif // USE_FULL_ASSERT



/** 开始任务优先级 */
#define START_TASK_PRIO					1

/** 开始任务堆栈大小 */
#define START_TASK_SIZE 				64  

/** 初始任务句柄 */
TaskHandle_t Start_Task_Handler;
TaskHandle_t Deliver_information_Task_Handler;


/** 初始任务函数 */
void start_task(void *pvParameters);


RCC_ClocksType rcc_clock;//获取时钟分频情况


void usart1_callback(uint16_t len, uint8_t* rev_num);
/**
 * @brief  Main program.
 */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//中断分组设置
	RCC_GetClocksFreqValue(&rcc_clock);//获取时钟树的时钟频率（拿来看看）
	uartx_init(USART1,230400, usart1_callback);	
	//printf("11111\n");	
	NZ_Delay_init();//时钟初始化
	//while (USART_GetFlagStatus(USART1, USART_FLAG_TXDE) == RESET)		
	//printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
	xTaskCreate((TaskFunction_t )start_task,            //任务函数
							(const char*    )"start_task",          //任务名称，有时候需要打印错误的时候可以知道是哪个任务出错
              (uint16_t       )START_TASK_SIZE,       //任务堆栈大小
              (void*          )NULL,                  //传递给任务函数的参数
              (UBaseType_t    )START_TASK_PRIO,       //任务优先级
              (TaskHandle_t*  )&Start_Task_Handler);  //任务句柄
	vTaskStartScheduler();          //开启任务调度	
}


//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	
		//创建自己的任务
    xTaskCreate((TaskFunction_t )deliver_information_task,     	
                (const char*    )"deliver_information_task",   	
                (uint16_t       )DELIVER_INFORMATION_TASK_SIZE,
                (void*          )NULL,				
                (UBaseType_t    )DELIVER_INFORMATION_TASK_PRIO,	
                (TaskHandle_t*  )&Deliver_information_Task_Handler);   
								
								
    vTaskDelete(Start_Task_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}


void usart1_callback(uint16_t len, uint8_t* rev_num)
{
	uartx_send_data(USART1,rev_num,len);
}

/**
 * @}
 */
