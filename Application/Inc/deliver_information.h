/**
 * @file deliver_information.h
 * @author Monchell
 * @version v1.0.0
 * @brief ����ת�����񣬽��ܴ��ڷ��͵���ϢȻ����ת����ȥ
 * @copyright free
 */
#ifndef __DELIVER_INFORMATION__
#define __DELIVER_INFORMATION__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g45x.h"
//�����ջ��С
#define DELIVER_INFORMATION_TASK_SIZE 		128
	
//�������ȼ�
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
