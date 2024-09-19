/**
 * @file main.cpp
 * @author Monchell
 * @version v1.0.0
 * @brief ����DMA����ʵ�飬����һ������DMA���գ�ͬʱDMA�жϰѽ��յ�������ת����ָ������Ȼ���ڷ��ظ����ڣ�Ҳ��������ת��
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



/** ��ʼ�������ȼ� */
#define START_TASK_PRIO					1

/** ��ʼ�����ջ��С */
#define START_TASK_SIZE 				64  

/** ��ʼ������ */
TaskHandle_t Start_Task_Handler;
TaskHandle_t Deliver_information_Task_Handler;


/** ��ʼ������ */
void start_task(void *pvParameters);


RCC_ClocksType rcc_clock;//��ȡʱ�ӷ�Ƶ���


void usart1_callback(uint16_t len, uint8_t* rev_num);
/**
 * @brief  Main program.
 */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//�жϷ�������
	RCC_GetClocksFreqValue(&rcc_clock);//��ȡʱ������ʱ��Ƶ�ʣ�����������
	uartx_init(USART1,230400, usart1_callback);	
	//printf("11111\n");	
	NZ_Delay_init();//ʱ�ӳ�ʼ��
	//while (USART_GetFlagStatus(USART1, USART_FLAG_TXDE) == RESET)		
	//printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
	xTaskCreate((TaskFunction_t )start_task,            //������
							(const char*    )"start_task",          //�������ƣ���ʱ����Ҫ��ӡ�����ʱ�����֪�����ĸ��������
              (uint16_t       )START_TASK_SIZE,       //�����ջ��С
              (void*          )NULL,                  //���ݸ��������Ĳ���
              (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
              (TaskHandle_t*  )&Start_Task_Handler);  //������
	vTaskStartScheduler();          //�����������	
}


//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	
		//�����Լ�������
    xTaskCreate((TaskFunction_t )deliver_information_task,     	
                (const char*    )"deliver_information_task",   	
                (uint16_t       )DELIVER_INFORMATION_TASK_SIZE,
                (void*          )NULL,				
                (UBaseType_t    )DELIVER_INFORMATION_TASK_PRIO,	
                (TaskHandle_t*  )&Deliver_information_Task_Handler);   
								
								
    vTaskDelete(Start_Task_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


void usart1_callback(uint16_t len, uint8_t* rev_num)
{
	uartx_send_data(USART1,rev_num,len);
}

/**
 * @}
 */
