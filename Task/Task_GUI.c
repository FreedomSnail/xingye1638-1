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
** Created by : ף��һ
** Created date :2012��2��27��9:39:37
** Version :V1.1.0
** Description :ͨ��ģʽ�µĽ�����ʾ��������
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
	(void) p_arg;		//��ֹ����������
	msg =msg;
	while (1) {   
		msg = (u8 *)OSQPend(*pGuiQSem,  0, &err);			//�ȴ���Ϣ
		USART_OUT(USART1,"������������һ��\r\n");
	}
}



