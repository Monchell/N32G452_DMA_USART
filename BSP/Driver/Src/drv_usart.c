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
 * 
 * @copyright free
 */

#include "drv_usart.h"
#include "stdio.h"

#if USART1_CONFIG == 1 
void uart1_dma_init(void);
uint8_t usart1_rx_buf[USART1_MAX_UART_RXBUF]={};
uint8_t usart1_tx_buf[USART1_MAX_UART_TXBUF]={};
usart_callback usart1_callback = NULL;
#endif

//printf重定向到了USART1
int fputc(int ch, FILE* f)
{
    USART_SendData(USART_PRINTF, (uint8_t)ch);
    while (USART_GetFlagStatus(USART_PRINTF, USART_FLAG_TXDE) == RESET);
    return (ch);
}


/**
 * @brief DMA串口初始化函数
 * @param USARTx：初始化的串口,比如USART1
 * @param uiBaudRate：波特率
 * @param callback_fun：接收到数据触发钩子函数，用户自行编写函数实现然后把函数指针填进去
 */

void uartx_init(USART_Module* USARTx, uint32_t uiBaudRate, usart_callback callback_fun)
{
	GPIO_InitType GPIO_InitStructure;
	NVIC_InitType NVIC_InitStructure;
	USART_InitType USART_InitStructure;
	
	#if USART1_CONFIG == 1 
	if(USARTx == USART1)
	{
		usart1_callback = callback_fun;
		uart1_dma_init();
		/* 初始化GPIO,USART1默认使用PA9，PA10作为引脚*/
		/* 使能GPIO和复用时钟 */
		RCC_EnableAPB2PeriphClk(USART1_GPIO_CLK|RCC_APB2_PERIPH_AFIO, ENABLE);						
		/* 使能USART时钟 */
		USART1_APBxClkCmd(USART1_CLK, ENABLE);	
		/* 配置相关GPIO */
		GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin            = USART1_TxPin; 
		GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_50MHz;		
		GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_AF_PP;
		GPIO_InitPeripheral(USART1_GPIO, &GPIO_InitStructure);
		
		GPIO_InitStructure.Pin            = USART1_RxPin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitPeripheral(USART1_GPIO, &GPIO_InitStructure);
		
		
		/*配置中断优先级*/
		NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority				 = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
	}
	#endif
	/*配置中断优先级*/
	
	/* 配置USART*/
	USART_DeInit(USARTx);
	USART_InitStructure.BaudRate            = uiBaudRate;
	USART_InitStructure.WordLength          = USART_WL_8B;
	USART_InitStructure.StopBits            = USART_STPB_1;
	USART_InitStructure.Parity              = USART_PE_NO;
	USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
	USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;	
	USART_Init(USARTx, &USART_InitStructure);		
	
	//使能串口空闲中断
	USART_ConfigInt(USARTx, USART_INT_IDLEF, ENABLE);
	
	/* 最后使能串口与DMA串口 */
	USART_EnableDMA(USARTx, USART_DMAREQ_RX | USART_DMAREQ_TX, ENABLE);
	USART_Enable(USARTx, ENABLE);
}



#if USART1_CONFIG == 1 
void uart1_dma_init(void)
{
	
	DMA_InitType DMA_InitStructure;
	NVIC_InitType NVIC_InitStructure;
	/* 使能DMA时钟,这里避免麻烦两个都使能了 */
	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA1, ENABLE);
	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA2, ENABLE);
	/* 配置DMA USART发送通道*/
	DMA_DeInit(USART1_Tx_DMA_Channel);
	DMA_InitStructure.PeriphAddr 			= (uint32_t)&(USART1->DAT);				//外设地址
	DMA_InitStructure.MemAddr    			= (uint32_t)usart1_tx_buf;					//本例程发送和接收共用一个数组，接收到数据之后立即返回
	DMA_InitStructure.Direction  			= DMA_DIR_PERIPH_DST;							//外设作为目标设备
	DMA_InitStructure.BufSize    			= USART1_MAX_UART_TXBUF;							//发送数据大小，每次发送可以重新定义
	DMA_InitStructure.PeriphInc     	= DMA_PERIPH_INC_DISABLE;
	DMA_InitStructure.DMA_MemoryInc 	= DMA_MEM_INC_ENABLE;
	DMA_InitStructure.PeriphDataSize 	= DMA_PERIPH_DATA_SIZE_BYTE;
	DMA_InitStructure.MemDataSize    	= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.CircularMode   	= DMA_MODE_NORMAL;
	DMA_InitStructure.Priority       	= DMA_PRIORITY_VERY_HIGH;
	DMA_InitStructure.Mem2Mem        	= DMA_M2M_DISABLE;
	DMA_Init(USART1_Tx_DMA_Channel, &DMA_InitStructure);
	DMA_RequestRemap(USART1_Tx_DMA_REMAP, DMA1, USART1_Tx_DMA_Channel, ENABLE);
	
	
	/* 使能传输完成中断 */
	DMA_ConfigInt(USART1_Tx_DMA_Channel, DMA_INT_TXC, ENABLE);
	/* 配置DMA USART接收*/
	DMA_InitStructure.PeriphAddr = (uint32_t)&(USART1->DAT);
	DMA_InitStructure.MemAddr    = (uint32_t)usart1_rx_buf;
	DMA_InitStructure.Direction  = DMA_DIR_PERIPH_SRC;
	DMA_InitStructure.BufSize    = USART1_MAX_UART_RXBUF;					//默认接收数据大小
	DMA_Init(USART1_Rx_DMA_Channel, &DMA_InitStructure);
	DMA_RequestRemap(USART1_Rx_DMA_REMAP, DMA1, USART1_Rx_DMA_Channel, ENABLE);
	/* 使能传输完成中断 */
	DMA_ConfigInt(USART1_Rx_DMA_Channel, DMA_INT_TXC, ENABLE);
	/* ！！！初始状态关闭发送通道，需要时打开，否则初始化完成之后会直接发送一包数据 */
	DMA_EnableChannel(USART1_Tx_DMA_Channel, DISABLE);  
	/* 打开DMA接收通道 */
	DMA_EnableChannel(USART1_Rx_DMA_Channel, ENABLE); 
	
	/*最后配置DMA相关的中断，正常如果余量足够接收中断可以不配置 */
	
	/* 配置DMA发送中断*/
	NVIC_InitStructure.NVIC_IRQChannel            = USART1_Tx_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* 配置DMA接收中断*/
	NVIC_InitStructure.NVIC_IRQChannel            = USART1_Rx_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
#endif


/*使用DMA发送数据，数据为usartx_rx_buf内存储的数据
  uint16_t Length 要发送的数据长度
*/

/**
 * @brief DMA串口发送数据
 * @param USARTx：调用的串口,比如USART1
 * @param send_data：待发送数据的数组
 * @param Length：发送长度
 * @note  不可大于最大的数组缓存区长度
 */
void uartx_send_data(USART_Module* USARTx, uint8_t* send_data, uint16_t Length)
{
	#if USART1_CONFIG == 1
	if(USARTx == USART1)
	{
		
		DMA_EnableChannel(USART1_Tx_DMA_Channel, DISABLE);	  //关闭DMA通道	 
		if(Length > USART1_MAX_UART_TXBUF)
			Length = USART1_MAX_UART_TXBUF;
		for(int i = 0; i < Length; i++)
			usart1_tx_buf[i] = send_data[i];		
		DMA_SetCurrDataCounter(USART1_Tx_DMA_Channel,Length); //设置传输长度
		DMA_EnableChannel(USART1_Tx_DMA_Channel, ENABLE);     //使能DMA通道		
	}
	#endif
}





#if USART1_CONFIG == 1 	
void USART1_Tx_DMA_IRQHandler(void)
{
    if(DMA_GetIntStatus(USART1_Tx_DMA_INT, DMA1) != RESET)
    {            
        DMA_EnableChannel(USART1_Tx_DMA_Channel, DISABLE);
        DMA_ClrIntPendingBit(USART1_Tx_DMA_INT, DMA1);         
    }
}
void USART1_Rx_DMA_IRQHandler(void)
{
    if(DMA_GetIntStatus(USART1_Rx_DMA_INT, DMA1) != RESET)
    {            
        DMA_EnableChannel(USART1_Rx_DMA_Channel, DISABLE);
        DMA_ClrIntPendingBit(USART1_Rx_DMA_INT, DMA1);    
        DMA_SetCurrDataCounter(USART1_Rx_DMA_Channel,USART1_MAX_UART_RXBUF);
        DMA_EnableChannel(USART1_Rx_DMA_Channel, ENABLE);
    }     
}


void USART1_IRQHandler(void)
{
	uint16_t rev_len = 0;      //当前数据个数
	if(USART_GetIntStatus(USART1,USART_INT_IDLEF)!=RESET)
	{
		USART_ReceiveData(USART1);				//为了清除中断
		//先关闭DMA接收
		DMA_EnableChannel(USART1_Rx_DMA_Channel, DISABLE);
		//清一下DMA中断标志位，因为没有触发也可以不清
		DMA_ClrIntPendingBit(USART1_Rx_DMA_INT, DMA1);
		//实际接收数据个数
		rev_len = USART1_MAX_UART_RXBUF - DMA_GetCurrDataCounter(USART1_Rx_DMA_Channel);		
		//进入上层实现的回调函数，也是底层提供给上层的hook函数
		if(usart1_callback != NULL)
		{
			usart1_callback(rev_len,usart1_rx_buf);
		}
		//最后设置接收个数，打开接收
		DMA_SetCurrDataCounter(USART1_Rx_DMA_Channel,USART1_MAX_UART_RXBUF); 
		DMA_EnableChannel(USART1_Rx_DMA_Channel, ENABLE);
	}
}
#endif

