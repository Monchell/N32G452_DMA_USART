#include"timer.h"
/**
 * @brief  Configures the different system clocks.
 */
 
 
void mc_timer2_init(void)
{
  /* TIM2 clock enable */
	//ʹ��TIM2��ʱ��
  RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM2, ENABLE);
	

	/* Enable the TIM2 global Interrupt */
	//�ж�����
	NVIC_InitType NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn;//ѡ��timer2�ж�
	//���ȼ����ã���ռ���ȼ��������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitType TIM_TimeBaseStructure;

  /* Time base configuration */
	//��Ƶ�ͼ�������
  TIM_InitTimBaseStruct(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.Period    = (uint16_t) 1000 - 1;
  TIM_TimeBaseStructure.Prescaler = (uint16_t) 72 - 1;
	TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
  TIM_InitTimeBase(TIM2, &TIM_TimeBaseStructure); 	//д�붨ʱ������	
	
	TIM_ClearFlag(TIM2,TIM_FLAG_UPDATE);//��������жϱ�־λ
	TIM_ConfigInt(TIM2,TIM_INT_UPDATE, ENABLE);//ʹ�ܸ����ж�
	
	TIM_Enable(TIM2,ENABLE);//��ʼ����
	
}