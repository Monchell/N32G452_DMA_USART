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

//printf�ض�����USART1
int fputc(int ch, FILE* f)
{
    USART_SendData(USART_PRINTF, (uint8_t)ch);
    while (USART_GetFlagStatus(USART_PRINTF, USART_FLAG_TXDE) == RESET);
    return (ch);
}


/**
 * @brief DMA���ڳ�ʼ������
 * @param USARTx����ʼ���Ĵ���,����USART1
 * @param uiBaudRate��������
 * @param callback_fun�����յ����ݴ������Ӻ������û����б�д����ʵ��Ȼ��Ѻ���ָ�����ȥ
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
		/* ��ʼ��GPIO,USART1Ĭ��ʹ��PA9��PA10��Ϊ����*/
		/* ʹ��GPIO�͸���ʱ�� */
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
		
		
		/*�����ж����ȼ�*/
		NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority				 = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
	}
	#endif
	/*�����ж����ȼ�*/
	
	/* ����USART*/
	USART_DeInit(USARTx);
	USART_InitStructure.BaudRate            = uiBaudRate;
	USART_InitStructure.WordLength          = USART_WL_8B;
	USART_InitStructure.StopBits            = USART_STPB_1;
	USART_InitStructure.Parity              = USART_PE_NO;
	USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
	USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;	
	USART_Init(USARTx, &USART_InitStructure);		
	
	//ʹ�ܴ��ڿ����ж�
	USART_ConfigInt(USARTx, USART_INT_IDLEF, ENABLE);
	
	/* ���ʹ�ܴ�����DMA���� */
	USART_EnableDMA(USARTx, USART_DMAREQ_RX | USART_DMAREQ_TX, ENABLE);
	USART_Enable(USARTx, ENABLE);
}



#if USART1_CONFIG == 1 
void uart1_dma_init(void)
{
	
	DMA_InitType DMA_InitStructure;
	NVIC_InitType NVIC_InitStructure;
	/* ʹ��DMAʱ��,��������鷳������ʹ���� */
	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA1, ENABLE);
	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA2, ENABLE);
	/* ����DMA USART����ͨ��*/
	DMA_DeInit(USART1_Tx_DMA_Channel);
	DMA_InitStructure.PeriphAddr 			= (uint32_t)&(USART1->DAT);				//�����ַ
	DMA_InitStructure.MemAddr    			= (uint32_t)usart1_tx_buf;					//�����̷��ͺͽ��չ���һ�����飬���յ�����֮����������
	DMA_InitStructure.Direction  			= DMA_DIR_PERIPH_DST;							//������ΪĿ���豸
	DMA_InitStructure.BufSize    			= USART1_MAX_UART_TXBUF;							//�������ݴ�С��ÿ�η��Ϳ������¶���
	DMA_InitStructure.PeriphInc     	= DMA_PERIPH_INC_DISABLE;
	DMA_InitStructure.DMA_MemoryInc 	= DMA_MEM_INC_ENABLE;
	DMA_InitStructure.PeriphDataSize 	= DMA_PERIPH_DATA_SIZE_BYTE;
	DMA_InitStructure.MemDataSize    	= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.CircularMode   	= DMA_MODE_NORMAL;
	DMA_InitStructure.Priority       	= DMA_PRIORITY_VERY_HIGH;
	DMA_InitStructure.Mem2Mem        	= DMA_M2M_DISABLE;
	DMA_Init(USART1_Tx_DMA_Channel, &DMA_InitStructure);
	DMA_RequestRemap(USART1_Tx_DMA_REMAP, DMA1, USART1_Tx_DMA_Channel, ENABLE);
	
	
	/* ʹ�ܴ�������ж� */
	DMA_ConfigInt(USART1_Tx_DMA_Channel, DMA_INT_TXC, ENABLE);
	/* ����DMA USART����*/
	DMA_InitStructure.PeriphAddr = (uint32_t)&(USART1->DAT);
	DMA_InitStructure.MemAddr    = (uint32_t)usart1_rx_buf;
	DMA_InitStructure.Direction  = DMA_DIR_PERIPH_SRC;
	DMA_InitStructure.BufSize    = USART1_MAX_UART_RXBUF;					//Ĭ�Ͻ������ݴ�С
	DMA_Init(USART1_Rx_DMA_Channel, &DMA_InitStructure);
	DMA_RequestRemap(USART1_Rx_DMA_REMAP, DMA1, USART1_Rx_DMA_Channel, ENABLE);
	/* ʹ�ܴ�������ж� */
	DMA_ConfigInt(USART1_Rx_DMA_Channel, DMA_INT_TXC, ENABLE);
	/* ��������ʼ״̬�رշ���ͨ������Ҫʱ�򿪣������ʼ�����֮���ֱ�ӷ���һ������ */
	DMA_EnableChannel(USART1_Tx_DMA_Channel, DISABLE);  
	/* ��DMA����ͨ�� */
	DMA_EnableChannel(USART1_Rx_DMA_Channel, ENABLE); 
	
	/*�������DMA��ص��жϣ�������������㹻�����жϿ��Բ����� */
	
	/* ����DMA�����ж�*/
	NVIC_InitStructure.NVIC_IRQChannel            = USART1_Tx_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* ����DMA�����ж�*/
	NVIC_InitStructure.NVIC_IRQChannel            = USART1_Rx_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
#endif


/*ʹ��DMA�������ݣ�����Ϊusartx_rx_buf�ڴ洢������
  uint16_t Length Ҫ���͵����ݳ���
*/

/**
 * @brief DMA���ڷ�������
 * @param USARTx�����õĴ���,����USART1
 * @param send_data�����������ݵ�����
 * @param Length�����ͳ���
 * @note  ���ɴ����������黺��������
 */
void uartx_send_data(USART_Module* USARTx, uint8_t* send_data, uint16_t Length)
{
	#if USART1_CONFIG == 1
	if(USARTx == USART1)
	{
		
		DMA_EnableChannel(USART1_Tx_DMA_Channel, DISABLE);	  //�ر�DMAͨ��	 
		if(Length > USART1_MAX_UART_TXBUF)
			Length = USART1_MAX_UART_TXBUF;
		for(int i = 0; i < Length; i++)
			usart1_tx_buf[i] = send_data[i];		
		DMA_SetCurrDataCounter(USART1_Tx_DMA_Channel,Length); //���ô��䳤��
		DMA_EnableChannel(USART1_Tx_DMA_Channel, ENABLE);     //ʹ��DMAͨ��		
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
	uint16_t rev_len = 0;      //��ǰ���ݸ���
	if(USART_GetIntStatus(USART1,USART_INT_IDLEF)!=RESET)
	{
		USART_ReceiveData(USART1);				//Ϊ������ж�
		//�ȹر�DMA����
		DMA_EnableChannel(USART1_Rx_DMA_Channel, DISABLE);
		//��һ��DMA�жϱ�־λ����Ϊû�д���Ҳ���Բ���
		DMA_ClrIntPendingBit(USART1_Rx_DMA_INT, DMA1);
		//ʵ�ʽ������ݸ���
		rev_len = USART1_MAX_UART_RXBUF - DMA_GetCurrDataCounter(USART1_Rx_DMA_Channel);		
		//�����ϲ�ʵ�ֵĻص�������Ҳ�ǵײ��ṩ���ϲ��hook����
		if(usart1_callback != NULL)
		{
			usart1_callback(rev_len,usart1_rx_buf);
		}
		//������ý��ո������򿪽���
		DMA_SetCurrDataCounter(USART1_Rx_DMA_Channel,USART1_MAX_UART_RXBUF); 
		DMA_EnableChannel(USART1_Rx_DMA_Channel, ENABLE);
	}
}
#endif

