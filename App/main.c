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
** Created date :2015年11月23日10:07:26
** Version :V1.1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :
** Modified date :
** Version :
** Description :
实现功能：
1.机身号码擦写，依靠WIFI透传，使用手机APP修改当前机身号码。
2.远程监控使用授权，通过sim卡，主动发送本机机身编号与授权状态到服务器，接收服务器反馈的授权信息
3.可以通过PWM输入或者串口信号输入控制水泵的开关
4.水泵电压可调
************************************************************************************************/
#define GLOBALS

#include "includes.h"

//#pragma diag_suppress 870		  //禁止出现warning:  #870-D: invalid multibyte character sequence这个警告消息


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
	CPU_IntDis();				//禁止CPU中断
	OSInit();     				//UCOS 初始化                                             
	BSP_Stage_1_Init();   		//硬件平台第一阶段初始化，主要是CPU内部模块初始化，必须前于Sys_Data_Init()运行  
	Sys_Data_Init();			//全部数据初始化,要先于任务运行之前赋初值
//	BSP_Stage_2_Init();   		//硬件平台第二阶段初始化，，必须后于Sys_Data_Init()运行 
//	CPU_IntEn();	//使能CPU中断
	//建立主任务， 优先级最高  建立这个任务另外一个用途是为了以后使用统计任务
	os_err = os_err;	//防止编译器报错
	os_err = OSTaskCreate(	(void (*) (void *)) App_TaskStart,	  		  				//指向任务代码的指针
                          	(void *) 0,								  					//任务开始执行时，传递给任务的参数的指针
               				(OS_STK *) &App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],	//分配给任务的堆栈的栈顶指针   从顶向下递减
               				(INT8U) APP_TASK_START_PRIO);								//分配给任务的优先级
   	
	//ucos的节拍计数器清0    节拍计数器是0-4294967295    对于节拍频率100hz时， 每隔497天就重新计数 
	OSTimeSet(0);
	OSStart(); 
	return (0);	
}
/************************************************************************************************
** Function name :			
** Description :系统运行所需数据
** 数据初始化,要先于任务运行之前赋初值
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
** Description :第二阶段硬件初始化，需要系统数据支持，必须在系统数据运行之后运行。例如背光值，先读取flash数据
**				得到之后，再设置相应的硬件参数。
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
	if(LIQUID_WRITE_SN_READ()) {	//检测是否需要写码，拉低为写码
		LOG_WRITE_SN("等待写码\r\n");
		if(Get_Product_Number()==0) {
			LOG_WRITE_SN("获取机身号码成功\r\n");
			LOG_WRITE_SN(pumpBoardInfo.deviceSNStr);
			LOG_WRITE_SN("\r\n");
		} else {
			LOG_WRITE_SN("读取机身号码失败\r\n");
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
	LOG_WRITE_SN("读取本机机身号码....\r\n");
	if(Get_Product_Number()==0) {
		LOG_SIM900("获取机身号码成功\r\n");
		LOG_SIM900(pumpBoardInfo.deviceSNStr);
		LOG_SIM900("\r\n");
		pumpBoardInfo.isSNSave = SN_SAVE_YES;
	} else {
		LOG_SIM900("读取机身号码失败\r\n");
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
		LOG_SIM900("授权允许\r\n");
	} else {
		LOG_SIM900("禁止授权\r\n");
	}
	App_TaskCreate();
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); //启动DMA通道
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
	OS_CPU_SysTickInit();			//初始化ucos时钟节拍，配置100Hz系统心跳
	//使能ucos 的统计任务
	#if (OS_TASK_STAT_EN > 0)
	//----统计任务初始化函数  
	OSStatInit();                    /* Determine CPU capacity.                              */
	#endif
	BSP_Stage_2_Init();   			//硬件平台第二阶段初始化
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
