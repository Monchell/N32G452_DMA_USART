/**
 * @file timer.c
 * @author Monchell
 * @version v1.0.0
 * @brief 包含操作系统的systick的配置以及延时函数的配置
 * @note 移植到工程里面可以直接使用的
 * @copyright free
 */
#include "timer.h"
#include "Freertos.h"
#include "task.h"

/**
 * @brief 初始化SYSTICK,并且配置延时函数
 * @note  SYSTICK的时钟固定为HCLK时钟
 */
static u32  fac_us = 0;	 //us延时倍乘数			   
static u32  fac_ms = 0;	 //ms延时倍乘数
void NZ_Delay_init(void)
{
	u32 reload;

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//选择外部时钟HCLK 144M，此时滴答1s可以跳144M下
	fac_us = (SystemCoreClock / (1000000));					//SystemCoreClock为系统时钟，fac_us算的是1us滴答多少M跳

	reload = (SystemCoreClock / 1000000);						//每秒钟的计数次数 单位为M
	reload *= (1000000 / configTICK_RATE_HZ);       //根据configTICK_RATE_HZ设定溢出时间
	fac_ms = (1000 / configTICK_RATE_HZ);				    //代表OS可以毫秒延时的最少单位,是OS的频率（这里也是滴答溢出的频率）决定的

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;			  //开启SYSTICK中断
	SysTick->LOAD=reload; 					     					  //每1M/configTICK_RATE_HZ断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 				//开启SYSTICK   
}


/**
 * @brief us级别延时函数
 * @param nus：需要延时的微秒数
 * @note  如果延时的时间到了毫秒级别，则用NZ_Delay_xms实现
 */
void NZ_Delay_us(u32 nus)
{
		u32 ticks;
		u32 told,tnow,tcnt = 0;
		u32 reload = SysTick->LOAD;		  //LOAD的值	    	 
		ticks = nus*fac_us; 						//需要的节拍数 
		told  = SysTick->VAL;        		//刚进入时的计数器值
		while(1)
		{
				tnow = SysTick->VAL;	
				if(tnow != told)
				{	    
						if(tnow<told)   //这里注意一下SYSTICK是一个递减的计数器就可以了
							tcnt += told-tnow;	
						else 
							tcnt += reload-tnow+told;	   
						
						told = tnow;
						if(tcnt >= ticks)	//时间超过/等于要延迟的时间,则退出
							break;			
				}  
		};
}

/**
 * @brief ms级别延时函数,会引起系统调度
 * @param nms：需要延时的毫秒数
 */

void NZ_Delay_ms(u32 nms)
{
		if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) //系统已经运行
		{		
				if(nms >= fac_ms)							//延时的时间大于OS的最少时间周期 
				{ 
						vTaskDelay(nms / fac_ms);	 		//FreeRTOS延时
				}
				
				nms %= fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
		}
		NZ_Delay_us((u32)(nms*1000));				//普通方式延时    
}

/**
 * @brief ms级别延时函数,不会引起系统调度
 * @param nms：需要延时的毫秒数
 */
void NZ_Delay_xms(u32 nms)
{
		u32 i;
		for(i=0;i<nms;i++) NZ_Delay_us(1000);  
}



/**
 * @brief  This function handles SysTick Handler.
 */
extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)// 系统已经运行
	{
		xPortSysTickHandler();
	}
}


