/**
 * @file deliver_information.cpp
 * @author Monchell
 * @version v1.0.0
 * @brief ����ת�����񣬽��ܴ��ڷ��͵���ϢȻ����ת����ȥ
 * @copyright free
 */

#include "deliver_information.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timer.h"


/** ������ */
/**
 * @brief ����,ת��������Ϣ
 * @param pvParameters�������������
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


