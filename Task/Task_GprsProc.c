/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name : Task_GprsProc.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : 祝定一
** Created date :2012年8月25日10:02:44
** Version :V1.1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :
** Modified date :
** Version :
** Description :
************************************************************************************************/

#include "includes.h"

OS_STK TaskGprsProcStk[TASK_GPRS_PROC_STK_SIZE];
OS_EVENT 	*GprsQSem; 
void     	*GprsQMsgTbl[GPRS_RESOURCES];

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
void Task_Gprs_Proc(void* p_arg)
{
	INT8U err;
	u8 end = CIPSEND_END_CHAR;
	u8 GprsDataSend[GPRS_CMD_SEND_LENTH+1];
	u8 offset;
	u16 timeout=100;
	//u8 TryCheckChinaMobileCnt;		//尝试
	//u8 TryConnectRemoteFailedCnt;		//尝试连接服务器失败次数
	//u8 SendDataFailedCnt;				//发送数据失败次数
	//u8 GetRemoteDataFailedCnt;		//请求服务器数据失败次数
	//u8 GetRemoteDataSuccessCnt = 0;	//请求服务器数据成功次数
	(void) p_arg;
	GprsQSem = OSQCreate(&GprsQMsgTbl[0], GPRS_RESOURCES);
	while(1) {
		OSQPend(GprsQSem,timeout,&err);
		switch(err) {
			case OS_ERR_NONE:
				switch(GsmCmd.GsmCmdStage) {
					case GSM_CMD_STAGE_AT_COPS:
						if(strstr((const char *)GsmCmd.RxDataAt, "CHINA MOBILE") ) {//检测是否收到 CHINA MOBILE 服务商信息。如果收到证明是连接上网络了
							GsmCmd.GsmCmdStage = GSM_CMD_STAGE_AT_CIPCLOSE;
							LOG_SIM900("CHINA\r\n");
						}
						break;
					case GSM_CMD_STAGE_AT_CIPSTART:
						if(strstr((const char *)GsmCmd.RxDataAt, "CONNECT OK") ) {//检测是否收到 CONNECT OK ,如果这连接成功
							GsmCmd.GsmCmdStage = GSM_CMD_STAGE_AT_CIPSEND;
							LOG_SIM900("CONNECT OK\r\n");
						}
						break;
					case GSM_CMD_STAGE_AT_CIPSEND:
						if(strstr((const char *)GsmCmd.RxDataAt, "SEND OK") ) {//检测是否收到 SEND OK，收到证明发送数据成功
							GsmCmd.GsmCmdStage = GSM_CMD_STAGE_REMOTE_RESPOND;
							LOG_SIM900("SEND OK\r\n");
						}
						break;
					case GSM_CMD_STAGE_REMOTE_RESPOND:
						if(strstr((const char *)GsmCmd.RxDataAt, (const char *)GprsCmd.SNLocal) ) {//检测是否收到 与本机相同的机身编码，收到证明
							LOG_SIM900("Revceid server respond!\r\n");
							if(strstr((const char *)GsmCmd.RxDataAt, ",NO") ) {			//禁止,不需要重发
								timeout=100;
								GsmCmd.GsmCmdStage = GSM_CMD_STAGE_CLOSE_NET;	//关闭网络
								GprsCmd.PermissionRemote = PERMISSION_PROHIBIT;
								LOG_SIM900("PERMISSION_PROHIBIT\r\n");
							} else if(strstr((const char *)GsmCmd.RxDataAt, ",NA") ){		//禁止,需重发数据提供服务器确认
								timeout=100;
								GsmCmd.GsmCmdStage = GSM_CMD_STAGE_AT_CIPSEND;	//返回发送状态
								GprsCmd.PermissionRemote = PERMISSION_PROHIBIT;
								LOG_SIM900("NNNNN AGAIN\r\n");
							} else if(strstr((const char *)GsmCmd.RxDataAt, ",YO") ){		//授权使用,不需要重发
								timeout=100;
								GsmCmd.GsmCmdStage = GSM_CMD_STAGE_CLOSE_NET;	//关闭网络
								GprsCmd.PermissionRemote = PERMISSION_ALLOW;
								LOG_SIM900("PERMISSION_ALLOW\r\n");
							} else if(strstr((const char *)GsmCmd.RxDataAt, ",YA") ){		//授权使用,需重发数据提供服务器确认
								timeout=100;
								GsmCmd.GsmCmdStage = GSM_CMD_STAGE_AT_CIPSEND;	//返回发送状态
								GprsCmd.PermissionRemote = PERMISSION_ALLOW;
								LOG_SIM900("YYYYYYY AGAIN\r\n");
							}
							if(GprsCmd.PermissionLocal != GprsCmd.PermissionRemote) {	//不一致才修改保存
								GprsCmd.PermissionLocal = GprsCmd.PermissionRemote;
								Set_Product_Permission();
								LOG_SIM900("Permission has saved!\r\n");
								#if 0
								if(GprsCmd.PermissionLocal == PERMISSION_ALLOW) {	//获得使用权限
									LOG_SIM900("重新获取权限!\r\n");
									//建立编解码处理任务------------------------------------------------------
								   	OSTaskCreateExt(Task_CODEC,
								   					(void *)0,
								   					(OS_STK *)&TaskDecodeStk[TASK_DECODE_STK_SIZE-1],
								   					TASK_DECODE_PRIO,
								   					TASK_DECODE_PRIO,
								   					(OS_STK *)&TaskDecodeStk[0],
										                    TASK_DECODE_STK_SIZE,
										                    (void *)0,
										                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR); 

									USART_ITConfig(SERIAL_PORT_DJI_SDK, USART_IT_RXNE, ENABLE);                    //使能接收中断
									ADC_SoftwareStartConvCmd(ADC1, ENABLE);
									DMA_Cmd(DMA1_Channel1, ENABLE); //启动DMA通道
									TIM_Cmd(TIM2, ENABLE);
									
								}
								#endif
							}
						}
						break;
					default:
						break;
				}
				break;
			case OS_ERR_TIMEOUT:
				switch(GsmCmd.GsmCmdStage) {
					case GSM_CMD_STAGE_AT_COPS:
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_COPS);
						LOG_SIM900(AT_STRING_COPS);
						break;
					case GSM_CMD_STAGE_AT_CIPCLOSE:
						GsmCmd.GsmCmdStage++;
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CIPCLOSE);
						LOG_SIM900(AT_STRING_CIPCLOSE);
						break;
					case GSM_CMD_STAGE_AT_CIPSHUT:
						GsmCmd.GsmCmdStage++;
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CIPSHUT);
						LOG_SIM900(AT_STRING_CIPSHUT);
						break;
					case GSM_CMD_STAGE_AT_CREG:
						GsmCmd.GsmCmdStage++;
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CREG);
						LOG_SIM900(AT_STRING_CREG);
						break;
					case GSM_CMD_STAGE_AT_CGATT:
						GsmCmd.GsmCmdStage++;
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CGATT);
						LOG_SIM900(AT_STRING_CGATT);
						break;
					case GSM_CMD_STAGE_AT_CMEE:
						GsmCmd.GsmCmdStage++;
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CMEE);
						LOG_SIM900(AT_STRING_CMEE);
						break;
					case GSM_CMD_STAGE_AT_CSTT:
						GsmCmd.GsmCmdStage++;
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CSTT);
						LOG_SIM900(AT_STRING_CSTT);
						break;
					case GSM_CMD_STAGE_AT_CIICR:
						GsmCmd.GsmCmdStage++;
						timeout = 400;
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CIICR);
						LOG_SIM900(AT_STRING_CIICR);
						break;
					case GSM_CMD_STAGE_AT_CIFSR:
						GsmCmd.GsmCmdStage++;
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CIFSR);
						LOG_SIM900(AT_STRING_CIFSR);
						break;
					case GSM_CMD_STAGE_AT_CIPSTART:
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CIPSTART);
						LOG_SIM900(AT_STRING_CIPSTART);
						break;
					case GSM_CMD_STAGE_AT_CIPSEND:
						timeout = 1500;
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CIPSEND);
						LOG_SIM900(AT_STRING_CIPSEND);
						OSTimeDly(30);
						memcpy(GprsDataSend,GprsCmd.SNLocal,TXA_SN_LENTH);
						offset = TXA_SN_LENTH;
						GprsDataSend[offset] = ',';
						offset++;
						if(GprsCmd.PermissionLocal == PERMISSION_ALLOW) {
							GprsDataSend[offset] = 'Y';
						} else {
							GprsDataSend[offset] = 'N';
						}
						offset++;
						GprsDataSend[offset] = ',';
						offset++;
						memcpy(GprsDataSend+offset,GprsCmd.BaseStationLongitude,GPRS_BASE_STATION_LONGITUDE_LENTH);
						offset += GPRS_BASE_STATION_LONGITUDE_LENTH;
						GprsDataSend[offset] = ',';
						offset++;
						memcpy(GprsDataSend+offset,GprsCmd.BaseStationLatitude,GPRS_BASE_STATION_LATITUDE_LENTH);
						offset += GPRS_BASE_STATION_LATITUDE_LENTH;
						GprsDataSend[GPRS_CMD_SEND_LENTH] = 0;
						USART_OUT(SERIAL_PORT_SIM900A,GprsDataSend);
						LOG_SIM900(GprsDataSend);
						OSTimeDly(100);
						USART_OUT(SERIAL_PORT_SIM900A,&end);//以0x1a结束
						LOG_SIM900(&end);//以0x1a结束
						break;
					case GSM_CMD_STAGE_REMOTE_RESPOND:
						GsmCmd.GsmCmdStage = GSM_CMD_STAGE_AT_CIPSEND;	//如果超时收不到，则转回发送状态
						break;
					case GSM_CMD_STAGE_CLOSE_NET:
						GsmCmd.GsmCmdStage = GSM_CMD_STAGE_IDLE;
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CIPCLOSE);
						LOG_SIM900(AT_STRING_CIPCLOSE);
						OSTimeDly(100);
						USART_OUT(SERIAL_PORT_SIM900A,AT_STRING_CIPSHUT);
						LOG_SIM900(AT_STRING_CIPSHUT);
						break;
					case GSM_CMD_STAGE_IDLE:
						LOG_SIM900("Communicate Success!\r\n");
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
}

