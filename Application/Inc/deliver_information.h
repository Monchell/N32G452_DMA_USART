/**
 * @file deliver_information.h
 * @author Monchell
 * @version v1.0.0
 * @brief 串口转发任务，接受串口发送的消息然后再转发回去
 * @copyright free
 */
#ifndef __DELIVER_INFORMATION__
#define __DELIVER_INFORMATION__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g45x.h"
//任务堆栈大小
#define DELIVER_INFORMATION_TASK_SIZE 		128
	
//任务优先级
#define DELIVER_INFORMATION_TASK_PRIO			2
void deliver_information_task(void *pvParameters);
#ifdef __cplusplus
}
#endif

#endif /* __DELIVER_INFORMATION__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
