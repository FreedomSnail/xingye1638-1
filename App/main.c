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
	MCP41xxx_Write_RES(PUM_VOLTAGE_OUT);
	Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_IDLE;
	Uart2.WriteSnPackageStatus = WRITE_SN_PACKAGE_RECV_IDLE;
	GsmCmd.GsmCmdStage = GSM_CMD_STAGE_AT_COPS;
	
	
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
	//GprsCmd.PermissionLocal = PERMISSION_ALLOW;		
	//Set_Product_Permission();
	//Set_Product_Number("TXA1509280000097",TXA_SN_LENTH);
	LOG_WRITE_SN("http://www.txauav.com\r\n");
	if(LIQUID_WRITE_SN_READ()) {	//����Ƿ���Ҫд�룬����Ϊд��
		LOG_WRITE_SN("�ȴ�д��\r\n");
		if(Get_Product_Number()==0) {
			LOG_WRITE_SN("��ȡ�������ɹ�\r\n");
			LOG_WRITE_SN(GprsCmd.SNLocal);
			LOG_WRITE_SN("\r\n");
		} else {
			LOG_WRITE_SN("��ȡ�������ʧ��\r\n");
			memcpy(GprsCmd.SNLocal,"000000000000000000000",TXA_SN_LENTH);
		}
		Device.Usart2Process = USART2_Write_SN_Process;
		OSTaskCreateExt(Task_Write_SN,
						(void *)0,
						(OS_STK *)&TaskWriteSNStk[TASK_WRITE_SN_SIZE-1],
						TASK_WRITE_SN_PRIO,
						TASK_WRITE_SN_PRIO,
						(OS_STK *)&TaskWriteSNStk[0],
								TASK_WRITE_SN_SIZE,
								(void *)0,
								OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
		
		return ;
	}
	Device.Usart2Process = USART2_DJI_Process;
	LOG_WRITE_SN("��ȡ�����������....\r\n");
	if(Get_Product_Number()==0) {
		LOG_SIM900("��ȡ�������ɹ�\r\n");
		LOG_SIM900(GprsCmd.SNLocal);
		LOG_SIM900("\r\n");
	} else {
		LOG_SIM900("��ȡ�������ʧ��\r\n");
		memcpy(GprsCmd.SNLocal,"000000000000000000000",TXA_SN_LENTH);
		return ;
	}
	Get_Product_Permission();
	memcpy(GprsCmd.BaseStationLongitude,"106.123456",GPRS_BASE_STATION_LONGITUDE_LENTH);
	memcpy(GprsCmd.BaseStationLatitude,"21.123456",GPRS_BASE_STATION_LATITUDE_LENTH);
	//���ӷ�������������------------------------------------------------------
	OSTaskCreateExt(Task_Gprs_Proc,
   					(void *)0,
   					(OS_STK *)&TaskGprsProcStk[TASK_GPRS_PROC_STK_SIZE-1],
   					TASK_GPRS_PROC_PRIO,
   					TASK_GPRS_PROC_PRIO,
   					(OS_STK *)&TaskGprsProcStk[0],
		                    TASK_GPRS_PROC_STK_SIZE,
		                    (void *)0,
		                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
	
	if(GprsCmd.PermissionLocal == PERMISSION_ALLOW) {	
		LOG_SIM900("��Ȩ����\r\n");
		//���봦������------------------------------------------------------
	   	OSTaskCreateExt(Task_Decode,
	   					(void *)0,
	   					(OS_STK *)&TaskDecodeStk[TASK_DECODE_STK_SIZE-1],
	   					TASK_DECODE_PRIO,
	   					TASK_DECODE_PRIO,
	   					(OS_STK *)&TaskDecodeStk[0],
			                    TASK_DECODE_STK_SIZE,
			                    (void *)0,
			                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR); 

		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		DMA_Cmd(DMA1_Channel1, ENABLE); //����DMAͨ��
		TIM_Cmd(TIM2, ENABLE);
	} else {
		LOG_SIM900("��ֹ��Ȩ\r\n");
		USART_ITConfig(SERIAL_PORT_DJI_SDK, USART_IT_RXNE, DISABLE);  //��ֹ�����ж�
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
** �������е����
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
	//App_TaskCreate();				//��������������
	BSP_Stage_2_Init();   			//Ӳ��ƽ̨�ڶ��׶γ�ʼ��
	OSTimeDly(100);
	Device.PumpCurrentRef = Get_Pump_Current_Ref();
	while (1) {
		OSTimeDly(1000);
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
	#if 0
	//���ӷ�������������------------------------------------------------------
   	OSTaskCreateExt(Task_Gprs_Proc,
   					(void *)0,
   					(OS_STK *)&TaskGprsProcStk[TASK_GPRS_PROC_STK_SIZE-1],
   					TASK_GPRS_PROC_PRIO,
   					TASK_GPRS_PROC_PRIO,
   					(OS_STK *)&TaskGprsProcStk[0],
		                    TASK_GPRS_PROC_STK_SIZE,
		                    (void *)0,
		                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
	
	//����봦������------------------------------------------------------
   	OSTaskCreateExt(Task_Decode,
   					(void *)0,
   					(OS_STK *)&TaskDecodeStk[TASK_DECODE_STK_SIZE-1],
   					TASK_DECODE_PRIO,
   					TASK_DECODE_PRIO,
   					(OS_STK *)&TaskDecodeStk[0],
		                    TASK_DECODE_STK_SIZE,
		                    (void *)0,
		                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR); 
	
	//���洦������
    OSTaskCreateExt(Task_GUI,
   					(void *)0,
   					(OS_STK *)&TaskGuiStk[TASK_GUI_STK_SIZE-1],
   					TASK_GUI_PRIO,
   					TASK_GUI_PRIO,
   					(OS_STK *)&TaskGuiStk[0],
		                    TASK_GUI_STK_SIZE,
		                    (void *)0,
		                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
	
	//ʵʱʱ�䴦������------------------------------------------------------
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
