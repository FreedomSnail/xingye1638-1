/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name : Task_Write_SN.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : ף��һ
** Created date :2015��12��21��11:51:55
** Version :V1.1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :
** Modified date :
** Version :
** Description :
************************************************************************************************/

#include "includes.h"

OS_STK TaskWriteSNStk[TASK_WRITE_SN_SIZE];
OS_EVENT 	*WriteSNQSem; 
void     	*WriteSNQMsgTbl[WRITE_SN_RESOURCES];

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
void Task_Write_SN(void* p_arg)
{
	INT8U err;
	u8 i;
	(void)p_arg;	
	WriteSNQSem = OSQCreate(&WriteSNQMsgTbl[0], 6);
	while(1) {
		OSQPend(WriteSNQSem,  100, &err);
		if(err == OS_ERR_NONE) {
			LOG_WRITE_SN("�յ���Ϣ��\r\n");
			//����ʽ�Ƿ���ȷ
			err = 0;
			if((Uart2.RxDataSecondBuf[0]!='T')||(Uart2.RxDataSecondBuf[1]!='X')||(Uart2.RxDataSecondBuf[2]!='A')) {
				err = 1;
			}
			for(i=3;i<TXA_SN_LENTH;i++) {
				if((Uart2.RxDataSecondBuf[i]<'0')||(Uart2.RxDataSecondBuf[i]>'9')) {
					err = 1;
				}
			}
			if(err == 0) {	//��ʽ��ȷ
				LOG_WRITE_SN("��ʽ��ȷ��д��\r\n");
				USART_OUT(SERIAL_PORT_DJI_SDK,"TEST\r\n");
				Set_Product_Number(Uart2.RxDataSecondBuf,TXA_SN_LENTH);
				GprsCmd.PermissionLocal = PERMISSION_ALLOW;
				Set_Product_Permission();
			} else {
				LOG_WRITE_SN("��ʽ����������д��\r\n");
			}
		} 
		//LOG_WRITE_SN("�ȴ�д������....\r\n");
		LOG_WRITE_SN(GprsCmd.SNLocal);
		LOG_WRITE_SN("\r\n");
		SEND_SN_2_WIFI(GprsCmd.SNLocal);
		SEND_SN_2_WIFI("\r\n");
	}
}

