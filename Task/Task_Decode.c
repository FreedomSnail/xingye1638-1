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
** Created by : ½־
** Created date :2015��12��21��11:52:20
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
OS_EVENT 	*UsartDjiCtrlPumpQsem; 
void     	*UsartDJICtrlQMsgTbl[USART_DJI_CTRL_RESOURCES];



OS_STK 		TaskCodecStk[TASK_PWM_CTRL_PUMP_STK_SIZE];
OS_EVENT  	*SemDjiCodec;

OS_EVENT  	*SemPWM;

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
			Pro_Receive_Interface();
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
void Task_PWM_Ctrl_Pump(void* p_arg)
{
#if 1
	INT8U err;
	p_arg = p_arg;
	SemPWM = OSSemCreate(0);
	if((GprsCmd.PermissionLocal == PERMISSION_PROHIBIT)||(GprsCmd.isSNSave == SN_SAVE_NO)) {
		while(1) {
			OSTimeDly(500);
			LOG_DJI("\r\nPWM ctrl disable\r\n");
		}
	}
	while(1) {
		OSSemPend(SemPWM,10,&err); 
		if(OS_ERR_NONE == err) {
			if(Device.PWMPeriod<PWM_HIGHT_LEVEL_WIDTH_DOWN) {
				Pump_Voltage_Set(0);
			} else if(Device.PWMPeriod>PWM_HIGHT_LEVEL_WIDTH_UP){
				Pump_Voltage_Set(PUMP_VOLTAGE_OUT);
			}
		} else {	// lose pwm signal
			Pump_Voltage_Set(0);
		}
	}
#else
	INT8U err;
	u8* msg;
	u8 cnt;
	u16 NoLiquidHighLevelCnt,NoLiquidLowLevelCnt;
	u16 PumpHighCurrentCnt,PumpLowCurrentCnt;
	u8  NoLiquidFlag = 0;
	u8  PumpLowCurrentFlag = 0;
	
	msg = msg;
	UsartDjiCtrlPumpQsem = OSQCreate(&UsartDJICtrlQMsgTbl[0], USART_DJI_CTRL_RESOURCES);
	
	while (1) {
		OSTimeDly(5);
		if( Device.isA2 == PWM_RAISING_CNT) {	// A2�汾��ˮ��ֻ��PWM�źſ��ƣ�PWM�������1500usʱ����ˮ�ã�����ر�
			LOG_DJI("Version is A2!\r\n");
			while(1) {
				OSTimeDly(10);
				LOG_DJI(".");
				if(Device.PWMSignalCnt == 0) { 	//��ʧPWM�ź�
					Pump_Voltage_Set(0);
				} else {
					if(Device.PWMPeriod<PWM_HIGHT_LEVEL_WIDTH_DOWN) {
						Pump_Voltage_Set(0);
					} else if(Device.PWMPeriod>PWM_HIGHT_LEVEL_WIDTH_UP){
						Pump_Voltage_Set(PUMP_VOLTAGE_OUT);	//  PUMP_VOLTAGE_12V
					}
				}
				Adc_Filter();
				Device.V6s = Get_6S_Val();
				Device.PumpCurrent = Get_Pump_Current();
				if(LIQUID_OUTOF_IN_READ()) {			//��⵽�Ǹߵ�ƽ
					if(NoLiquidLowLevelCnt>0) {
						NoLiquidLowLevelCnt = 0;
					}
					NoLiquidHighLevelCnt++;
					if(NoLiquidHighLevelCnt==3) {	//ȷ���Ǹߵ�ƽ���壬�����һ����Ϣ
						NoLiquidFlag = 0;
					}
				} else {		//��⵽�͵�ƽ
					if(NoLiquidHighLevelCnt>0) {
						NoLiquidHighLevelCnt = 0;
					}
					NoLiquidLowLevelCnt++;
					if(NoLiquidLowLevelCnt==3) {	//ȷ���ǵ͵�ƽ���壬�����һ����Ϣ
						NoLiquidFlag = 1;
					}
				}
				if(Device.PumpCurrent > PUMP_CURRENT_CRITICAL_POINT) {			//ˮ�õ�����
					if(PumpLowCurrentCnt>0) {
						PumpLowCurrentCnt = 0;
					}
					PumpHighCurrentCnt++;
					if(PumpHighCurrentCnt==3) {	//ȷ����ˮ�ô��ڸߵ�������(��Ϊ��ũҩ�������������)�������һ����Ϣ
						PumpLowCurrentFlag = 0;
					}
				} else {		//��⵽�͵���
					if(PumpHighCurrentCnt>0) {
						PumpHighCurrentCnt = 0;
					}
					PumpLowCurrentCnt++;
					if(PumpLowCurrentCnt==3) {	//ȷ����ˮ�ô��ڵ͵������У������һ����Ϣ
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
			
			Pro_Receive_Interface();//һ֡���ݽ������
			if((DataFromMobile.CommandSet == 0x02)&&(DataFromMobile.CommandId == 0x02)) {	//�ƶ��豸ÿ��500ms��һ����
				LOG_DJI("received mobile data!\r\n");
				Device.LoseRemoteSignalCnt = 200;
				switch(DataFromMobile.data[0]) {
					case '1':	//��ˮ��
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
		Device.V6s = Get_6S_Val();
		Device.V12s = Device.V6s ;
		Device.PumpCurrent = Get_Pump_Current();
		Send_Msg_2_M100();
	}
	#endif
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
	if((GprsCmd.PermissionLocal == PERMISSION_PROHIBIT)||(GprsCmd.isSNSave == SN_SAVE_NO)) {
		while(1) {
			OSTimeDly(500);
			LOG_DJI("\r\nUsart ctrl disable\r\n");
		}
	}
	while(1) {
		msg = OSQPend(UsartDjiCtrlPumpQsem,100,&err); 
		
		if(OS_ERR_NONE == err) {
			if('1'==*msg) {
				Pump_Voltage_Set(PUMP_VOLTAGE_OUT);
			} else if('0'==*msg) {
				Pump_Voltage_Set(0);
			}
		} else {	// lose usart signal
			Pump_Voltage_Set(0);
		}
	}
	
}

