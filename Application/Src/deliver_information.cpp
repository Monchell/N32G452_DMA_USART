/**
 * @file deliver_information.cpp
 * @author Monchell
 * @version v1.0.0
 * @brief ����ת�����񣬽��ܴ��ڷ��͵���ϢȻ����ת����ȥ
 * @copyright free
 */

#include "deliver_information.h"
#include "drv_usart.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timer.h"


/** ������ */
/**
 * @brief ����,ת��������Ϣ
 * @param pvParameters�������������
 */

int i;
char s[] = "hellhfaiouhfebniaushgidflhalksdhfesdfo\n";
uint8_t test_data[100] = {};
void deliver_information_task(void *pvParameters)
{
	while(1)		
	{	
		//printf("11111\n");
		//NZ_Delay_ms(1000);
		memcpy(test_data,s,strlen(s)+1);
		uartx_send_data(test_data,strlen(s)+1);
		//NZ_Delay_ms(2);
		//DMA_EnableChannel(USART1_Tx_DMA_Channel, DISABLE);
		NZ_Delay_ms(1000);
		//USART_SendData(USART1, 1);		
		//while (USART_GetFlagStatus(USART1, USART_FLAG_TXDE) == RESET)		
		//USART_SendData(USART1, 2);		
		
		i++;
	}
}


