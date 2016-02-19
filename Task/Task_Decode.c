/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name : Task_Decode.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : 陆志
** Created date :2015年12月21日11:52:20
** Version :V1.1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :
** Modified date :
** Version :
** Description :
************************************************************************************************/

#include "includes.h"


OS_STK 		TaskPwmCtrlPumpStk[TASK_PWM_CTRL_PUMP_STK_SIZE];


OS_STK 		TaskUsartDjiCtrlPumpStk[TASK_PWM_CTRL_PUMP_STK_SIZE];
OS_EVENT 	*UsartDjiCtrlPumpQsem; 
void     	*UsartDJICtrlQMsgTbl[USART_DJI_CTRL_RESOURCES];



OS_STK 		TaskCodecStk[TASK_PWM_CTRL_PUMP_STK_SIZE];
OS_EVENT  	*SemDjiCodec;

OS_EVENT  	*SemPWM;


OS_STK 		TaskSendPumpBoardInfoStk[TASK_SEND_PUMP_BOARD_INFO_STK_SIZE];
//OS_EVENT 	*SemSendPumpBoardInfo;

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
void Task_Dji_SDK_Codec(void *p_arg)
{
	INT8U err;
	p_arg = p_arg;
	SemDjiCodec = OSSemCreate(0);
	while(1) {
		OSSemPend(SemDjiCodec,0,&err); 
		if(OS_ERR_NONE == err) {
			//LOG_DJI(".");
			Pro_Receive_Flight_Ctrl_Board();
		}
	}
}
void Pump_Delay_Check_Set(void)
{
	if(pumpBoardInfo.PumpDelayCheckCurrentFlag != 1) {
 		pumpBoardInfo.PumpDelayCheckCurrentFlag = 1;
		pumpBoardInfo.PumpDelayCheckCurrent = DELAY_CHECK_PUMP_CURRENT_CNT;
		LOG_DJI("\r\npump open!\r\n");
 	}
}
void Pump_Delay_Check_Reset(void)
{
	pumpBoardInfo.PumpDelayCheckCurrentFlag = 0;
	pumpBoardInfo.PumpDelayCheckCurrent = 0;
}

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
void Task_PWM_Ctrl_Pump(void* p_arg)
{
	INT8U err;
	u8 pwmCnt=0;
	u8 pwmOffCnt = 0;
	u8 pwmOnCnt = 0;
	p_arg = p_arg;
	SemPWM = OSSemCreate(0);
	if((pumpBoardInfo.permission == PERMISSION_PROHIBIT)||(pumpBoardInfo.isSNSave == SN_SAVE_NO)) {
		while(1) {
			OSTimeDly(500);
			LOG_DJI("\r\nPWM ctrl disable\r\n");
		}
	}
	while(1) {
		OSSemPend(SemPWM,10,&err); 
		if(pumpBoardInfo.remoteOnlineFlag == OFFLINE) {
			if(OS_ERR_NONE == err) {
				if( pwmCnt<10 ) {
					pwmCnt++;
					if(pumpBoardInfo.PWMPeriod<PWM_HIGHT_LEVEL_WIDTH_DOWN) {
						pwmOffCnt++;
					} else if(pumpBoardInfo.PWMPeriod>PWM_HIGHT_LEVEL_WIDTH_UP){
						pwmOnCnt ++;
					}
				} else {
					pwmCnt = 0;
					if(pwmOffCnt>7) {
						Pump_Voltage_Set(0);
					} else if(pwmOnCnt>7) {
						Pump_Voltage_Set(PUMP_VOLTAGE_OUT);
					}
					pwmOffCnt = pwmOnCnt = 0;
				}
				pumpBoardInfo.pwmOnlineFlag = ONLINE;
			} else {	// lose pwm signal
				pumpBoardInfo.pwmOnlineFlag = OFFLINE;
				Pump_Voltage_Set(0);
			}
		}
	}
}
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
void Task_Usart_DJI_Ctrl_Pump(void* p_arg)
{
	INT8U err;
	u8* msg;
	p_arg = p_arg;
	UsartDjiCtrlPumpQsem = OSQCreate(&UsartDJICtrlQMsgTbl[0], USART_DJI_CTRL_RESOURCES);
	if((pumpBoardInfo.permission == PERMISSION_PROHIBIT)||(pumpBoardInfo.isSNSave == SN_SAVE_NO)) {
		while(1) {
			OSTimeDly(500);
			LOG_DJI("\r\nUsart ctrl disable\r\n");
		}
	}
	while(1) {
		msg = (u8 *)OSQPend(UsartDjiCtrlPumpQsem,50,&err); 
		if(OS_ERR_NONE == err) {
			//USART_Send_Buf(SERIAL_PORT_DEBUG,msg,5);
			//temp = Device.targetPumpVoltage*10;
			//USART_Send_Buf(SERIAL_PORT_DEBUG,"v=%d\r\n",Device.targetPumpVoltage*10);
			//USART_OUT(SERIAL_PORT_DEBUG,"v=%d\r\n",temp);
 			if(1==*msg) {
 				Pump_Delay_Check_Set();
				Pump_Voltage_Set(PUMP_VOLTAGE_OUT);
				pumpBoardInfo.remoteOnlineFlag = ONLINE;
			} else if(0==*msg) {
				Pump_Delay_Check_Reset();
				Pump_Voltage_Set(0);
				pumpBoardInfo.remoteOnlineFlag = ONLINE;
			}
			memcpy((u8*)&pumpBoardInfo.targetPumpVoltage,msg+1,4);
		} else {	// lose usart signal
			Pump_Delay_Check_Reset();
			pumpBoardInfo.remoteOnlineFlag = OFFLINE;
			if(pumpBoardInfo.pwmOnlineFlag == OFFLINE) {
				Pump_Voltage_Set(0);
			}
		}
	}
	
}
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
void Task_Send_Pump_Board_Info(void* p_arg)
{
	//INT8U err;
	u8 cmd[DATA_LENGTH_RECEIVE_PUMP_CONTROL_BOARD];
	u16 PumpHighCurrentCnt,PumpLowCurrentCnt;
	u8  PumpLowCurrentFlag = 0;
	(void) p_arg;		//防止编译器警告
	
	cmd[0] = 0xAA;
	cmd[1] = DATA_LENGTH_RECEIVE_PUMP_CONTROL_BOARD;	//27个字节长度
	cmd[2] = 0x02;
	cmd[3] = 0x02;
	#if 1
	while(1) {
		OSTimeDly(10);
		Adc_Filter();
		pumpBoardInfo.supplyVoltage = Get_Supply_Voltage();
		pumpBoardInfo.PumpVoltage = Get_Pump_Voltage();
		pumpBoardInfo.PumpCurrent = Get_Pump_Current();
		if(pumpBoardInfo.PumpDelayCheckCurrent > 0) {	//延迟
			pumpBoardInfo.PumpDelayCheckCurrent--;
			if(pumpBoardInfo.PumpDelayCheckCurrent > 0) {
				LOG_DJI("\r\npump check delay...\r\n");
			} else {
				LOG_DJI("\r\npump check delay done!\r\n");
			}
			PumpLowCurrentCnt = 0;
			PumpHighCurrentCnt = 0;
			PumpLowCurrentFlag = 0;
		} else {
				if(pumpBoardInfo.PumpCurrent > PUMP_CURRENT_CRITICAL_POINT) {			//水泵电流高
					if(PumpLowCurrentCnt>0) {
						PumpLowCurrentCnt = 0;
					}
					PumpHighCurrentCnt++;
					if(PumpHighCurrentCnt>SAME_CURRENT_STATUS_CNT) {	//确认是水泵处于高电流运行(因为有农药所以相对阻力大)，则产生一次消息
						PumpLowCurrentFlag = 0;
					}
				} else {		//检测到低电流
					if(PumpHighCurrentCnt>0) {
						PumpHighCurrentCnt = 0;
					}
					PumpLowCurrentCnt++;
					if(PumpLowCurrentCnt>SAME_CURRENT_STATUS_CNT) {	//确认是水泵处于低电流运行，则产生一次消息
						PumpLowCurrentFlag = 1;
					}
				}
			
				if( pumpBoardInfo.isPumpRunning == PUMP_RUNNING) {
					if(PumpLowCurrentFlag == 1) {
						pumpBoardInfo.isDoseRunOut = DOSE_EMPTY;
					} else {
						pumpBoardInfo.isDoseRunOut = DOSE_FULL;
					}
				} else {
					pumpBoardInfo.isDoseRunOut = DOSE_FULL;
				}
		}
		cmd[4] = pumpBoardInfo.isPumpRunning;		//水泵工作状态
		memcpy(cmd+5,(u8 *)&pumpBoardInfo.PumpVoltage,4);	//水泵电压
		memcpy(cmd+9,(u8 *)&pumpBoardInfo.supplyVoltage,4);//供电电压电压
		cmd[13] = pumpBoardInfo.isDoseRunOut;	//农药剩余量信息，还有农药
		memcpy(&cmd[14],(unsigned char*)&pumpBoardInfo.deviceSN,8);		//机身编号 
		cmd[22] = pumpBoardInfo.permission;			//授权码
		send_cmd_to_flight_ctrl_board(cmd, DATA_LENGTH_RECEIVE_PUMP_CONTROL_BOARD);
	}
	#endif
}
