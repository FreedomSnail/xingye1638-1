#ifndef _TASK_DECODE_H
#define _TASK_DECODE_H

#define		TASK_PWM_CTRL_PUMP_STK_SIZE	128
#define		TASK_PWM_CTRL_PUMP_PRIO		4



#define		TASK_USART_DJI_CTRL_PUMP_STK_SIZE	128
#define		TASK_USART_DJI_CTRL_PUMP_PRIO		3
#define		USART_DJI_CTRL_RESOURCES	10

#define		TASK_CODEC_STK_SIZE	128
#define		TASK_CODEC_PRIO		6


#define		TASK_SEND_PUMP_BOARD_INFO_STK_SIZE	128
#define		TASK_SEND_PUMP_BOARD_INFO_PRIO		7

#define		TASK_BEEP_TIP_STK_SIZE	128
#define		TASK_BEEP_TIP_PRIO		11



extern	OS_STK TaskPwmCtrlPumpStk[];
extern	OS_STK TaskUsartDjiCtrlPumpStk[];
extern	OS_EVENT *UsartDjiCtrlPumpQsem; 


extern  OS_STK 		TaskCodecStk[];
extern	OS_EVENT  	*SemDjiCodec;

extern  OS_EVENT  	*SemPWM;

extern  OS_STK 		TaskSendPumpBoardInfoStk[];

extern  OS_STK 		TaskBeepTipStk[];


void Task_Dji_SDK_Codec(void *p_arg);

void Task_PWM_Ctrl_Pump(void* p_arg);

void Task_Usart_DJI_Ctrl_Pump(void* p_arg);

void Task_Send_Pump_Board_Info(void* p_arg);

void Task_Beep_Tip(void* p_arg);

#endif


