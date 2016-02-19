#ifndef _BSP_H
#define _BSP_H

#define	BEEP_PORT_RCC_ENABLE 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)
#define	BEEP_PORT				GPIOC
#define	BEEP_PIN				GPIO_Pin_13
#define BEEP_OFF  				BEEP_PORT->BRR  = BEEP_PIN
#define BEEP_ON					BEEP_PORT->BSRR = BEEP_PIN	//BSRR设置使cpu的io口出高电平




#define	LED_NIGHT_FLIGHT_PORT_RCC_ENABLE 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)
#define	LED_NIGHT_FLIGHT_PORT				GPIOC
#define	LED_NIGHT_FLIGHT_PIN				GPIO_Pin_8
#define LED_NIGHT_FLIGHT_OFF  				LED_NIGHT_FLIGHT_PORT->BSRR = LED_NIGHT_FLIGHT_PIN	//BSRR设置使cpu的io口出高电平
#define LED_NIGHT_FLIGHT_ON					LED_NIGHT_FLIGHT_PORT->BRR  = LED_NIGHT_FLIGHT_PIN

#define	LED_OPERATION_PORT_RCC_ENABLE 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define	LED_OPERATION_PORT				GPIOB
#define	LED_OPERATION_PIN				GPIO_Pin_5
#define LED_OPERATION_ON				LED_OPERATION_PORT->BRR  = LED_OPERATION_PIN
#define LED_OPERATION_OFF  				LED_OPERATION_PORT->BSRR = LED_OPERATION_PIN	//BSRR设置使cpu的io口出高电平


#define LIQUID_OUTOF_IN_PORT_RCC_ENABLE	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
#define LIQUID_OUTOF_IN_PORT			GPIOA
#define LIQUID_OUTOF_IN_PIN				GPIO_Pin_6
#define LIQUID_OUTOF_IN_READ()  		((LIQUID_OUTOF_IN_PORT->IDR & LIQUID_OUTOF_IN_PIN) != 0)	/* 读取液位信号口线状态 */

#define LIQUID_WRITE_SN_PORT_RCC_ENABLE	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
#define LIQUID_WRITE_SN_PORT			GPIOA
#define LIQUID_WRITE_SN_PIN				GPIO_Pin_8
#define LIQUID_WRITE_SN_READ()  		((LIQUID_WRITE_SN_PORT->IDR & LIQUID_WRITE_SN_PIN) == 0)	/* 读取写码检测口线状态 */

#define	PUMP_CONTROL_PORT_RCC_ENABLE	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define	PUMP_CONTROL_PORT				GPIOB
#define	PUMP_CONTROL_FRONT_PIN 			GPIO_Pin_7
#define	PUMP_CONTROL_CENTER_PIN 		GPIO_Pin_8
#define	PUMP_CONTROL_REAR_PIN 			GPIO_Pin_9

#define PUMP_FRONT_OPEN			PUMP_CONTROL_PORT->BSRR = PUMP_CONTROL_FRONT_PIN	//BSRR是置位，BRR是复位
#define PUMP_FRONT_CLOSE		PUMP_CONTROL_PORT->BRR  = PUMP_CONTROL_FRONT_PIN

#define PUMP_CENTER_OPEN		PUMP_CONTROL_PORT->BSRR = PUMP_CONTROL_CENTER_PIN
#define PUMP_CENTER_CLOSE		PUMP_CONTROL_PORT->BRR  = PUMP_CONTROL_CENTER_PIN

#define PUMP_REAR_OPEN			PUMP_CONTROL_PORT->BSRR = PUMP_CONTROL_REAR_PIN
#define PUMP_REAR_CLOSE			PUMP_CONTROL_PORT->BRR  = PUMP_CONTROL_REAR_PIN

#define	PUMP_VOLTAGE_10V		200			// 200 --- 10.07V
#define	PUMP_VOLTAGE_12V		168			// 152 --- 12.07V
#define	PUMP_VOLTAGE_16V		54			// 54 --- 16.07V

#define	PUMP_VOLTAGE_OUT			PUMP_VOLTAGE_16V

#define	PUMP_CURRENT_CRITICAL_POINT	1.0f	// 1A电流
#define	SAME_CURRENT_STATUS_CNT	4



#define N 20 //每通道采50次
#define M 4 //为12个通道

#define	PWM_RAISING_CNT		50	//PWM上升沿计数，超过50个认为是A2版本，否则是A3版本
#define	PWM_HIGHT_LEVEL_WIDTH_UP		1600
#define	PWM_HIGHT_LEVEL_WIDTH_DOWN		1400

#define	TIM2_OVER_FLOW_VALUE 50000

#define DATA_LENGTH_RECEIVE_PUMP_CONTROL_BOARD	27
#define DATA_LENGTH_SEND_PUMP_CONTROL_BOARD		13

#define 	TXA_SN_LENTH		16
#define 	DELAY_CHECK_PUMP_CURRENT_CNT		10


typedef void (*pUsartProcess_t)(u8);


typedef enum {
	ONLINE,
	OFFLINE
}SignalStatus_TypeEnum;

typedef enum {
	PERMISSION_ALLOW,
	PERMISSION_PROHIBIT
}Permission_TypeEnum;

typedef enum {
	SN_SAVE_YES,
	SN_SAVE_NO
}IsSNSave_TypeEnum;

typedef enum {
	PUMP_STOP,
	PUMP_RUNNING
}isPumpRunning_TypeEnum;
typedef enum {
	DOSE_FULL,
	DOSE_EMPTY
}isDoseRunOut_TypeEnum;


typedef struct
{
	Permission_TypeEnum permission;				//授权，是否可用
	IsSNSave_TypeEnum isSNSave;					//是否保存有正确的机身编号

	SignalStatus_TypeEnum  pwmOnlineFlag;	
	SignalStatus_TypeEnum  remoteOnlineFlag;

	
	isPumpRunning_TypeEnum 	isPumpRunning;		//水泵是否工作
	isDoseRunOut_TypeEnum 	isDoseRunOut;		//农药量是否耗尽
					//机身编号
	u16 PWMPeriod;
	u16	LiquidSpeed;
	u16	PumpDelayCheckCurrentFlag;			//滞后检查电流计数
	u16	PumpDelayCheckCurrent;				//滞后检查电流计数
	
	float PumpCurrentRef;	 				//水泵电流基准值
	float PumpCurrent;						//水泵电流
	float PumpVoltage;						//检测到的水泵电压
	float targetPumpVoltage;
	float supplyVoltage;					//供电电压，外部提供给水泵控制板的电压
	
	u8  deviceSNStr[TXA_SN_LENTH];			//机身编号字符串
	uint64_t deviceSN;		
	pUsartProcess_t Usart2Process;
}pump_board_data_t;



extern vu16 AdcValue[][M]; //用来存放ADC转换结果，也是DMA的目标地址
extern vu16 AfterFilter[]; //用来存放求平均值之后的结果


extern pump_board_data_t pumpBoardInfo;





void RCC_Configuration(void);

void GPIO_Configuration(void);

void NVIC_Configuration(void);

CPU_INT32U  BSP_CPU_ClkFreq (void);

//INT32U  OS_CPU_SysTickClkFreq (void);

void  OS_CPU_SysTickInit(void);

void BSP_Stage_1_Init(void);
void Adc_Filter(void);
float Get_Pump_Current_Ref(void);
float Get_Pump_Current(void);
float Get_Supply_Voltage(void);
float Get_Pump_Voltage(void);
void Pump_Voltage_Set(u8 volt);
void Log_test(void);
void Send_Msg_2_M100(void);

void send_cmd_to_flight_ctrl_board(u8* str,u8 len);

void Pro_Receive_Flight_Ctrl_Board(void);


#endif
