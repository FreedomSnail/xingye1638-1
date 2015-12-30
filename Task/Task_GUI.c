/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name : Task_GUI_General.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : 祝定一
** Created date :2012年2月27日9:39:37
** Version :V1.1.0
** Description :通用模式下的界面显示处理任务
**-----------------------------------------------------------------------------------------------
** Modified by :
** Modified date :
** Version :
** Description :
************************************************************************************************/
#include "includes.h"

OS_EVENT **pGuiQSem; 
OS_EVENT *GuiNormalQSem; 
OS_STK TaskGuiStk[TASK_GUI_STK_SIZE];
void     *GuiNormalQSemTbl[GUI_RESOURCES];
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
void Task_GUI(void* p_arg)
{
	INT8U err;
	u8* msg;
	(void) p_arg;		//防止编译器警告
	msg =msg;
	while (1) {   
		msg = (u8 *)OSQPend(*pGuiQSem,  0, &err);			//等待消息
		USART_OUT(USART1,"界面任务运行一次\r\n");
	}
}



