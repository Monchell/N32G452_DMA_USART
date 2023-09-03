/**
 * @file timer.h
 * @author Monchell
 * @version v1.0.0
 * @brief 包含操作系统的systick的配置以及延时函数的配置
 * @note 移植到工程里面可以直接使用的
 * @copyright free
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "n32g45x.h"
void NZ_Delay_init(void);
void NZ_Delay_us(u32 nus);
void NZ_Delay_ms(u32 nms);
void NZ_Delay_xms(u32 nms);
#include "n32g45x.h"
#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__ */
