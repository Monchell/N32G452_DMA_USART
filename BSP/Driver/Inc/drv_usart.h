/**
 * @file drv_usart.c
 * @author Monchell
 * @version v1.0.0
 * @brief DMA�������ã�DMA�����շ�������cpu��Դռ�ã����ܶ�
 * @note Ŀǰֻ��������1����Ϊʵ���ҵİ���ֻ���˴���1�����������������Ч
 * ��ôʹ�����������
 * 1. �Ѹ��ļ���Դ�ļ���ͷ�ļ����뵽��Ŀ���̵���
 * 2. ������Ҫ�õ��Ĵ��ڣ�����غ궨����1
 * 3. ����uartx_init������ɴ���Ӳ����صĳ�ʼ������
 * ��ν������ݣ�
 * 	�ڳ�ʼ������Ҫ�������û����б�д�Ĺ��Ӻ��������Ӻ�����void (uint16_t , uint8_t*)�ĺ������ͣ�
 *  ����������ڽ��յ�һ������֮���Զ����ã������ж����棩��
 * ��η������ݣ�
 * 	����void uartx_send_data(USART_Module* USARTx, uint8_t* send_data, uint16_t Length)��������
 * ���ļ���ʵ���˴����ض���Ҳ����printf�������ں궨���������ָ���ض���Ĵ��ڣ�ע����Ҫ����microlib
 * @copyright free
 */

#ifndef __DRV_USART_H__
#define __DRV_USART_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "n32g45x.h"

/**
 * ����ָ�룬����ʵ�ֻص�����
 */
typedef void (* usart_callback)(uint16_t, uint8_t*);	
	

#define USART_PRINTF USART1
	
	
//��غ궨�壬ʹ�õ��ľ���1	��ʵ�ֿɲü�������Դʹ�ã����Ҳ���ռ�õ�������;
	
	
//�Ƿ�ʹ�ô���1
#define USART1_CONFIG         					1
#if USART1_CONFIG == 1 	
	
/* USART1�������*/
#define USART1_CLK        	RCC_APB2_PERIPH_USART1
#define USART1_GPIO       	GPIOA
#define USART1_GPIO_CLK   	RCC_APB2_PERIPH_GPIOA
#define USART1_RxPin      	GPIO_PIN_10
#define USART1_TxPin      	GPIO_PIN_9
#define USART1_APBxClkCmd 	RCC_EnableAPB2PeriphClk
	
#define USART1_IRQn       	USART1_IRQn
#define USART1_IRQHandler 	USART1_IRQHandler
#define USART1_MAX_UART_RXBUF                 256    
#define USART1_MAX_UART_TXBUF                 256    


/*����ΪUSART1 DMA������ã�ʹ����DMA1CH4��DMA1CH5�����շ���DMA*/
#define USART1_Tx_DMA_Channel       DMA1_CH4
#define USART1_Tx_DMA_FLAG          DMA1_FLAG_TC4
#define USART1_Tx_DMA_IRQn          DMA1_Channel4_IRQn														
#define USART1_Tx_DMA_IRQHandler    DMA1_Channel4_IRQHandler
#define USART1_Tx_DMA_INT           DMA1_INT_TXC4
#define USART1_Tx_DMA_REMAP         DMA1_REMAP_USART1_TX


#define USART1_Rx_DMA_Channel       DMA1_CH5
#define USART1_Rx_DMA_FLAG          DMA1_FLAG_TC5
#define USART1_Rx_DMA_IRQn          DMA1_Channel5_IRQn
#define USART1_Rx_DMA_IRQHandler    DMA1_Channel5_IRQHandler
#define USART1_Rx_DMA_INT           DMA1_INT_TXC5
#define USART1_Rx_DMA_REMAP         DMA1_REMAP_USART1_RX
#endif


void uartx_send_data(USART_Module* USARTx, uint8_t* send_data, uint16_t Length);
void uartx_init(USART_Module* USARTx, uint32_t uiBaudRate, usart_callback callback_fun);

#ifdef __cplusplus
}
#endif

#endif

