/**
 * @file drv_usart.c
 * @author Monchell
 * @version v1.0.0
 * @brief DMA串口配置
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
 * 函数指针，用于实现回调函数
 */
typedef void (* TaskFunc)( void * );	
	
void uartx_init(USART_Module* USARTx, uint32_t uiBaudRate);
	
	
/* 串口相关配置，根据需要的串口自行设置，如果需要多个串口就另外定义其他内容*/
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


/*以下为USARTx DMA相关配置*/
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

void uart1_dma_init(void);
void uartx_send_data(uint8_t* send_data, uint16_t Length);

#ifdef __cplusplus
}
#endif

#endif

