#ifndef _TASK_WRITE_SN_H
#define _TASK_WRITE_SN_H



#define		TASK_WRITE_SN_SIZE	128
#define		TASK_WRITE_SN_PRIO	5
#define		WRITE_SN_RESOURCES	10





extern	OS_STK 		TaskWriteSNStk[];
extern  OS_EVENT 	*WriteSNQSem; 

extern	void Task_Write_SN(void* p_arg);

#endif


