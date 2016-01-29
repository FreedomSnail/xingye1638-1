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


OS_STK 		TaskDecodeStk[TASK_DECODE_STK_SIZE];
OS_EVENT 	*DecodeQSem; 
void     	*DecodeQMsgTbl[DECODE_RESOURCES];
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
void Task_Decode(void* p_arg)
{
	INT8U err;
	u8* msg;
	u8 cnt;
	u16 NoLiquidHighLevelCnt,NoLiquidLowLevelCnt;
	u16 PumpHighCurrentCnt,PumpLowCurrentCnt;
	u8  NoLiquidFlag = 0;
	u8  PumpLowCurrentFlag = 0;
	
	msg = msg;
	DecodeQSem = OSQCreate(&DecodeQMsgTbl[0], DECODE_RESOURCES);
	
	DJI_Onboard__API_Activation_Init();
	DJI_Onboard_API_Activation();
	while (1) {
		msg = (u8 *)OSQPend(DecodeQSem,  200, &err);
		if(*msg == 1) {
			Pro_Receive_Interface();//һ֡���ݽ������
			if((DataFromMobile.CommandSet == 0x00)&&(DataFromMobile.CommandId == 0x00)) {
				LOG_SIM900("Activate ok!\r\n");//����ɹ�
				break;
			} else {
				LOG_SIM900("Activate failed!\r\n");//����ʧ��
			}
		}
		DJI_Onboard_API_Activation();
		if( Device.isA2 == PWM_RAISING_CNT) {	// A2�汾��ˮ��ֻ��PWM�źſ��ƣ�PWM�������1500usʱ����ˮ�ã�����ر�
			LOG_SIM900("Version is A2!\r\n");
			while(1) {
				OSTimeDly(1);
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
				#if 0
				cnt++;
				if(cnt<100) {
					LED_OPERATION_ON;
				} else if(cnt<200) {
					LED_OPERATION_OFF;
				} else {
					cnt = 0;
				}
				#endif
				Log_test();
			}
		}
	}
	while (1) {
		(u8 *)OSQPend(DecodeQSem,  10, &err);
		if(err == OS_ERR_NONE) {
			Pro_Receive_Interface();//һ֡���ݽ������
			if((DataFromMobile.CommandSet == 0x02)&&(DataFromMobile.CommandId == 0x02)) {	//�ƶ��豸ÿ��500ms��һ����
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
}

