/**
 * @file drv_usart.c
 * @author Monchell
 * @version v1.0.0
 * @brief DMA��������
 * @note 
 * @copyright free
 */
#include "drv_usart.h"
#include "stdio.h"


uint8_t usart1_rx_buf[USART1_MAX_UART_RXBUF];
uint8_t usart1_tx_buf[USART1_MAX_UART_TXBUF];

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
		/* ��ʼ��GPIO,USARTĬ��ʹ��PA9��PA10��Ϊ����*/
		/* ʹ��GPIO�͸���ʱ��ʱ�� */
		RCC_EnableAPB2PeriphClk(USART1_GPIO_CLK|RCC_APB2_PERIPH_AFIO, ENABLE);						
		/* ʹ��USARTʱ�� */
		USART1_APBxClkCmd(USART1_CLK, ENABLE);	
		/* �������GPIO */
		GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin            = USART1_TxPin; 
		GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_50MHz;		
		GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_AF_PP;
		GPIO_InitPeripheral(USART1_GPIO, &GPIO_InitStructure);
		
		GPIO_InitStructure.Pin            = USART1_RxPin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitPeripheral(USART1_GPIO, &GPIO_InitStructure);

		/* ����USART*/
		USART_DeInit(USARTx);
		USART_InitStructure.BaudRate            = uiBaudRate;
		USART_InitStructure.WordLength          = USART_WL_8B;
		USART_InitStructure.StopBits            = USART_STPB_1;
		USART_InitStructure.Parity              = USART_PE_NO;
		USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
		USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;	
		
		USART_Init(USARTx, &USART_InitStructure);		
		USART_Enable(USARTx, ENABLE);
		//printf("1111");
	}
	

//	/* ʹ��USART��DMA */
//	USART_EnableDMA(USARTx, USART_DMAREQ_RX | USART_DMAREQ_TX, ENABLE);
//	/* ʹ��USART�����ж�*/
//	USART_ConfigInt(USARTx, USART_INT_IDLEF, ENABLE);
//	USART_Enable(USARTx, ENABLE);			//ʹ��USART	
//	/*�����ж�*/
//	NVIC_InitStructure.NVIC_IRQChannel                   = USARTx_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPriority           = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
////	NVIC_Init(&NVIC_InitStructure);
}




//void uart_dma_init(void)
//{
//	DMA_InitType DMA_InitStructure;
//	NVIC_InitType NVIC_InitStructure;
//	/* ʹ��DMAʱ�� */
//	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA, ENABLE);
//	/* ����DMA USART����ͨ��*/
//	DMA_DeInit(USARTx_Tx_DMA_Channel);
//	DMA_InitStructure.PeriphAddr = (uint32_t)&(USARTx->DAT);				//�����ַ
//	DMA_InitStructure.MemAddr    = (uint32_t)usartx_rx_buf;					//�����̷��ͺͽ��չ���һ�����飬���յ�����֮����������
//	DMA_InitStructure.Direction  = DMA_DIR_PERIPH_DST;							//������ΪĿ���豸
//	DMA_InitStructure.BufSize    = USARTx_MAX_UART_BUF;							//�������ݴ�С��ÿ�η��Ϳ������¶���
//	DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
//	DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
//	DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
//	DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.CircularMode   = DMA_MODE_NORMAL;
//	DMA_InitStructure.Priority       = DMA_PRIORITY_VERY_HIGH;
//	DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
//	DMA_Init(USARTx_Tx_DMA_Channel, &DMA_InitStructure);
//	DMA_RequestRemap(USARTx_Tx_DMA_REMAP, DMA, USARTx_Tx_DMA_Channel, ENABLE);
//	/* ʹ�ܴ�������ж� */
//	DMA_ConfigInt(USARTx_Tx_DMA_Channel, DMA_INT_TXC, ENABLE);
//	/* ����DMA USART����*/
//	DMA_InitStructure.PeriphAddr = (uint32_t)&(USARTx->DAT);
//	DMA_InitStructure.MemAddr    = (uint32_t)usartx_rx_buf;
//	DMA_InitStructure.Direction  = DMA_DIR_PERIPH_SRC;
//	DMA_InitStructure.BufSize    = USARTx_MAX_UART_BUF;					//Ĭ�Ͻ������ݴ�С
//	DMA_Init(USARTx_Rx_DMA_Channel, &DMA_InitStructure);
//	DMA_RequestRemap(USARTx_Rx_DMA_REMAP, DMA, USARTx_Rx_DMA_Channel, ENABLE);
//	/* ʹ�ܴ�������ж� */
//	DMA_ConfigInt(USARTx_Rx_DMA_Channel, DMA_INT_TXC, ENABLE);
//	/* ��������ʼ״̬�رշ���ͨ������Ҫʱ�򿪣������ʼ�����֮���ֱ�ӷ���һ������ */
//	DMA_EnableChannel(USARTx_Tx_DMA_Channel, DISABLE);  
//	/* ��DMA����ͨ�� */
//	DMA_EnableChannel(USARTx_Rx_DMA_Channel, ENABLE); 
//	/*�������DMA��ص��жϣ�������������㹻�����жϿ��Բ����� */
//	/* ����DMA�����ж�*/
//	NVIC_InitStructure.NVIC_IRQChannel            = USARTx_Tx_DMA_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPriority    = 3;
//	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	/* ����DMA�����ж�*/
//	NVIC_InitStructure.NVIC_IRQChannel            = USARTx_Rx_DMA_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPriority    = 2;
//	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}


///*ʹ��DMA�������ݣ�����Ϊusartx_rx_buf�ڴ洢������
//  uint16_t Length Ҫ���͵����ݳ���
//*/
//void uartx_send_data(uint16_t Length)
//{
//	DMA_EnableChannel(USARTx_Tx_DMA_Channel, DISABLE);	  //�ر�DMAͨ��	 
//	DMA_SetCurrDataCounter(USARTx_Tx_DMA_Channel,Length); //���ô��䳤��
//	DMA_EnableChannel(USARTx_Tx_DMA_Channel, ENABLE);     //ʹ��DMAͨ��
//}

