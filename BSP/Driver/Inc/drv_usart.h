/**
 * @file drv_usart.c
 * @author Monchell
 * @version v1.0.0
 * @brief DMA��������
 * @note 
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
typedef void (* TaskFunc)( void * );	
	
void uartx_init(USART_Module* USARTx, uint32_t uiBaudRate);
	
	
/* ����������ã�������Ҫ�Ĵ����������ã������Ҫ������ھ����ⶨ����������*/
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


/*����ΪUSARTx DMA�������*/
#define USARTx_Tx_DMA_Channel       DMA_CH4
#define USARTx_Tx_DMA_FLAG          DMA_FLAG_TC4
#define USARTx_Tx_DMA_IRQn          DMA_Channel3_4_IRQn
#define USARTx_Tx_DMA_IRQHandler    DMA_Channel3_4_IRQHandler
#define USARTx_Tx_DMA_INT           DMA_INT_TXC4
#define USARTx_Rx_DMA_Channel       DMA_CH5
#define USARTx_Rx_DMA_FLAG          DMA_FLAG_TC5
#define USARTx_Rx_DMA_IRQn          DMA_Channel5_IRQn
#define USARTx_Rx_DMA_IRQHandler    DMA_Channel5_IRQHandler
#define USARTx_Rx_DMA_INT           DMA_INT_TXC5
#define USARTx_Tx_DMA_REMAP         DMA_REMAP_USART1_TX
#define USARTx_Rx_DMA_REMAP         DMA_REMAP_USART1_RX

void uart_dma_init(void);
void uartx_send_data(uint16_t Length);

#ifdef __cplusplus
}
#endif

#endif

