/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name :APP.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : luzhi
** Created date :2015��11��23��10:07:26
** Version :V1.1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :
** Modified date :
** Version :
** Description :
ʵ�ֹ��ܣ�
1.��������д������WIFI͸����ʹ���ֻ�APP�޸ĵ�ǰ������롣
2.Զ�̼��ʹ����Ȩ��ͨ��sim�����������ͱ�������������Ȩ״̬�������������շ�������������Ȩ��Ϣ
3.����ͨ��PWM������ߴ����ź��������ˮ�õĿ���
4.ˮ�õ�ѹ�ɵ�
************************************************************************************************/
#define GLOBALS

#include "includes.h"

//#pragma diag_suppress 870		  //��ֹ����warning:  #870-D: invalid multibyte character sequence���������Ϣ


static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];

	

void App_TaskCreate(void);
void App_TaskStart(void* p_arg);
void Sys_Data_Init(void);

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
int main(void)
{
	CPU_INT08U os_err;
	CPU_IntDis();				//��ֹCPU�ж�
	OSInit();     				//UCOS ��ʼ��                                             
	BSP_Stage_1_Init();   		//Ӳ��ƽ̨��һ�׶γ�ʼ������Ҫ��CPU�ڲ�ģ���ʼ��������ǰ��Sys_Data_Init()����  
	Sys_Data_Init();			//ȫ�����ݳ�ʼ��,Ҫ������������֮ǰ����ֵ
//	BSP_Stage_2_Init();   		//Ӳ��ƽ̨�ڶ��׶γ�ʼ�������������Sys_Data_Init()���� 
//	CPU_IntEn();	//ʹ��CPU�ж�
	//���������� ���ȼ����  ���������������һ����;��Ϊ���Ժ�ʹ��ͳ������
	os_err = os_err;	//��ֹ����������
	os_err = OSTaskCreate(	(void (*) (void *)) App_TaskStart,	  		  				//ָ����������ָ��
                          	(void *) 0,								  					//����ʼִ��ʱ�����ݸ�����Ĳ�����ָ��
               				(OS_STK *) &App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],	//���������Ķ�ջ��ջ��ָ��   �Ӷ����µݼ�
               				(INT8U) APP_TASK_START_PRIO);								//�������������ȼ�
   	
	//ucos�Ľ��ļ�������0    ���ļ�������0-4294967295    ���ڽ���Ƶ��100hzʱ�� ÿ��497������¼��� 
	OSTimeSet(0);
	OSStart(); 
	return (0);	
}
/************************************************************************************************
** Function name :			
** Description :ϵͳ������������
** ���ݳ�ʼ��,Ҫ������������֮ǰ����ֵ
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void Sys_Data_Init(void)
{
	MCP41xxx_Write_RES(PUMP_VOLTAGE_OUT);
	Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_IDLE;
	Uart2.WriteSnPackageStatus = WRITE_SN_PACKAGE_RECV_IDLE;

	pumpBoardInfo.isPumpRunning = PUMP_STOP;
}
uint64_t my_atoll(u8* str)
{
	uint64_t result=0;
	while(*str) {
		if((*str<'0')&&(*str>'9')) {
			return 0;
		} else {
			result *= 10;
			result += *str-'0';
			str++;
		}
	}
	return result;
}
/************************************************************************************************
** Function name :			
** Description :�ڶ��׶�Ӳ����ʼ������Ҫϵͳ����֧�֣�������ϵͳ��������֮�����С����米��ֵ���ȶ�ȡflash����
**				�õ�֮����������Ӧ��Ӳ��������
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void BSP_Stage_2_Init(void)
{
	PUMP_FRONT_CLOSE;
	PUMP_CENTER_CLOSE;
 	PUMP_REAR_CLOSE;
	//GprsCmd.permission = PERMISSION_ALLOW;		
	//Set_Product_Permission();
	//Set_Product_Number("TXA1509280000097",TXA_SN_LENTH);
	LOG_WRITE_SN("http://www.txauav.com\r\n");
	if(LIQUID_WRITE_SN_READ()) {	//����Ƿ���Ҫд�룬����Ϊд��
		LOG_WRITE_SN("�ȴ�д��\r\n");
		if(Get_Product_Number()==0) {
			LOG_WRITE_SN("��ȡ�������ɹ�\r\n");
			LOG_WRITE_SN(pumpBoardInfo.deviceSNStr);
			LOG_WRITE_SN("\r\n");
		} else {
			LOG_WRITE_SN("��ȡ�������ʧ��\r\n");
			memcpy(pumpBoardInfo.deviceSNStr,"000000000000000000000",TXA_SN_LENTH);
		}
		pumpBoardInfo.Usart2Process = USART2_Write_SN_Process;
		OSTaskCreateExt(Task_Write_SN,
						(void *)0,
						(OS_STK *)&TaskWriteSNStk[TASK_WRITE_SN_SIZE-1],
						TASK_WRITE_SN_PRIO,
						TASK_WRITE_SN_PRIO,
						(OS_STK *)&TaskWriteSNStk[0],
								TASK_WRITE_SN_SIZE,
								(void *)0,
								OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 
		NVIC_Configuration();
		return ;
	}
	pumpBoardInfo.Usart2Process = USART2_Flight_Board_Process;
	LOG_WRITE_SN("��ȡ�����������....\r\n");
	if(Get_Product_Number()==0) {
		LOG_SIM900("��ȡ�������ɹ�\r\n");
		LOG_SIM900(pumpBoardInfo.deviceSNStr);
		LOG_SIM900("\r\n");
		pumpBoardInfo.isSNSave = SN_SAVE_YES;
	} else {
		LOG_SIM900("��ȡ�������ʧ��\r\n");
		memcpy(pumpBoardInfo.deviceSNStr,"000000000000000000000",TXA_SN_LENTH);
		pumpBoardInfo.isSNSave = SN_SAVE_NO;
		pumpBoardInfo.deviceSN = 0;
	}
		//memcpy(kkk,&pumpBoardInfo.deviceSNStr[3],TXA_SN_LENTH-3);
		//memcpy(pumpBoardInfo.deviceSNStr+3,"2601010550871",TXA_SN_LENTH-3);
		
		//pumpBoardInfo.deviceSN = atoll("1601010555501");
		pumpBoardInfo.deviceSN = atoll((const char*)pumpBoardInfo.deviceSNStr+3);

	//pumpBoardInfo.deviceSN = 1509280000097;
	Get_Product_Permission();
	memcpy(GprsCmd.BaseStationLongitude,"106.123456",GPRS_BASE_STATION_LONGITUDE_LENTH);
	memcpy(GprsCmd.BaseStationLatitude,"21.123456",GPRS_BASE_STATION_LATITUDE_LENTH);
	
	if(pumpBoardInfo.permission == PERMISSION_ALLOW) {	
		LOG_SIM900("��Ȩ����\r\n");
	} else {
		LOG_SIM900("��ֹ��Ȩ\r\n");
	}
	App_TaskCreate();
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); //����DMAͨ��
	TIM_Cmd(TIM2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 
	NVIC_Configuration();
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
void App_TaskStart(void* p_arg)
{	
	(void) p_arg;
	OS_CPU_SysTickInit();			//��ʼ��ucosʱ�ӽ��ģ�����100Hzϵͳ����
	//ʹ��ucos ��ͳ������
	#if (OS_TASK_STAT_EN > 0)
	//----ͳ�������ʼ������  
	OSStatInit();                    /* Determine CPU capacity.                              */
	#endif
	BSP_Stage_2_Init();   			//Ӳ��ƽ̨�ڶ��׶γ�ʼ��
	OSTimeDly(100);
	pumpBoardInfo.PumpCurrentRef = Get_Pump_Current_Ref();
	while (1) {
		OSTimeDly(10);
		LED_NIGHT_FLIGHT_ON;
		OSTimeDly(10);
		LED_NIGHT_FLIGHT_OFF;
		OSTimeDly(10);
		LED_NIGHT_FLIGHT_ON;
		OSTimeDly(100);
		LED_NIGHT_FLIGHT_OFF;
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
void App_TaskCreate(void)
{
	OSTaskCreateExt(Task_Dji_SDK_Codec,
   					(void *)0,
   					(OS_STK *)&TaskCodecStk[TASK_CODEC_STK_SIZE-1],
   					TASK_CODEC_PRIO,
   					TASK_CODEC_PRIO,
   					(OS_STK *)&TaskCodecStk[0],
		                    TASK_CODEC_STK_SIZE,
		                    (void *)0,
		                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR); 
	OSTaskCreateExt(Task_PWM_Ctrl_Pump,
   					(void *)0,
   					(OS_STK *)&TaskPwmCtrlPumpStk[TASK_PWM_CTRL_PUMP_STK_SIZE-1],
   					TASK_PWM_CTRL_PUMP_PRIO,
   					TASK_PWM_CTRL_PUMP_PRIO,
   					(OS_STK *)&TaskPwmCtrlPumpStk[0],
		                    TASK_PWM_CTRL_PUMP_STK_SIZE,
		                    (void *)0,
		                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR); 
	OSTaskCreateExt(Task_Usart_DJI_Ctrl_Pump,
   					(void *)0,
   					(OS_STK *)&TaskUsartDjiCtrlPumpStk[TASK_USART_DJI_CTRL_PUMP_STK_SIZE-1],
   					TASK_USART_DJI_CTRL_PUMP_PRIO,
   					TASK_USART_DJI_CTRL_PUMP_PRIO,
   					(OS_STK *)&TaskUsartDjiCtrlPumpStk[0],
		                    TASK_USART_DJI_CTRL_PUMP_STK_SIZE,
		                    (void *)0,
		                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(Task_Send_Pump_Board_Info,
   					(void *)0,
   					(OS_STK *)&TaskSendPumpBoardInfoStk[TASK_SEND_PUMP_BOARD_INFO_STK_SIZE-1],
   					TASK_SEND_PUMP_BOARD_INFO_PRIO,
   					TASK_SEND_PUMP_BOARD_INFO_PRIO,
   					(OS_STK *)&TaskSendPumpBoardInfoStk[0],
		                    TASK_SEND_PUMP_BOARD_INFO_STK_SIZE,
		                    (void *)0,
		                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR); 
	#if 1	                    
	OSTaskCreateExt(Task_Gprs_Proc,
   					(void *)0,
   					(OS_STK *)&TaskGprsProcStk[TASK_GPRS_PROC_STK_SIZE-1],
   					TASK_GPRS_PROC_PRIO,
   					TASK_GPRS_PROC_PRIO,
   					(OS_STK *)&TaskGprsProcStk[0],
		                    TASK_GPRS_PROC_STK_SIZE,
		                    (void *)0,
		                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
			                    
   	OSTaskCreateExt(Task_Write_SN,
   					(void *)0,
   					(OS_STK *)&TaskWriteSNStk[TASK_WRITE_SN_SIZE-1],
   					TASK_WRITE_SN_PRIO,
   					TASK_WRITE_SN_PRIO,
   					(OS_STK *)&TaskWriteSNStk[0],
		                    TASK_WRITE_SN_SIZE,
		                    (void *)0,
		                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
	#endif
}
/*
*********************************************************************************************************
*********************************************************************************************************
*                                          uC/OS-II APP HOOKS
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_APP_HOOKS_EN > 0)
/*
*********************************************************************************************************
*                                      TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument : ptcb   is a pointer to the task control block of the task being created.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskCreateHook(OS_TCB* ptcb)
{
}

/*
*********************************************************************************************************
*                                    TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskDelHook(OS_TCB* ptcb)
{
   (void) ptcb;
}

/*
*********************************************************************************************************
*                                      IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument : none.
*
* Note     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void App_TaskIdleHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                        STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument : none.
*********************************************************************************************************
*/

void App_TaskStatHook(void)
{
}

/*
*********************************************************************************************************
*                                        TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument : none.
*
* Note     : 1 Interrupts are disabled during this call.
*
*            2  It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void App_TaskSwHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                     OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument : ptcb    is a pointer to the TCB of the task being created.
*
* Note     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void App_TCBInitHook(OS_TCB* ptcb)
{
   (void) ptcb;
}
#endif

#endif
