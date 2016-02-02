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
				if(pumpBoardInfo.PWMPeriod<PWM_HIGHT_LEVEL_WIDTH_DOWN) {
					Pump_Delay_Check_Reset();
					Pump_Voltage_Set(0);
				} else if(pumpBoardInfo.PWMPeriod>PWM_HIGHT_LEVEL_WIDTH_UP){
					Pump_Delay_Check_Set();
					Pump_Voltage_Set(PUMP_VOLTAGE_OUT);
				}
				pumpBoardInfo.pwmOnlineFlag = ONLINE;
			} else {	// lose pwm signal
				Pump_Delay_Check_Reset();
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
	INT8U err;
	u8 cmd[DATA_LENGTH_RECEIVE_PUMP_CONTROL_BOARD];
	u16 PumpHighCurrentCnt,PumpLowCurrentCnt;
	u8  PumpLowCurrentFlag = 0;
	(void) p_arg;		//防止编译器警告
	

	cmd[0] = 0xAA;
	cmd[1] = DATA_LENGTH_RECEIVE_PUMP_CONTROL_BOARD;	//27个字节长度
	cmd[2] = 0x02;
	cmd[3] = 0x02;
	#if 1
	//SemSendPumpBoardInfo = OSSemCreate(0);
	while(1) {
		OSTimeDly(10);
		//OSSemPend(SemSendPumpBoardInfo,10,&err); 
		//if()
		
		Adc_Filter();
		pumpBoardInfo.supplyVoltage = Get_Supply_Voltage();
		pumpBoardInfo.PumpVoltage = Get_Pump_Voltage();
		pumpBoardInfo.PumpCurrent = Get_Pump_Current();
		if(pumpBoardInfo.PumpDelayCheckCurrent > 0) {	//延迟
			pumpBoardInfo.PumpDelayCheckCurrent--;
			LOG_DJI("\r\npump check delay!\r\n");
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
	
	#if 0
	while(1) {
		OSTimeDly(10);
		cmd[4] = 1;		//水泵工作状态
		temp = 26.5;			//水泵电压
		memcpy(cmd+5,(u8 *)&temp,4);
		temp = 30.6;		//供电电压电压
		memcpy(cmd+9,(u8 *)&temp,4);
		cmd[13] = 1;	//农药剩余量信息，还有农药
		memcpy(&cmd[14],(unsigned char*)&sn,8);		//机身编号 
		cmd[22] = 'Y';			//授权码
		send_cmd_to_flight_ctrl_board(cmd, DATA_LENGTH_RECEIVE_PUMP_CONTROL_BOARD);
	}
	#endif
}

#if 0
	INT8U err;
	u8* msg;
	u8 cnt;
	u16 NoLiquidHighLevelCnt,NoLiquidLowLevelCnt;
	
	u8  NoLiquidFlag = 0;
	u8  PumpLowCurrentFlag = 0;
	
	msg = msg;
	UsartDjiCtrlPumpQsem = OSQCreate(&UsartDJICtrlQMsgTbl[0], USART_DJI_CTRL_RESOURCES);
	
	while (1) {
		OSTimeDly(5);
		if( Device.isA2 == PWM_RAISING_CNT) {	// A2版本，水泵只受PWM信号控制，PWM脉宽大于1500us时开启水泵，否则关闭
			LOG_DJI("Version is A2!\r\n");
			while(1) {
				OSTimeDly(10);
				LOG_DJI(".");
				if(Device.PWMSignalCnt == 0) { 	//丢失PWM信号
					Pump_Voltage_Set(0);
				} else {
					if(Device.PWMPeriod<PWM_HIGHT_LEVEL_WIDTH_DOWN) {
						Pump_Voltage_Set(0);
					} else if(Device.PWMPeriod>PWM_HIGHT_LEVEL_WIDTH_UP){
						Pump_Voltage_Set(PUMP_VOLTAGE_OUT);	//  PUMP_VOLTAGE_12V
					}
				}
				Adc_Filter();
				Device.V6s = Get_Supply_Voltage();
				Device.PumpCurrent = Get_Pump_Current();
				if(LIQUID_OUTOF_IN_READ()) {			//检测到是高电平
					if(NoLiquidLowLevelCnt>0) {
						NoLiquidLowLevelCnt = 0;
					}
					NoLiquidHighLevelCnt++;
					if(NoLiquidHighLevelCnt==3) {	//确认是高电平脉冲，则产生一次消息
						NoLiquidFlag = 0;
					}
				} else {		//检测到低电平
					if(NoLiquidHighLevelCnt>0) {
						NoLiquidHighLevelCnt = 0;
					}
					NoLiquidLowLevelCnt++;
					if(NoLiquidLowLevelCnt==3) {	//确认是低电平脉冲，则产生一次消息
						NoLiquidFlag = 1;
					}
				}
				if(Device.PumpCurrent > PUMP_CURRENT_CRITICAL_POINT) {			//水泵电流高
					if(PumpLowCurrentCnt>0) {
						PumpLowCurrentCnt = 0;
					}
					PumpHighCurrentCnt++;
					if(PumpHighCurrentCnt==3) {	//确认是水泵处于高电流运行(因为有农药所以相对阻力大)，则产生一次消息
						PumpLowCurrentFlag = 0;
					}
				} else {		//检测到低电流
					if(PumpHighCurrentCnt>0) {
						PumpHighCurrentCnt = 0;
					}
					PumpLowCurrentCnt++;
					if(PumpLowCurrentCnt==3) {	//确认是水泵处于低电流运行，则产生一次消息
						PumpLowCurrentFlag = 1;
					}
				}
				if( Device.LiquidSpeed>0 ) {
					//if((NoLiquidFlag == 1)&&(PumpLowCurrentFlag == 1)) {
					if(PumpLowCurrentFlag == 1) {
						Device.isDoseRunOut = 1;
					} else {
						Device.isDoseRunOut = 0;
					}
				} else {
					Device.isDoseRunOut = 0;
				}
			}
		}
	}
	while (1) {
		(u8 *)OSQPend(UsartDjiCtrlPumpQsem,  10, &err);
		if(err == OS_ERR_NONE) {
			
			Pro_Receive_Interface();//一帧数据接收完成
			if((DataFromMobile.CommandSet == 0x02)&&(DataFromMobile.CommandId == 0x02)) {	//移动设备每隔500ms发一个包
				LOG_DJI("received mobile data!\r\n");
				Device.LoseRemoteSignalCnt = 200;
				switch(DataFromMobile.data[0]) {
					case '1':	//打开水泵
						Pump_Voltage_Set(PUMP_VOLTAGE_OUT);
						break;
					case '0':
						Pump_Voltage_Set(0);
						break;
					default:
						break;
				}
			}
		}
		#if 1
		cnt++;
		if(cnt<10) {
			LED_OPERATION_ON;
		} else if(cnt<20) {
			LED_OPERATION_OFF;
		} else {
			cnt = 0;
		}
		#endif
		if( Device.LoseRemoteSignalCnt == 0) {
			Pump_Voltage_Set(0);
		}
		Device.V6s = Get_Supply_Voltage();
		Device.V12s = Device.V6s ;
		Device.PumpCurrent = Get_Pump_Current();
		Send_Msg_2_M100();
	}
	#endif

