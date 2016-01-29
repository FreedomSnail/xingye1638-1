/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name :BSP.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : ף��һ
** Created date :2012��2��22��16:18:26
** Version :V1.1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :ף��һ
** Modified date :2013��4��21��9:16:19
** Version :
** Description :
************************************************************************************************/
#include "includes.h"

vu16 AdcValue[N][M]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ
vu16 AfterFilter[M]; //���������ƽ��ֵ֮��Ľ��

Device_TypeDef Device;
/************************************************************************************************
** Function name :			
** Description :����48Mϵͳʱ��
** RCC����(ʹ���ⲿ8MHz����)��6��Ƶ
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void RCC_Configuration(void)
{	
	ErrorStatus HSEStartUpStatus;
  	//SystemInit(); 
	/*������RCC�Ĵ�������Ϊȱʡֵ */   
	RCC_DeInit();
	/*�����ⲿ���پ���HSE��*/    
	RCC_HSEConfig(RCC_HSE_ON);   //RCC_HSE_ON����HSE�����(ON)     
	/*�ȴ�HSE����*/    
	HSEStartUpStatus = RCC_WaitForHSEStartUp();     
	if(HSEStartUpStatus == SUCCESS){      //SUCCESS��HSE�����ȶ��Ҿ��� 
	/*����AHBʱ�ӣ�HCLK��*/       
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  //RCC_SYSCLK_Div1����AHBʱ�� = ϵͳʱ��        
	/* ���ø���AHBʱ�ӣ�PCLK2��*/      
	RCC_PCLK2Config(RCC_HCLK_Div1); //RCC_HCLK_Div1����APB2ʱ�� = HCLK        
	/*���õ���AHBʱ�ӣ�PCLK1��*/      
	RCC_PCLK1Config(RCC_HCLK_Div2);   //RCC_HCLK_Div2����APB1ʱ�� = HCLK /  2       
	/*����FLASH�洢����ʱʱ��������*/      
	FLASH_SetLatency(FLASH_Latency_2);    //FLASH_Latency_2  2��ʱ����       
	/*ѡ��FLASHԤȡָ�����ģʽ*/       
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // Ԥȡָ����ʹ��       
	/*����PLLʱ��Դ����Ƶϵ��*/       
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);  // PLL������ʱ�� = HSEʱ��Ƶ�ʣ�RCC_PLLMul_5����PLL����ʱ��x 5         
	/*ʹ��PLL */      
	RCC_PLLCmd(ENABLE);        
	/*���ָ����RCC��־λ(PLL׼���ñ�־)�������*/        
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){        
	}       
	/*����ϵͳʱ�ӣ�SYSCLK�� */       
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);   //RCC_SYSCLKSource_PLLCLK����ѡ��PLL��Ϊϵͳʱ��       
	/* PLL��������ϵͳʱ�ӵ�ʱ��Դ*/      
	while(RCC_GetSYSCLKSource() != 0x08);   //0x08��PLL��Ϊϵͳʱ��         {         }      
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
** ������
************************************************************************************************/
void GPIO_Beep_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	BEEP_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** ҹ���ź�ָʾ��
************************************************************************************************/
void GPIO_Led_Night_Flight_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	LED_NIGHT_FLIGHT_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = LED_NIGHT_FLIGHT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(LED_NIGHT_FLIGHT_PORT, &GPIO_InitStructure);
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** �ź�ָʾ��
************************************************************************************************/
void GPIO_Led_Operation_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	LED_OPERATION_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = LED_OPERATION_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(LED_OPERATION_PORT, &GPIO_InitStructure);
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** ũҩ����ܽ�
************************************************************************************************/
void GPIO_Liquid_Outof_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	LIQUID_OUTOF_IN_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = LIQUID_OUTOF_IN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//����
	GPIO_Init(LIQUID_OUTOF_IN_PORT, &GPIO_InitStructure);
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** ˮ�õ�Դ���ؿ�������
ˮ��ǰ -- PB07
ˮ���� -- PB08
ˮ�ú� -- PB09
************************************************************************************************/
void GPIO_Pump_Ctrl_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	PUMP_CONTROL_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = PUMP_CONTROL_FRONT_PIN|PUMP_CONTROL_CENTER_PIN|PUMP_CONTROL_REAR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(PUMP_CONTROL_PORT, &GPIO_InitStructure);
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
void GPIO_ADC1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//PC0/1/2/3 ��Ϊģ��ͨ����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** д���жϽ�
************************************************************************************************/
void GPIO_Write_SN_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	LIQUID_WRITE_SN_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = LIQUID_WRITE_SN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_Init(LIQUID_WRITE_SN_PORT, &GPIO_InitStructure);
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
void GPIO_Configuration(void)
{
	GPIO_Pump_Ctrl_Config();
	GPIO_Beep_Config();
	GPIO_Led_Night_Flight_Config();
	GPIO_Led_Operation_Config();
	GPIO_Liquid_Outof_Config();
	GPIO_ADC1_Config();
	GPIO_MX25_Config();
	GPIO_MCP41xxx_Config();
	GPIO_Write_SN_Config();
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
void ADC1_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	RCC_ADCCLKConfig(RCC_PCLK2_Div8); //72M/8=9,ADC���ʱ�䲻�ܳ���14M
	/* Enable ADC1  clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_DeInit(ADC1); //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ


	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode =ENABLE; //ģ��ת��������ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�ⲿ����ת���ر�
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = M; //˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure); //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���


	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	//ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5 );
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_239Cycles5 );
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 6, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_71Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_71Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_71Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_71Cycles5 );
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 11, ADC_SampleTime_239Cycles5 );
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 12, ADC_SampleTime_239Cycles5 );

	// ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
	ADC_DMACmd(ADC1, ENABLE);


	ADC_Cmd(ADC1, ENABLE); //ʹ��ָ����ADC1

	ADC_ResetCalibration(ADC1); //��λָ����ADC1��У׼�Ĵ���

	while(ADC_GetResetCalibrationStatus(ADC1)); //��ȡADC1��λУ׼�Ĵ�����״̬,����״̬��ȴ�


	ADC_StartCalibration(ADC1); //��ʼָ��ADC1��У׼״̬

	while(ADC_GetCalibrationStatus(ADC1)); //��ȡָ��ADC1��У׼����,����״̬��ȴ�


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
void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMA����
	DMA_DeInit(DMA1_Channel1); //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AdcValue; //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = N*M; //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //����DMAͨ��
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
void IWDG_Configuration(void)
{
	/* д��0x5555,�����������Ĵ���д�빦�� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* ����ʱ�ӷ�Ƶ,40K/256=156HZ(6.4ms)*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	/* ι��ʱ�� 1.5s/6.4MS=234 .ע�ⲻ�ܴ���0xfff*/
	IWDG_SetReload(234);
	
	/* ι��*/
	IWDG_ReloadCounter();
	
	/* ʹ�ܹ���*/
//	IWDG_Enable();
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** ��ʱ��1��CH1~4�������pwm�ź�
************************************************************************************************/
void TIM1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStruct;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//50Mʱ���ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/*-------------------------------------------------------------------
	TIM2CLK=48MHz  Ԥ��Ƶϵ��Prescaler=47 ������Ƶ ��ʱ��ʱ��Ϊ1MHz
	-------------------------------------------------------------------*/
	TIM_TimeBaseStructure.TIM_Prescaler = 48-1;						// 1�����72M/72=1M��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���������ϼ���ģʽ TIM3_CR1[4]=0
	TIM_TimeBaseStructure.TIM_Period =6700;				         		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;					//ʱ�ӷ�Ƶ���� TIM3_CR1[9:8]=00
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_Pulse = 2000;
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
	TIM_OC2Init(TIM1,&TIM_OCInitStruct);
	TIM_OC3Init(TIM1,&TIM_OCInitStruct);
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	TIM_SetAutoreload(TIM1,2000);				//��������
	TIM_SetCompare1(TIM1,1000);					//���øߵ�ƽʱ��
	//TIM_CtrlPWMOutputs(TIM1, ENABLE);  

	
	/*Ԥ������ж�λ*/
	//TIM_ClearITPendingBit(TIM1,TIM_IT_Update); 
	//TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
	
	/* ����TIM1��ʼ���� */
	//TIM_Cmd(TIM1, ENABLE);
	/* ֹͣTIM1��ʼ���� */
	TIM_Cmd(TIM1, DISABLE);
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
�����⣬ʹ�ö�ʱ��2�����벶���1��2����׽��ƽ������
�����½�һ��gpio�ڳ�ʼ�����ܽ�����Ҳ��������
************************************************************************************************/
void TIM2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//50Mʱ���ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/*-------------------------------------------------------------------
	TIM2CLK=48MHz  Ԥ��Ƶϵ��Prescaler=47 ������Ƶ ��ʱ��ʱ��Ϊ1MHz
	-------------------------------------------------------------------*/
	TIM_TimeBaseStructure.TIM_Prescaler = 48-1;						    //Ԥ��Ƶ��TIM3_PSC=3	 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���������ϼ���ģʽ TIM3_CR1[4]=0
	TIM_TimeBaseStructure.TIM_Period =TIM2_OVER_FLOW_VALUE;			//50ms�ж�һ��	    
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;					//ʱ�ӷ�Ƶ���� TIM3_CR1[9:8]=00
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  	//TIM����2��3��4ѡ���Ӧ��IC1��IC2��IC3��IC4����
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     		//TIM�����ڲ���������ÿ̽�⵽һ������ִ��һ�� 
  	TIM_ICInitStructure.TIM_ICFilter = 0x0f;      					//�˲����ã������������������϶������ȶ�0x0-0xf; 
	TIM_ICInit(TIM2, &TIM_ICInitStructure);


	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  	//TIM����2��3��4ѡ���Ӧ��IC1��IC2��IC3��IC4����
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     		//TIM�����ڲ���������ÿ̽�⵽һ������ִ��һ�� 
  	TIM_ICInitStructure.TIM_ICFilter = 0x0f;      					//�˲����ã������������������϶������ȶ�0x0-0xf; 
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	/*Ԥ����������ж�λ*/
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC1); 
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC2); 

	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	/* ����TIM2��ʼ���� */
//	TIM_Cmd(TIM2, ENABLE);
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** ��ʱ��3��CH2~4�������pwm
************************************************************************************************/
void TIM3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//��������ָʾ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//50Mʱ���ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/*-------------------------------------------------------------------
	TIM2CLK=48MHz  Ԥ��Ƶϵ��Prescaler=47 ������Ƶ ��ʱ��ʱ��Ϊ1MHz
	-------------------------------------------------------------------*/
	TIM_TimeBaseStructure.TIM_Prescaler = 48-1;						    //Ԥ��Ƶ��TIM3_PSC=3	 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���������ϼ���ģʽ TIM3_CR1[4]=0
	TIM_TimeBaseStructure.TIM_Period =50000;				         	//50ms�ж�һ��	    
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;					//ʱ�ӷ�Ƶ���� TIM3_CR1[9:8]=00
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  	//TIM����2��3��4ѡ���Ӧ��IC1��IC2��IC3��IC4����
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     		//TIM�����ڲ���������ÿ̽�⵽һ������ִ��һ�� 
  	TIM_ICInitStructure.TIM_ICFilter = 0x03;      					//�˲����ã������������������϶������ȶ�0x0-0xf; 
	TIM_ICInit(TIM2, &TIM_ICInitStructure);


	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  	//TIM����2��3��4ѡ���Ӧ��IC1��IC2��IC3��IC4����
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     		//TIM�����ڲ���������ÿ̽�⵽һ������ִ��һ�� 
  	TIM_ICInitStructure.TIM_ICFilter = 0x03;      					//�˲����ã������������������϶������ȶ�0x0-0xf; 
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	/*Ԥ����������ж�λ*/
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC1); 
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC2); 

	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	/* ����TIM2��ʼ���� */
//	TIM_Cmd(TIM2, ENABLE);
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
////SYSTICK ���ã� 10msһ��systick�жϣ�����ucos ���Ƚ��ģ� 1KHz
void  OS_CPU_SysTickInit(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    INT32U         cnts;


    RCC_GetClocksFreq(&rcc_clocks);

    cnts = (INT32U)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;

	if(SysTick_Config(cnts))
	{
		while(1);
	}
}
/************************************************************************************************
** Function name :			
** Description :��һ�׶�Ӳ����ʼ������ײ��Ӳ����ʼ��������Ҫϵͳ����֧�֡�����ϵͳ���ݳ�ʼ��֮ǰ���У�
**				��Ϊϵͳ���ݳ�ʼ��Ҫͨ�����ⲿflash���ݻ�á�				
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void BSP_Stage_1_Init(void)
{
  	RCC_Configuration();   				//ʱ�������䣬��������72M��Ƶ  	
  	GPIO_Configuration();				//��IO��
	DMA_Configuration();
	ADC1_Configuration();
	USART2_Config(SERIAL_PORT_DJI_SDK,115200);		//����2��ʼ��,DJI_SDK�ӿ�
	USART3_Config(SERIAL_PORT_SIM900A,9600);		//����3��ʼ��,SIM900A�ӿ�
	UART4_Config(SERIAL_PORT_DEBUG,115200);		//����4��ʼ�������Խӿ�
	SPI_Flash_Init();					//SPIģ�飬������д�ⲿflashоƬ
	TIM2_Config();						//PWM��⣬�õ���ʱ��2�����벶���ܣ�����ͨ��1��2
  	NVIC_Configuration();				//�ж���������,�����õõ����ж϶�����������
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** �رյ��Խӿڣ���GPIOʹ��
************************************************************************************************/
void UnableJTAG(void)
{
   RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                     // enable clock for Alternate Function
   AFIO->MAPR &= ~(7UL<<24); // clear used bit
   AFIO->MAPR |= (4UL<<24); // set used bits
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
void Adc_Filter(void)
{
	u8 i;
	u16 sum = 0;
	u8 count;
	for(i=0;i<M;i++) {
		for ( count=0;count<N;count++){
			sum += AdcValue[count][i];
		}
		AfterFilter[i]=sum/N;
		sum = 0;
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
** �ϵ�ʱ��ȡ����ˮ�õ���ֵ�Ļ�׼�Ƚ�ֵ
************************************************************************************************/
float Get_Pump_Current_Ref(void)
{
	Adc_Filter();
	Adc_Filter();
	return (float)AfterFilter[0]*3.3/4096;
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** ��ȡˮ�õ���ֵVout=(0.5Vcc+Ip*Sensitivity)/2
************************************************************************************************/
float Get_Pump_Current(void)
{
	float Amp;
	float ADC_ConvertedValue;
	Adc_Filter();
	ADC_ConvertedValue= AfterFilter[0];//�������
	ADC_ConvertedValue = ADC_ConvertedValue*3.3/4096;
	// 5v����
	if(ADC_ConvertedValue<Device.PumpCurrentRef) {
		Amp = 0;
	} else if(ADC_ConvertedValue<(Device.PumpCurrentRef+2.5*0.185)) { // (2500+5*185)/2=1712
		ADC_ConvertedValue = (ADC_ConvertedValue-Device.PumpCurrentRef)*2;
		Amp = ADC_ConvertedValue/0.185;
	} else {
		Amp = 5;	// 50����5A
	}
	return Amp;
}

/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** ��ȡ6S��ѹֵ
************************************************************************************************/
u16 Get_6S_Val(void)
{
	float ADC_ConvertedValue;
	Adc_Filter();
	ADC_ConvertedValue= AfterFilter[2];//����6s��ѹ
	ADC_ConvertedValue = ADC_ConvertedValue/4096*3.3*180+0.5;	//�������봦��
	return (u16)ADC_ConvertedValue;
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
��ֵ���ѹ��Ӧ��
10  --- 14.63v
50  --- 15.48v
65  --- 15.79
80  --- 16.1v
127 --- 17.08v
150 --- 22.51v
************************************************************************************************/
void Pump_Voltage_Set(u8 volt)
{
	if(volt == 0) {
		PUMP_FRONT_CLOSE;
		PUMP_CENTER_CLOSE;
		PUMP_REAR_CLOSE;	
		Device.LiquidSpeed = 0;
	} else {
		if(Device.PumpVoltage != volt) {
			Device.PumpVoltage = volt;
			MCP41xxx_Write_RES(Device.PumpVoltage);
		}
		Device.LiquidSpeed = 80;
		PUMP_FRONT_OPEN;
		PUMP_CENTER_OPEN;
		PUMP_REAR_OPEN;	
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
void Log_test(void)
{
	u8 Str[]="$24.0V,24.0V,0.0A,0*xx\r\n";// 6S��ѹ��ˮ�õ�ѹ��ˮ�õ�����ũҩ�Ƿ��Ѿ�
	*Str = *Str;	//��ֹ���뾯��
	Str[1] = '0'+Device.V6s/100;
	Str[2] = '0'+Device.V6s/10%10;
	Str[4] = '0'+Device.V6s%10;

	Str[7] = '0'+Device.PumpVoltage/100;
	Str[8] = '0'+Device.PumpVoltage/10%10;
	Str[10] = '0'+Device.PumpVoltage%10;


	Str[13] = '0'+(u8)Device.PumpCurrent;
	Str[15] = '0'+(u8)(Device.PumpCurrent*10)%10;

	if(Device.isDoseRunOut) {
		Str[18] = '1';
	} else {
		Str[18] = '0';
	}
	 
	LOG_ADC(Str);
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
void Send_Msg_2_M100(void)
{
	//u8 Str[]="$LIQUID,24.0V,24.0V,100A,000,000*xx\r\n";// 6S��ѹ��6s��ѹ���ܵ�����ˮ�ÿ��ϳ̶�(pwmռ�ձ�)���������ϳ̶�
	//str[0] = "123456789";
	//memcpy(str,"123456789",9);
	#if 1
	// ��һ�����ֽ���12s��ѹֵ�������ĸ��ֽ�������һ�ߵ�6s��ѹ���������������
	// �ֽ�����   ��С(��λbyte)   ˵��
	// 0			2				12s��ѹ
	// 2			2				6s��ѹ
	// 4			2				����
	// 6			1				��ͷ�ٶ�
	// 7			1				�����ٶ�
	// 8			1				�Ƿ�ҩ��
	// 9			1				�����
	//ע:��׿��λ������ʹ�õ���byte����(��Χ-128~127),Ϊ�˷�����λ���Ĵ����д������ʹ��128���Ʊ�ʾ����
	str[0] = Device.V12s/128;
	str[1] = Device.V12s%128;
	
	str[2] = Device.V6s/128;
	str[3] = Device.V6s%128;

	str[4] = Device.Amp/128;
	str[5] = Device.Amp%128;
	
	//str[4] = 50;
	
	str[6] = Device.LiquidSpeed;
	//str[6] = 80;

	str[7] = Device.Atomizer;

	str[8] = Device.isDoseRunOut;
	str[9]++;
	#endif
	
	DJI_Onboard_send();
}

