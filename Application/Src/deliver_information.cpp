/**
 * @file deliver_information.cpp
 * @author Monchell
 * @version v1.0.0
 * @brief 串口转发任务，接受串口发送的消息然后再转发回去
 * @copyright free
 */

#include "deliver_information.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timer.h"


/** 任务函数 */
/**
 * @brief 任务,转发串口信息
 * @param pvParameters：任务参数，无
 */

int i;
void deliver_information_task(void *pvParameters)
{
	while(1)		
	{	
		printf("11111\n");
		NZ_Delay_ms(1000);
		//USART_SendData(USART1, 1);		
		//while (USART_GetFlagStatus(USART1, USART_FLAG_TXDE) == RESET)		
		//USART_SendData(USART1, 2);		
		
		i++;
	}
}


