/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V1.1
* Date               : 05/30/2008
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED 
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

u16 TIM6_cnt=0;
u8 EXINT1_cnt=0;
//u8 EXINT3_cnt=0;
//extern void TimingDelay_Decrement(void);
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NMIException
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMIException(void)
{
 
}

/*******************************************************************************
* Function Name  : HardFaultException
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFaultException(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManageException
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManageException(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFaultException
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFaultException(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFaultException
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFaultException(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMonitor(void)
{
}

/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVCHandler(void)
{
}

/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSVC(void)
{
}

/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTickHandler(void)
{
//	TimingDelay_Decrement();
   OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR*/
    OSIntNesting++;	   //OSSemPost(NMEA_MBOX); 
    OS_EXIT_CRITICAL();	  //恢复全局中断标志
	
    OSTimeTick();     /* Call uC/OS-II's OSTimeTick(),在os_core.c文件里定义,主要判断延时的任务是否计时到*/
	IWDG_ReloadCounter();	//喂狗

	if( Device.LoseRemoteSignalCnt > 0) {
		Device.LoseRemoteSignalCnt--;
	}
	if(Device.PWMSignalCnt > 0) {
		Device.PWMSignalCnt--;
	}
    OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换    
     
}


/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : This function handles WWDG interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : PVD_IRQHandler
* Description    : This function handles PVD interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PVD_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TAMPER_IRQHandler
* Description    : This function handles Tamper interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TAMPER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
    
}

/*******************************************************************************
* Function Name  : FLASH_IRQHandler
* Description    : This function handles Flash interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RCC_IRQHandler
* Description    : This function handles RCC interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External interrupt Line 0 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI0_IRQHandler(void)
{

}
/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External interrupt Line 1 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI1_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : This function handles External interrupt Line 2 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI2_IRQHandler(void)
{


}

/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI3_IRQHandler(void)
{  


}
/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : This function handles External interrupt Line 4 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_IRQHandler(void)
{  
}

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel1_IRQHandler(void)
{


}

/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : This function handles DMA1 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel3_IRQHandler
* Description    : This function handles DMA1 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel4_IRQHandler
* Description    : This function handles DMA1 Channel 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel5_IRQHandler
* Description    : This function handles DMA1 Channel 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel6_IRQHandler
* Description    : This function handles DMA1 Channel 6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel7_IRQHandler
* Description    : This function handles DMA1 Channel 7 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC1_2_IRQHandler
* Description    : This function handles ADC1 and ADC2 global interrupts requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_HP_CAN_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN_TX_IRQHandler(void)
{
 
}

/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN_RX0_IRQHandler(void)
{
 
}

/*******************************************************************************
* Function Name  : CAN_RX1_IRQHandler
* Description    : This function handles CAN RX1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_RX1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN_SCE_IRQHandler
* Description    : This function handles CAN SCE interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_SCE_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	CPU_SR     cpu_sr;
	OS_ENTER_CRITICAL();
    	OSIntNesting++;
    	OS_EXIT_CRITICAL();
	//中断标志位清除
	EXTI_ClearITPendingBit(EXTI_Line7);
	//中断标志位清除
	//用户程序..
	
	//用户程序..
    OSIntExit();

}

/*******************************************************************************
* Function Name  : TIM1_BRK_IRQHandler
* Description    : This function handles TIM1 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : This function handles TIM1 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{
	CPU_SR         cpu_sr;
	OS_ENTER_CRITICAL();
	OSIntNesting++;
   	OS_EXIT_CRITICAL();
	//用户程序..
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	
	//用户程序..
	OSIntExit();

}
/*******************************************************************************
* Function Name  : TIM1_TRG_COM_IRQHandler
* Description    : This function handles TIM1 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_TRG_COM_IRQHandler(void)
{


}

/*******************************************************************************
* Function Name  : TIM1_CC_IRQHandler
* Description    : This function handles TIM1 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	CPU_SR         cpu_sr;
	u16 temp;
	OS_ENTER_CRITICAL();
  	OSIntNesting++;
	OS_EXIT_CRITICAL();
	//用户程序..
	Device.PWMSignalCnt = 10;
	if(TIM_GetITStatus(TIM2,TIM_IT_CC1) == SET) {	//下降沿中断,经过三级管倒相了 
		Device.PWMFallingTime = TIM_GetCapture1(TIM2);
		if(Device.isA2 < PWM_RAISING_CNT) {		//A2版本判断
			Device.isA2++;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	} else {
		Device.PWMRaisingTime = TIM_GetCapture2(TIM2);
		#if 1
		if(Device.PWMRaisingTime > Device.PWMFallingTime) {
			temp = Device.PWMRaisingTime - Device.PWMFallingTime;	
			if(temp>TIM2_OVER_FLOW_VALUE/2) {
				temp = TIM2_OVER_FLOW_VALUE - Device.PWMRaisingTime +Device.PWMFallingTime;
			}
		} else {
			temp = Device.PWMFallingTime - Device.PWMRaisingTime;
			if(temp>TIM2_OVER_FLOW_VALUE/2) {
				temp = TIM2_OVER_FLOW_VALUE  - Device.PWMFallingTime + Device.PWMRaisingTime;
			}
		}	
		Device.PWMPeriod= temp;
		#endif
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
	} 
	//用户程序..
    OSIntExit();

}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None

*******************************************************************************/

void TIM3_IRQHandler(void)
{
	CPU_SR         cpu_sr;
	OS_ENTER_CRITICAL();
  	OSIntNesting++;
	OS_EXIT_CRITICAL();
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	//用户程序..
	//USART_OUT(USART1,"*\r\n");
	
		
	//用户程序..
	
	OSIntExit();
}
/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	#if 0
	CPU_SR         cpu_sr;
	OS_ENTER_CRITICAL();
  	OSIntNesting++;
	OS_EXIT_CRITICAL();
	//if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		//用户程序..
		
		//用户程序..
	//}
	OSIntExit();
	#endif
}
/*******************************************************************************
* Function Name  : I2C1_EV_IRQHandler
* Description    : This function handles I2C1 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_ER_IRQHandler
* Description    : This function handles I2C1 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_ER_IRQHandler
* Description    : This function handles I2C2 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI1_IRQHandler
* Description    : This function handles SPI1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI2_IRQHandler
* Description    : This function handles SPI2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_IRQHandler(void)
{
 
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
	CPU_SR cpu_sr;
	u8 Rev;
	OS_ENTER_CRITICAL();
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	//用户程序..
	if(USART_GetITStatus(SERIAL_PORT_DJI_SDK, USART_IT_RXNE) != RESET) {	//判断读寄存器是否非空	
    	// Read one byte from the receive data register
    	Rev= USART_ReceiveData(SERIAL_PORT_DJI_SDK);   //将读寄存器的数据缓存到接收缓冲区里
		//USART_SendData(SERIAL_PORT_DJI_SDK,Rev);
		//while(USART_GetFlagStatus(SERIAL_PORT_DJI_SDK, USART_FLAG_TC)==RESET);
		(*Device.Usart2Process)(Rev);
		#if 0
		switch(Uart2.DjiPackageStatus) {
			case DJI_PACKAGE_RECV_IDLE:
				if(Rev == _SDK_SOF) {
			 		Uart2.RxIndex = 1;
			 		Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_COMMAND_SET;
			 		memset(Uart2.RxDataBuf,0,RX_MAX_NUM);
			 		Uart2.RxDataBuf[0] = _SDK_SOF;
		 		}
		 		break;
			case DJI_PACKAGE_RECV_COMMAND_SET:
		 		Uart2.RxDataBuf[Uart2.RxIndex] = Rev;
		 		if(Uart2.RxIndex == sizeof(SDKHeader)) {
			 		Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_COMMAND_ID;
					#if 0
					if(Rev == 0x00) {	 //命令集-激活验证类 -机载设备==>飞控
						Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_COMMAND_ID;
					} else if(Rev == 0x02) { //命令集-推送数据类 - 飞控==>机载设备
						Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_COMMAND_ID;
					} else {
						Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_IDLE;
					}
					#endif
		 		}
		 		Uart2.RxIndex++;
	 			break;
			case DJI_PACKAGE_RECV_COMMAND_ID:
				Uart2.RxDataBuf[Uart2.RxIndex++] = Rev;
				Uart2.DataLen = ((unsigned int)(0x03&&Uart2.RxDataBuf[2])<<8)+(unsigned int)Uart2.RxDataBuf[1];
				Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_WAIT_DONE;
	 			break;
			case DJI_PACKAGE_RECV_WAIT_DONE:
	 			if(Uart2.RxIndex<RX_MAX_NUM) {
		 			Uart2.RxDataBuf[Uart2.RxIndex++] = Rev;
		 			if(Uart2.RxIndex == Uart2.DataLen) {
			 			Uart2.RxFlag = 1;
			 			OSQPost(DecodeQSem,(void *)&Msg);
			 			Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_IDLE;
		 			}
	 			} else {//接收到的数据致使数组越界
		 			Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_IDLE;
	 			}
	 			break;
			default:
		 		break;
			
		}
		#endif
  	}
	//用户程序..
	OSIntExit();
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
	CPU_SR cpu_sr;
	u8 Rev,Msg=1;
	u8 i;
	OS_ENTER_CRITICAL();
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	//用户程序..
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) { //判断读寄存器是否非空	
		// Read one byte from the receive data register
		Rev= USART_ReceiveData(USART3);   //将读寄存器的数据缓存到接收缓冲区里
		//USART_SendData(USART2,Rev);
		//while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);	
		GsmCmd.RxDataBuf[GsmCmd.RxIndex] = Rev;
		GsmCmd.RxIndex++;
		//收到了完整的AT指令，完整的AT指令是以0x0a 0x0d结尾的。故作此判断，在接收的过程中是否收到0x0a 0x0d
		if(GsmCmd.RxDataBuf[GsmCmd.RxIndex-2] == '\r' && GsmCmd.RxDataBuf[GsmCmd.RxIndex-1] == '\n' && GsmCmd.RxIndex >= 2) {
			//一旦收到0x0a 0x0d，就将数据保存起来。用户主函数的判断。			
			for(i=0; i<GsmCmd.RxIndex; i++) {
				GsmCmd.RxDataAt[i] = GsmCmd.RxDataBuf[i];
				GsmCmd.RxDataBuf[i] = 0;	
			}
			GsmCmd.RxDataAt[GsmCmd.RxIndex] = 0;
			GsmCmd.RxIndex = 0;
			//GsmCmd.RxFlag = 1;//收到了完整的at指令，通过这个标志位置1，这样主函数就知道去判断了。	
			OSQPost(GprsQSem,(void *)&Msg);
		} else if (GsmCmd.RxIndex >= GSM_RCV_MAX_NUM) {//因为内存有限，收到了50个字符还是没有看到0x0a 0x0d的话，就重新开始接收吧。
			GsmCmd.RxIndex = 0;
		}
	}
	//用户程序..
	OSIntExit();
}

/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{


}

/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
 
}

/*******************************************************************************
* Function Name  : USBWakeUp_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBWakeUp_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_BRK_IRQHandler
* Description    : This function handles TIM8 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_UP_IRQHandler
* Description    : This function handles TIM8 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_UP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_TRG_COM_IRQHandler
* Description    : This function handles TIM8 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_CC_IRQHandler
* Description    : This function handles TIM8 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC3_IRQHandler
* Description    : This function handles ADC3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : FSMC_IRQHandler
* Description    : This function handles FSMC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : This function handles TIM5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI3_IRQHandler
* Description    : This function handles SPI3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : This function handles UART5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM6_IRQHandler
* Description    : This function handles TIM6 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM6_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : TIM7_IRQHandler
* Description    : This function handles TIM7 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*
*******************************************************************************/
void TIM7_IRQHandler(void)
{


}

/*******************************************************************************
* Function Name  : DMA2_Channel1_IRQHandler
* Description    : This function handles DMA2 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel2_IRQHandler
* Description    : This function handles DMA2 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel3_IRQHandler
* Description    : This function handles DMA2 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel4_5_IRQHandler
* Description    : This function handles DMA2 Channel 4 and DMA2 Channel 5
*                  interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel4_5_IRQHandler(void)
{
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
