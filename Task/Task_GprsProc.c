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
	u8 flag=0;
	u16 timeout=100;
	GsmCmdStage_TypeEnum gsmCmdStage = GSM_CMD_STAGE_AT_COPS;
	Permission_TypeEnum PermissionRemote;	//服务器授权
	//u8 TryCheckChinaMobileCnt;		//尝试
	//u8 TryConnectRemoteFailedCnt;		//尝试连接服务器失败次数
	//u8 SendDataFailedCnt;				//发送数据失败次数
	//u8 GetRemoteDataFailedCnt;		//请求服务器数据失败次数
	//u8 GetRemoteDataSuccessCnt = 0;	//请求服务器数据成功次数
	(void) p_arg;
	GprsQSem = OSQCreate(&GprsQMsgTbl[0], GPRS_RESOURCES);
	if(pumpBoardInfo.isSNSave == SN_SAVE_NO) {
		while(1) {
			OSTimeDly(100);
			LOG_SIM900("No SN,please write sn!\r\n");
		}
	}
	while(1) {
		OSQPend(GprsQSem,timeout,&err);
		switch(err) {
			case OS_ERR_NONE:
				switch(gsmCmdStage) {
					case GSM_CMD_STAGE_AT_COPS:
						if(strstr((const char *)GsmCmd.RxDataAt, "CHINA MOBILE") ) {//检测是否收到 CHINA MOBILE 服务商信息。如果收到证明是连接上网络了
							gsmCmdStage = GSM_CMD_STAGE_AT_CIPCLOSE;
							LOG_SIM900("CHINA\r\n");
						}
						break;
					case GSM_CMD_STAGE_AT_CIPSTART:
						if(strstr((const char *)GsmCmd.RxDataAt, "CONNECT OK") ) {//检测是否收到 CONNECT OK ,如果这连接成功
							gsmCmdStage = GSM_CMD_STAGE_AT_CIPSEND;
							LOG_SIM900("CONNECT OK\r\n");
						}
						break;
					case GSM_CMD_STAGE_AT_CIPSEND:
						if(strstr((const char *)GsmCmd.RxDataAt, "SEND OK") ) {//检测是否收到 SEND OK，收到证明发送数据成功
							gsmCmdStage = GSM_CMD_STAGE_REMOTE_RESPOND;
							LOG_SIM900("SEND OK\r\n");
						}
						break;
					case GSM_CMD_STAGE_REMOTE_RESPOND:
						if(strstr((const char *)GsmCmd.RxDataAt, (const char *)pumpBoardInfo.deviceSNStr) ) {//检测是否收到 与本机相同的机身编码，收到证明
							LOG_SIM900("Revceid server respond!\r\n");
							if(strstr((const char *)GsmCmd.RxDataAt, ",NO") ) {			//禁止,不需要重发
								timeout=100;
								gsmCmdStage = GSM_CMD_STAGE_CLOSE_NET;	//关闭网络
								PermissionRemote = PERMISSION_PROHIBIT;
								LOG_SIM900("PERMISSION_PROHIBIT\r\n");
							} else if(strstr((const char *)GsmCmd.RxDataAt, ",NA") ){		//禁止,需重发数据提供服务器确认
								timeout=100;
								gsmCmdStage = GSM_CMD_STAGE_AT_CIPSEND;	//返回发送状态
								PermissionRemote = PERMISSION_PROHIBIT;
								LOG_SIM900("NNNNN AGAIN\r\n");
							} else if(strstr((const char *)GsmCmd.RxDataAt, ",YO") ){		//授权使用,不需要重发
								timeout=100;
								gsmCmdStage = GSM_CMD_STAGE_CLOSE_NET;	//关闭网络
								PermissionRemote = PERMISSION_ALLOW;
								LOG_SIM900("PERMISSION_ALLOW\r\n");
							} else if(strstr((const char *)GsmCmd.RxDataAt, ",YA") ){		//授权使用,需重发数据提供服务器确认
								timeout=100;
								gsmCmdStage = GSM_CMD_STAGE_AT_CIPSEND;	//返回发送状态
								PermissionRemote = PERMISSION_ALLOW;
								LOG_SIM900("YYYYYYY AGAIN\r\n");
							}
							if(pumpBoardInfo.permission != PermissionRemote) {	//不一致才修改保存
								pumpBoardInfo.permission = PermissionRemote;
								Set_Product_Permission();
								LOG_SIM900("Permission has saved!\r\n");
								flag = 1;
								#if 0
								if(GprsCmd.permission == PERMISSION_ALLOW) {	//获得使用权限
									LOG_SIM900("重新获取权限!\r\n");
									//建立编解码处理任务------------------------------------------------------
								   	OSTaskCreateExt(Task_CODEC,
								   					(void *)0,
								   					(OS_STK *)&TaskPwmCtrlPumpStk[TASK_PWM_CTRL_PUMP_STK_SIZE-1],
								   					TASK_PWM_CTRL_PUMP_PRIO,
								   					TASK_PWM_CTRL_PUMP_PRIO,
								   					(OS_STK *)&TaskPwmCtrlPumpStk[0],
										                    TASK_PWM_CTRL_PUMP_STK_SIZE,
										                    (void *)0,
										                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR); 

									USART_ITConfig(SERIAL_PORT_FLIGHT_CTRL_BOARD, USART_IT_RXNE, ENABLE);                    //使能接收中断
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
				switch(gsmCmdStage) {
					case GSM_CMD_STAGE_AT_COPS:
						SIM900_AT_CMD_SEND(AT_STRING_COPS);
						LOG_SIM900(AT_STRING_COPS);
						break;
					case GSM_CMD_STAGE_AT_CIPCLOSE:
						gsmCmdStage++;
						SIM900_AT_CMD_SEND(AT_STRING_CIPCLOSE);
						LOG_SIM900(AT_STRING_CIPCLOSE);
						break;
					case GSM_CMD_STAGE_AT_CIPSHUT:
						gsmCmdStage++;
						SIM900_AT_CMD_SEND(AT_STRING_CIPSHUT);
						LOG_SIM900(AT_STRING_CIPSHUT);
						break;
					case GSM_CMD_STAGE_AT_CREG:
						gsmCmdStage++;
						SIM900_AT_CMD_SEND(AT_STRING_CREG);
						LOG_SIM900(AT_STRING_CREG);
						break;
					case GSM_CMD_STAGE_AT_CGATT:
						gsmCmdStage++;
						SIM900_AT_CMD_SEND(AT_STRING_CGATT);
						LOG_SIM900(AT_STRING_CGATT);
						break;
					case GSM_CMD_STAGE_AT_CMEE:
						gsmCmdStage++;
						SIM900_AT_CMD_SEND(AT_STRING_CMEE);
						LOG_SIM900(AT_STRING_CMEE);
						break;
					case GSM_CMD_STAGE_AT_CSTT:
						gsmCmdStage++;
						SIM900_AT_CMD_SEND(AT_STRING_CSTT);
						LOG_SIM900(AT_STRING_CSTT);
						break;
					case GSM_CMD_STAGE_AT_CIICR:
						gsmCmdStage++;
						timeout = 400;
						SIM900_AT_CMD_SEND(AT_STRING_CIICR);
						LOG_SIM900(AT_STRING_CIICR);
						break;
					case GSM_CMD_STAGE_AT_CIFSR:
						gsmCmdStage++;
						SIM900_AT_CMD_SEND(AT_STRING_CIFSR);
						LOG_SIM900(AT_STRING_CIFSR);
						break;
					case GSM_CMD_STAGE_AT_CIPSTART:
						SIM900_AT_CMD_SEND(AT_STRING_CIPSTART);
						LOG_SIM900(AT_STRING_CIPSTART);
						break;
					case GSM_CMD_STAGE_AT_CIPSEND:
						timeout = 1500;
						SIM900_AT_CMD_SEND(AT_STRING_CIPSEND);
						LOG_SIM900(AT_STRING_CIPSEND);
						OSTimeDly(30);
						memcpy(GprsDataSend,pumpBoardInfo.deviceSNStr,TXA_SN_LENTH);
						offset = TXA_SN_LENTH;
						GprsDataSend[offset] = ',';
						offset++;
						if(pumpBoardInfo.permission == PERMISSION_ALLOW) {
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
						SIM900_AT_CMD_SEND(GprsDataSend);
						LOG_SIM900(GprsDataSend);
						OSTimeDly(100);
						SIM900_AT_CMD_SEND(&end);//以0x1a结束
						LOG_SIM900(&end);//以0x1a结束
						break;
					case GSM_CMD_STAGE_REMOTE_RESPOND:
						gsmCmdStage = GSM_CMD_STAGE_AT_CIPSEND;	//如果超时收不到，则转回发送状态
						break;
					case GSM_CMD_STAGE_CLOSE_NET:
						gsmCmdStage = GSM_CMD_STAGE_IDLE;
						SIM900_AT_CMD_SEND(AT_STRING_CIPCLOSE);
						LOG_SIM900(AT_STRING_CIPCLOSE);
						OSTimeDly(100);
						SIM900_AT_CMD_SEND(AT_STRING_CIPSHUT);
						LOG_SIM900(AT_STRING_CIPSHUT);
						break;
					case GSM_CMD_STAGE_IDLE:
						LOG_SIM900("Communicate Success!\r\n");
						if(flag==1) {
							LOG_SIM900("Permission is allow,please restart power!\r\n");
						}
						timeout = 1000;
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

