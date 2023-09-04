/**
 * @file drv_usart.c
 * @author Monchell
 * @version v1.0.0
 * @brief DMA串口配置
 * @note 
 * @copyright free
 */
#include "drv_usart.h"
#include "stdio.h"


uint8_t usart1_rx_buf[USART1_MAX_UART_RXBUF]={1,2,3,4};
uint8_t usart1_tx_buf[USART1_MAX_UART_TXBUF]={1,2,3,4};

int fputc(int ch, FILE* f)
{
    USART_SendData(USART1, (uint8_t)ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXDE) == RESET)
        ;

    return (ch);
}


void uartx_init(USART_Module* USARTx, uint32_t uiBaudRate)
{
	GPIO_InitType GPIO_InitStructure;
	NVIC_InitType NVIC_InitStructure;
	USART_InitType USART_InitStructure;
	
	if(USARTx == USART1)
	{
		/* 初始化GPIO,USART默认使用PA9，PA10作为引脚*/
		/* 使能GPIO和复用时钟时钟 */
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

		/* 配置USART*/
		USART_DeInit(USARTx);
		USART_InitStructure.BaudRate            = uiBaudRate;
		USART_InitStructure.WordLength          = USART_WL_8B;
		USART_InitStructure.StopBits            = USART_STPB_1;
		USART_InitStructure.Parity              = USART_PE_NO;
		USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
		USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;	
		
		USART_Init(USARTx, &USART_InitStructure);		
		
		//	/* 使能USART与DMA */
		USART_EnableDMA(USARTx, USART_DMAREQ_RX | USART_DMAREQ_TX, ENABLE);
		//	/* 使能USART空闲中断*/
//	USART_ConfigInt(USARTx, USART_INT_IDLEF, ENABLE);
		USART_Enable(USARTx, ENABLE);
//	/*配置中断*/
//	NVIC_InitStructure.NVIC_IRQChannel                   = USARTx_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPriority           = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
////	NVIC_Init(&NVIC_InitStructure);
	}
}




void uart1_dma_init(void)
{
	DMA_InitType DMA_InitStructure;
	NVIC_InitType NVIC_InitStructure;
	/* 使能DMA时钟 */
	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA1, ENABLE);
	/* 配置DMA USART发送通道*/
	DMA_DeInit(USART1_Tx_DMA_Channel);
	DMA_InitStructure.PeriphAddr = (uint32_t)&(USART1->DAT);				//外设地址
	DMA_InitStructure.MemAddr    = (uint32_t)usart1_tx_buf;					//本例程发送和接收共用一个数组，接收到数据之后立即返回
	DMA_InitStructure.Direction  = DMA_DIR_PERIPH_DST;							//外设作为目标设备
	DMA_InitStructure.BufSize    = USART1_MAX_UART_TXBUF;							//发送数据大小，每次发送可以重新定义
	DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
	DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
	DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
	DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.CircularMode   = DMA_MODE_NORMAL;
	DMA_InitStructure.Priority       = DMA_PRIORITY_VERY_HIGH;
	DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* 配置DMA接收中断*/
	NVIC_InitStructure.NVIC_IRQChannel            = USART1_Rx_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/*使用DMA发送数据，数据为usartx_rx_buf内存储的数据
  uint16_t Length 要发送的数据长度
*/
void uartx_send_data(uint8_t* send_data, uint16_t Length)
{
	
	DMA_EnableChannel(USART1_Tx_DMA_Channel, DISABLE);	  //关闭DMA通道	 
	if(Length > USART1_MAX_UART_TXBUF)
		Length = USART1_MAX_UART_TXBUF;
	for(int i = 0; i < Length; i++)
		usart1_tx_buf[i] = send_data[i];		
	DMA_SetCurrDataCounter(USART1_Tx_DMA_Channel,Length); //设置传输长度
	DMA_EnableChannel(USART1_Tx_DMA_Channel, ENABLE);     //使能DMA通道
}

void DMA1_Channel4_IRQHandler(void)
{
    if(DMA_GetIntStatus(USART1_Tx_DMA_INT, DMA1) != RESET)
    {            
        DMA_EnableChannel(USART1_Tx_DMA_Channel, DISABLE);
        DMA_ClrIntPendingBit(USART1_Tx_DMA_INT, DMA1);         
    }     
}
void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetIntStatus(USART1_Rx_DMA_INT, DMA1) != RESET)
    {            
        DMA_EnableChannel(USART1_Rx_DMA_Channel, DISABLE);
        DMA_ClrIntPendingBit(USART1_Rx_DMA_INT, DMA1);    
        DMA_SetCurrDataCounter(USART1_Rx_DMA_Channel,USART1_MAX_UART_RXBUF);
        DMA_EnableChannel(USART1_Rx_DMA_Channel, ENABLE);
    }     
}

