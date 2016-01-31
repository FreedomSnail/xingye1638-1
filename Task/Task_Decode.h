#ifndef _TASK_DECODE_H
#define _TASK_DECODE_H

#define		TASK_PWM_CTRL_PUMP_STK_SIZE	128
#define		TASK_PWM_CTRL_PUMP_PRIO		3
#define		USART_DJI_CTRL_RESOURCES		10

#define		TASK_CODEC_STK_SIZE	128
#define		TASK_CODEC_PRIO		2


extern	OS_STK TaskPwmCtrlPumpStk[];
extern	OS_EVENT *UsartDjiCtrlPumpQsem; 


extern  OS_STK 		TaskCodecStk[];
extern	OS_EVENT  	*SemDjiCodec;

extern  OS_EVENT  	*SemPWM;


void Task_Dji_SDK_Codec(void *p_arg);

void Task_PWM_Ctrl_Pump(void* p_arg);

void Task_Usart_DJI_Ctrl_Pump(void* p_arg);

#endif


