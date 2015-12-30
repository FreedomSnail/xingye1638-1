#ifndef _BSP_H
#define _BSP_H

#define	LED_OPERATION_PORT	GPIOA
#define	LED_OPERATION_PIN	GPIO_Pin_12
#define LED_OPERATION_ON	LED_OPERATION_PORT->BRR  = LED_OPERATION_PIN
#define LED_OPERATION_OFF  	LED_OPERATION_PORT->BSRR = LED_OPERATION_PIN	//BSRR����ʹcpu��io�ڳ��ߵ�ƽ

#define LIQUID_OUTOF_IN_PORT	GPIOA
#define LIQUID_OUTOF_IN_PIN		GPIO_Pin_6
#define LIQUID_OUTOF_IN_READ()  ((LIQUID_OUTOF_IN_PORT->IDR & LIQUID_OUTOF_IN_PIN) != 0)	/* ��ȡҺλ�źſ���״̬ */

#define LIQUID_WRITE_SN_PORT	GPIOA
#define LIQUID_WRITE_SN_PIN		GPIO_Pin_8
#define LIQUID_WRITE_SN_READ()  ((LIQUID_WRITE_SN_PORT->IDR & LIQUID_WRITE_SN_PIN) == 0)	/* ��ȡд�������״̬ */


#define	PUMP_CONTROL_PORT			GPIOB
#define	PUMP_CONTROL_FRONT_PIN 		GPIO_Pin_7
#define	PUMP_CONTROL_CENTER_PIN 	GPIO_Pin_8
#define	PUMP_CONTROL_REAR_PIN 		GPIO_Pin_9

#define PUMP_FRONT_OPEN			PUMP_CONTROL_PORT->BSRR = PUMP_CONTROL_FRONT_PIN	//BSRR����λ��BRR�Ǹ�λ
#define PUMP_FRONT_CLOSE		PUMP_CONTROL_PORT->BRR  = PUMP_CONTROL_FRONT_PIN

#define PUMP_CENTER_OPEN		PUMP_CONTROL_PORT->BSRR = PUMP_CONTROL_CENTER_PIN
#define PUMP_CENTER_CLOSE		PUMP_CONTROL_PORT->BRR  = PUMP_CONTROL_CENTER_PIN

#define PUMP_REAR_OPEN			PUMP_CONTROL_PORT->BSRR = PUMP_CONTROL_REAR_PIN
#define PUMP_REAR_CLOSE			PUMP_CONTROL_PORT->BRR  = PUMP_CONTROL_REAR_PIN

#define	PUMP_VOLTAGE_10V		200			// 200 --- 10.07V
#define	PUMP_VOLTAGE_12V		168			// 152 --- 12.07V
#define	PUMP_VOLTAGE_16V		54			// 54 --- 16.07V

#define	PUM_VOLTAGE_OUT			PUMP_VOLTAGE_12V


#define N 20 //ÿͨ����50��
#define M 4 //Ϊ12��ͨ��

#define	PWM_RAISING_CNT		50	//PWM�����ؼ���������50����Ϊ��A2�汾��������A3�汾
#define	PWM_HIGHT_LEVEL_WIDTH	1500

#define	TIM2_OVER_FLOW_VALUE 50000

typedef void (*pUsartProcess_t)(u8);


typedef enum {
	ONLINE,
	OFFLINE
}RemoteSignalStatus_TypeEnum;

typedef struct {
	u8  isA2;			//DJI A2����A3�汾 
	
	
	//u16 AmpInstant;	//����˲��ֵ
	//u16 AmpEverage;	//����ƽ��ֵ
	u16 Amp;
	u16 V12s;			//�����⵽�ĵ�ѹ��48.5v����485����ʾ
	u16 V6s;
	
	u16 PumpSwitch;		//ˮ�ÿ���
	u16	LiquidSpeed;
	u16 Atomizer;
	u16 AtomizerCurPWM;
	u16 AtomizerTargetPWM;
	u16 isDoseRunOut;	//ũҩ���� 1 ������ 0

	u16 PWMSignalCnt;
	u16 PWMRaisingTime;
	u16 PWMFallingTime;
	u16 PWMPeriod;
	
	//RemoteSignalStatus_TypeEnum RemoteSignalStatus;		//ң�����ź�״̬(δ��ʧ�Ͷ�ʧ)
	u16 LoseRemoteSignalCnt;	//��ʧң�����źż��� 

	u16 PumpVoltage;	//ˮ�õ�ѹ
	float PumpCurrentRef;	 //ˮ�õ�����׼ֵ
	float PumpCurrent;	//ˮ�õ���

	pUsartProcess_t Usart2Process;
}Device_TypeDef;



extern vu16 AdcValue[][M]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ
extern vu16 AfterFilter[]; //���������ƽ��ֵ֮��Ľ��


extern Device_TypeDef Device;





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
u16 Get_6S_Val(void);
void Pump_Voltage_Set(u8 volt);
void Log_test(void);
void Send_Msg_2_M100(void);
#endif
