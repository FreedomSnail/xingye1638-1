#ifndef _TASK_DJI_ACTIVATION_H
#define _TASK_DJI_ACTIVATION_H

#define		TASK_DJI_ACTIVATION_STK_SIZE	128
#define		TASK_DJI_ACTIVATION_PRIO		4


extern	OS_EVENT *SemDjiActivation; 
extern	OS_STK TASK_DJI_ACTIVATION_STK[];






extern	void Task_Dji_Activation(void* p_arg);

#endif
			   

 
