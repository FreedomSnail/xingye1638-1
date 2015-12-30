#ifndef _TASK_GPRS_PROC_H
#define _TASK_GPRS_PROC_H

#define  TASK_GPRS_PROC_STK_SIZE                        128
#define  TASK_GPRS_PROC_PRIO                             10
#define	 GPRS_RESOURCES									5






extern	OS_STK TaskGprsProcStk[];
extern	OS_EVENT 	*GprsQSem; 

extern	void Task_Gprs_Proc(void* p_arg);
#endif
