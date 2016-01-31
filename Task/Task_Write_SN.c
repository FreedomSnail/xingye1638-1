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
** Created by : 祝定一
** Created date :2015年12月21日11:51:55
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
			LOG_WRITE_SN("收到消息包\r\n");
			//检查格式是否正确
			err = 0;
			if((Uart2.RxDataSecondBuf[0]!='T')||(Uart2.RxDataSecondBuf[1]!='X')||(Uart2.RxDataSecondBuf[2]!='A')) {
				err = 1;
			}
			for(i=3;i<TXA_SN_LENTH;i++) {
				if((Uart2.RxDataSecondBuf[i]<'0')||(Uart2.RxDataSecondBuf[i]>'9')) {
					err = 1;
				}
			}
			if(err == 0) {	//格式正确
				LOG_WRITE_SN("格式正确，写入\r\n");
				USART_OUT(SERIAL_PORT_DJI_SDK,"TEST\r\n");
				Set_Product_Number(Uart2.RxDataSecondBuf,TXA_SN_LENTH);
				GprsCmd.PermissionLocal = PERMISSION_ALLOW;
				Set_Product_Permission();
			} else {
				LOG_WRITE_SN("格式错误，请重新写入\r\n");
			}
		} 
		//LOG_WRITE_SN("等待写码输入....\r\n");
		LOG_WRITE_SN(GprsCmd.SNLocal);
		LOG_WRITE_SN("\r\n");
		SEND_SN_2_WIFI(GprsCmd.SNLocal);
		SEND_SN_2_WIFI("\r\n");
	}
}

