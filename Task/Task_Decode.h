#ifndef _TASK_DECODE_H
#define _TASK_DECODE_H

#define		TASK_DECODE_STK_SIZE	128
#define		TASK_DECODE_PRIO		1
#define		DECODE_RESOURCES		10



extern	OS_STK TaskDecodeStk[];
extern	OS_EVENT *DecodeQSem; 


extern	void Task_Decode(void* p_arg);


#endif


