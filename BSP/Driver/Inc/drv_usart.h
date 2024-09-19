/**
 * @file drv_usart.c
 * @author Monchell
 * @version v1.0.0
 * @brief DMA串口配置，DMA数据收发，减少cpu资源占用，会快很多
 * @note 目前只包含串口1，因为实验室的板子只画了串口1（无语），其他串口无效
 * 怎么使用这个东西？
 * 1. 把该文件的源文件和头文件加入到项目工程当中
 * 2. 配置需要用到的串口，将相关宏定义置1
 * 3. 调用uartx_init函数完成串口硬件相关的初始化配置
 * 如何接收数据？
 * 	在初始化中需要填入了用户自行编写的钩子函数，钩子函数是void (uint16_t , uint8_t*)的函数类型，
 *  这个函数会在接收到一次数据之后自动调用（处于中断里面）。
 * 如何发送数据？
 * 	调用void uartx_send_data(USART_Module* USARTx, uint8_t* send_data, uint16_t Length)发送数据
 * 该文件还实现了串口重定向，也即是printf函数，在宏定义里面可以指定重定向的串口，注意需要开启microlib
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
typedef void (* usart_callback)(uint16_t, uint8_t*);	
	

#define USART_PRINTF USART1
	
	
//相关宏定义，使用到的就置1	，实现可裁剪减少资源使用，而且不会占用到其他用途
	
	
//是否使用串口1
#define USART1_CONFIG         					1
#if USART1_CONFIG == 1 	
	
/* USART1相关配置*/
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


/*以下为USART1 DMA相关配置，使用了DMA1CH4和DMA1CH5来做收发的DMA*/
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

