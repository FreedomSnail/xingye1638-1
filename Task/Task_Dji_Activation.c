/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name : Task_Dji_Activation.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : 陆志
** Created date :2016年1月30日13:42:50
** Version :
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :
** Modified date :
** Version :
** Description :
************************************************************************************************/
#include "includes.h"

OS_EVENT *SemDjiActivation; 
OS_STK TASK_DJI_ACTIVATION_STK[TASK_DJI_ACTIVATION_STK_SIZE];
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void Task_Dji_Activation(void* p_arg)
{
	INT8U err;
	u8 temp;
	(void) p_arg;		//防止编译器警告
	SemDjiActivation = OSSemCreate(0);
	DJI_Onboard_API_Activation_Init();
	while(1) {
		LOG_DJI("\r\ntry Activate!\r\n");
		DJI_Onboard_API_Activation();
		OSSemPend(SemDjiActivation,100,&err); 
		if(OS_ERR_NONE == err) {
			LOG_DJI("\r\nActivate ok!\r\n");
			break;
		}
	}
	while(1) {
		OSTimeDly(10);
		Adc_Filter();
		Device.V6s = Get_6S_Val();
		Device.V12s = Device.V6s;
		Device.PumpCurrent = Get_Pump_Current();
		temp = (u8)(Device.PumpCurrent*10);
		//USART_OUT(SERIAL_PORT_DEBUG,"6s=%d\r\n",Device.V6s);
		//USART_OUT(SERIAL_PORT_DEBUG,"c=%d\r\n",temp);
		Send_Msg_2_M100();
	}
}




