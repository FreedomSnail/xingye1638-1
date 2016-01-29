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
** Created by : 祝定一
** Created date :2012年2月22日16:18:26
** Version :V1.1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :祝定一
** Modified date :2013年4月21日9:16:19
** Version :
** Description :
************************************************************************************************/
#include "includes.h"

vu16 AdcValue[N][M]; //用来存放ADC转换结果，也是DMA的目标地址
vu16 AfterFilter[M]; //用来存放求平均值之后的结果

Device_TypeDef Device;
/************************************************************************************************
** Function name :			
** Description :配置48M系统时钟
** RCC配置(使用外部8MHz晶振)，6倍频
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
	/*将外设RCC寄存器重设为缺省值 */   
	RCC_DeInit();
	/*设置外部高速晶振（HSE）*/    
	RCC_HSEConfig(RCC_HSE_ON);   //RCC_HSE_ON——HSE晶振打开(ON)     
	/*等待HSE起振*/    
	HSEStartUpStatus = RCC_WaitForHSEStartUp();     
	if(HSEStartUpStatus == SUCCESS){      //SUCCESS：HSE晶振稳定且就绪 
	/*设置AHB时钟（HCLK）*/       
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  //RCC_SYSCLK_Div1——AHB时钟 = 系统时钟        
	/* 设置高速AHB时钟（PCLK2）*/      
	RCC_PCLK2Config(RCC_HCLK_Div1); //RCC_HCLK_Div1——APB2时钟 = HCLK        
	/*设置低速AHB时钟（PCLK1）*/      
	RCC_PCLK1Config(RCC_HCLK_Div2);   //RCC_HCLK_Div2——APB1时钟 = HCLK /  2       
	/*设置FLASH存储器延时时钟周期数*/      
	FLASH_SetLatency(FLASH_Latency_2);    //FLASH_Latency_2  2延时周期       
	/*选择FLASH预取指缓存的模式*/       
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // 预取指缓存使能       
	/*设置PLL时钟源及倍频系数*/       
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);  // PLL的输入时钟 = HSE时钟频率；RCC_PLLMul_5——PLL输入时钟x 5         
	/*使能PLL */      
	RCC_PLLCmd(ENABLE);        
	/*检查指定的RCC标志位(PLL准备好标志)设置与否*/        
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){        
	}       
	/*设置系统时钟（SYSCLK） */       
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);   //RCC_SYSCLKSource_PLLCLK——选择PLL作为系统时钟       
	/* PLL返回用作系统时钟的时钟源*/      
	while(RCC_GetSYSCLKSource() != 0x08);   //0x08：PLL作为系统时钟         {         }      
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
** 蜂鸣器
************************************************************************************************/
void GPIO_Beep_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	BEEP_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
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
** 夜航信号指示灯
************************************************************************************************/
void GPIO_Led_Night_Flight_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	LED_NIGHT_FLIGHT_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = LED_NIGHT_FLIGHT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
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
** 信号指示灯
************************************************************************************************/
void GPIO_Led_Operation_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	LED_OPERATION_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = LED_OPERATION_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
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
** 农药完检测管脚
************************************************************************************************/
void GPIO_Liquid_Outof_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	LIQUID_OUTOF_IN_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = LIQUID_OUTOF_IN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//输入
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
** 水泵电源开关控制引脚
水泵前 -- PB07
水泵中 -- PB08
水泵后 -- PB09
************************************************************************************************/
void GPIO_Pump_Ctrl_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	PUMP_CONTROL_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = PUMP_CONTROL_FRONT_PIN|PUMP_CONTROL_CENTER_PIN|PUMP_CONTROL_REAR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
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
	//PC0/1/2/3 作为模拟通道输入引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入引脚
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
** 写码判断脚
************************************************************************************************/
void GPIO_Write_SN_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	LIQUID_WRITE_SN_PORT_RCC_ENABLE;
	GPIO_InitStructure.GPIO_Pin = LIQUID_WRITE_SN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
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

	RCC_ADCCLKConfig(RCC_PCLK2_Div8); //72M/8=9,ADC最大时间不能超过14M
	/* Enable ADC1  clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_DeInit(ADC1); //将外设 ADC1 的全部寄存器重设为缺省值


	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode =ENABLE; //模数转换工作在扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //模数转换工作在连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //外部触发转换关闭
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = M; //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure); //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器


	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
	//ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期
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

	// 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
	ADC_DMACmd(ADC1, ENABLE);


	ADC_Cmd(ADC1, ENABLE); //使能指定的ADC1

	ADC_ResetCalibration(ADC1); //复位指定的ADC1的校准寄存器

	while(ADC_GetResetCalibrationStatus(ADC1)); //获取ADC1复位校准寄存器的状态,设置状态则等待


	ADC_StartCalibration(ADC1); //开始指定ADC1的校准状态

	while(ADC_GetCalibrationStatus(ADC1)); //获取指定ADC1的校准程序,设置状态则等待


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
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA传输
	DMA_DeInit(DMA1_Channel1); //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AdcValue; //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = N*M; //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //根据DMA_InitStruct中指定的参数初始化DMA的通道
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //启动DMA通道
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
	/* 写入0x5555,用于允许狗狗寄存器写入功能 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* 狗狗时钟分频,40K/256=156HZ(6.4ms)*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	/* 喂狗时间 1.5s/6.4MS=234 .注意不能大于0xfff*/
	IWDG_SetReload(234);
	
	/* 喂狗*/
	IWDG_ReloadCounter();
	
	/* 使能狗狗*/
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
** 定时器1的CH1~4用来输出pwm信号
************************************************************************************************/
void TIM1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStruct;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//50M时钟速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/*-------------------------------------------------------------------
	TIM2CLK=48MHz  预分频系数Prescaler=47 经过分频 定时器时钟为1MHz
	-------------------------------------------------------------------*/
	TIM_TimeBaseStructure.TIM_Prescaler = 48-1;						// 1秒计数72M/72=1M次
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//计数器向上计数模式 TIM3_CR1[4]=0
	TIM_TimeBaseStructure.TIM_Period =6700;				         		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;					//时钟分频因子 TIM3_CR1[9:8]=00
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
	TIM_SetAutoreload(TIM1,2000);				//设置周期
	TIM_SetCompare1(TIM1,1000);					//设置高电平时间
	//TIM_CtrlPWMOutputs(TIM1, ENABLE);  

	
	/*预先清除中断位*/
	//TIM_ClearITPendingBit(TIM1,TIM_IT_Update); 
	//TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
	
	/* 允许TIM1开始计数 */
	//TIM_Cmd(TIM1, ENABLE);
	/* 停止TIM1开始计数 */
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
过零检测，使用定时器2的输入捕获脚1和2来捕捉电平的跳变
懒得新建一个gpio口初始化，管脚配置也放这里了
************************************************************************************************/
void TIM2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//50M时钟速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/*-------------------------------------------------------------------
	TIM2CLK=48MHz  预分频系数Prescaler=47 经过分频 定时器时钟为1MHz
	-------------------------------------------------------------------*/
	TIM_TimeBaseStructure.TIM_Prescaler = 48-1;						    //预分频器TIM3_PSC=3	 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//计数器向上计数模式 TIM3_CR1[4]=0
	TIM_TimeBaseStructure.TIM_Period =TIM2_OVER_FLOW_VALUE;			//50ms中断一次	    
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;					//时钟分频因子 TIM3_CR1[9:8]=00
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  	//TIM输入2，3或4选择对应的IC1或IC2或IC3或IC4相连
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     		//TIM捕获在捕获输入行每探测到一个边沿执行一次 
  	TIM_ICInitStructure.TIM_ICFilter = 0x0f;      					//滤波设置，经历几个周期跳变认定波形稳定0x0-0xf; 
	TIM_ICInit(TIM2, &TIM_ICInitStructure);


	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  	//TIM输入2，3或4选择对应的IC1或IC2或IC3或IC4相连
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     		//TIM捕获在捕获输入行每探测到一个边沿执行一次 
  	TIM_ICInitStructure.TIM_ICFilter = 0x0f;      					//滤波设置，经历几个周期跳变认定波形稳定0x0-0xf; 
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	/*预先清除所有中断位*/
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC1); 
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC2); 

	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	/* 允许TIM2开始计数 */
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
** 定时器3的CH2~4用来输出pwm
************************************************************************************************/
void TIM3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//按键操作指示灯
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//50M时钟速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/*-------------------------------------------------------------------
	TIM2CLK=48MHz  预分频系数Prescaler=47 经过分频 定时器时钟为1MHz
	-------------------------------------------------------------------*/
	TIM_TimeBaseStructure.TIM_Prescaler = 48-1;						    //预分频器TIM3_PSC=3	 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//计数器向上计数模式 TIM3_CR1[4]=0
	TIM_TimeBaseStructure.TIM_Period =50000;				         	//50ms中断一次	    
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;					//时钟分频因子 TIM3_CR1[9:8]=00
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  	//TIM输入2，3或4选择对应的IC1或IC2或IC3或IC4相连
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     		//TIM捕获在捕获输入行每探测到一个边沿执行一次 
  	TIM_ICInitStructure.TIM_ICFilter = 0x03;      					//滤波设置，经历几个周期跳变认定波形稳定0x0-0xf; 
	TIM_ICInit(TIM2, &TIM_ICInitStructure);


	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  	//TIM输入2，3或4选择对应的IC1或IC2或IC3或IC4相连
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     		//TIM捕获在捕获输入行每探测到一个边沿执行一次 
  	TIM_ICInitStructure.TIM_ICFilter = 0x03;      					//滤波设置，经历几个周期跳变认定波形稳定0x0-0xf; 
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	/*预先清除所有中断位*/
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC1); 
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC2); 

	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	/* 允许TIM2开始计数 */
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
////SYSTICK 配置， 10ms一次systick中断，产生ucos 调度节拍， 1KHz
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
** Description :第一阶段硬件初始化，最底层的硬件初始化，不需要系统数据支持。先于系统数据初始化之前运行，
**				因为系统数据初始化要通过读外部flash数据获得。				
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void BSP_Stage_1_Init(void)
{
  	RCC_Configuration();   				//时钟最先配，这里配置72M主频  	
  	GPIO_Configuration();				//配IO口
	DMA_Configuration();
	ADC1_Configuration();
	USART2_Config(SERIAL_PORT_DJI_SDK,115200);		//串口2初始化,DJI_SDK接口
	USART3_Config(SERIAL_PORT_SIM900A,9600);		//串口3初始化,SIM900A接口
	UART4_Config(SERIAL_PORT_DEBUG,115200);		//串口4初始化，调试接口
	SPI_Flash_Init();					//SPI模块，用来读写外部flash芯片
	TIM2_Config();						//PWM检测，用到定时器2的输入捕获功能，捕获通道1，2
  	NVIC_Configuration();				//中断向量分配,所有用得到的中断都在这里配置
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** 关闭调试接口，作GPIO使用
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
** 上电时获取测量水泵电流值的基准比较值
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
** 获取水泵电流值Vout=(0.5Vcc+Ip*Sensitivity)/2
************************************************************************************************/
float Get_Pump_Current(void)
{
	float Amp;
	float ADC_ConvertedValue;
	Adc_Filter();
	ADC_ConvertedValue= AfterFilter[0];//计算电流
	ADC_ConvertedValue = ADC_ConvertedValue*3.3/4096;
	// 5v供电
	if(ADC_ConvertedValue<Device.PumpCurrentRef) {
		Amp = 0;
	} else if(ADC_ConvertedValue<(Device.PumpCurrentRef+2.5*0.185)) { // (2500+5*185)/2=1712
		ADC_ConvertedValue = (ADC_ConvertedValue-Device.PumpCurrentRef)*2;
		Amp = ADC_ConvertedValue/0.185;
	} else {
		Amp = 5;	// 50代表5A
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
** 获取6S电压值
************************************************************************************************/
u16 Get_6S_Val(void)
{
	float ADC_ConvertedValue;
	Adc_Filter();
	ADC_ConvertedValue= AfterFilter[2];//计算6s电压
	ADC_ConvertedValue = ADC_ConvertedValue/4096*3.3*180+0.5;	//四舍五入处理
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
数值与电压对应表
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
	u8 Str[]="$24.0V,24.0V,0.0A,0*xx\r\n";// 6S电压，水泵电压，水泵电流，农药是否已尽
	*Str = *Str;	//防止编译警告
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
	//u8 Str[]="$LIQUID,24.0V,24.0V,100A,000,000*xx\r\n";// 6S电压，6s电压，总电流，水泵开合程度(pwm占空比)，雾化器开合程度
	//str[0] = "123456789";
	//memcpy(str,"123456789",9);
	#if 1
	// 第一二个字节是12s电压值，第三四个字节是其中一边的6s电压，具体见如下索引
	// 字节索引   大小(单位byte)   说明
	// 0			2				12s电压
	// 2			2				6s电压
	// 4			2				电流
	// 6			1				喷头速度
	// 7			1				雾化器速度
	// 8			1				是否药尽
	// 9			1				包序号
	//注:安卓上位机接收使用的是byte变量(范围-128~127),为了方便上位机的代码编写，这里使用128进制表示发送
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

