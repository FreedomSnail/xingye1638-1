/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name :com.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : 祝定一
** Created date :2012年2月22日16:18:26
** Version :V1.1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :
** Modified date :
** Version :
** Description :
************************************************************************************************/
#include "includes.h"

UartTypedef		Uart2;
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
void USART1_Config(USART_TypeDef* USARTx,u32 baud)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  //PA9,PA10 复用IO口功能用于配置串口。因此要使能AFIO（复用功能IO）时钟。
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  //usart_init----------------------------------------------------
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //浮空输入模式	   
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 
  
  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					//控制485电路方向
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			  //复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIOA->BSRR  = GPIO_Pin_8;
  
  USART_InitStructure.USART_BaudRate =baud;						//速率115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

  /* Configure USART1 */
  USART_Init(USARTx, &USART_InitStructure);							//配置串口参数函数
 
  
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);                    //使能接收中断
  //USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);				    //使能发送缓冲空中断

  
  /* Enable the USART1 */
  USART_Cmd(USARTx, ENABLE);	

  //USART_ClearFlag(USARTx, USART_FLAG_TXE);     /* 清发送完成标志，Transmission Complete flag */
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
void USART2_Config(USART_TypeDef* USARTx,u32 baud)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  //PA2,PA3 复用IO口功能用于配置串口。因此要使能AFIO（复用功能IO）时钟。
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  //usart_init----------------------------------------------------
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //浮空输入模式	   
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 
  
  /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  USART_InitStructure.USART_BaudRate =baud;						//速率115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

  /* Configure USART2 */
  USART_Init(USARTx, &USART_InitStructure);							//配置串口参数函数
 
  
  /* Enable USART2 Receive and Transmit interrupts */
  //USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);                    //使能接收中断
  USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);                    //使能接收中断
  //USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);				    //使能发送缓冲空中断

  
  /* Enable the USART2 */
  USART_Cmd(USARTx, ENABLE);	

  //USART_ClearFlag(USARTx, USART_FLAG_TXE);     /* 清发送完成标志，Transmission Complete flag */
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
void USART3_Config(USART_TypeDef* USARTx,u32 baud)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  //PB10,PB11 复用IO口功能用于配置串口。因此要使能AFIO（复用功能IO）时钟。
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  //usart_init----------------------------------------------------
  /* Configure USART3 Rx (PB.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //浮空输入模式	   
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 
  
  /* Configure USART3 Tx (PB.10) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  
  USART_InitStructure.USART_BaudRate =baud;						//速率115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

  /* Configure USART3 */
  USART_Init(USARTx, &USART_InitStructure);							//配置串口参数函数
 
  
  /* Enable USART3 Receive and Transmit interrupts */
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);                    //使能接收中断
  //USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);				    //使能发送缓冲空中断

  
  /* Enable the USART3 */
  USART_Cmd(USARTx, ENABLE);	

  //USART_ClearFlag(USARTx, USART_FLAG_TXE);     /* 清发送完成标志，Transmission Complete flag */
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
void UART4_Config(USART_TypeDef* USARTx,u32 baud)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  //PC10,PC11 复用IO口功能用于配置串口。因此要使能AFIO（复用功能IO）时钟。
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  //usart_init----------------------------------------------------
  /* Configure USART4 Rx (PC.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //浮空输入模式	   
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 
  
  /* Configure USART4 Tx (PC.10) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  
  USART_InitStructure.USART_BaudRate =baud;						//速率115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

  /* Configure USART4 */
  USART_Init(USARTx, &USART_InitStructure);							//配置串口参数函数
 
  
  /* Enable USART4 Receive and Transmit interrupts */
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);                    //使能接收中断
  //USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);				    //使能发送缓冲空中断

  
  /* Enable the USART4 */
  USART_Cmd(USARTx, ENABLE);	

  //USART_ClearFlag(USARTx, USART_FLAG_TXE);     /* 清发送完成标志，Transmission Complete flag */
}

/******************************************************
		格式化串口输出函数
        "\r"	回车符	   USART_OUT(USART1, "abcdefg\r")   
		"\n"	换行符	   USART_OUT(USART1, "abcdefg\r\n")
		"%s"	字符串	   USART_OUT(USART1, "字符串是：%s","abcdefg")
		"%d"	十进制	   USART_OUT(USART1, "a=%d",10)
**********************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
{ 

	const char *s;
    int d;
   
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //判断是否到达字符串结束符
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data++;
					break;
				
				default:
					Data++;
				    break;
			}
			
			 
		}
		else if(*Data=='%'){									  //
			switch (*++Data){				
				case 's':										  //字符串
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //十进制
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}
  /******************************************************
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
		buf="-379"							   			  
**********************************************************/
char *itoa(long value, char *string, int radix)
{
    long     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 1000000000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */
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
void USART_Send_Buf(USART_TypeDef* USARTx, u8* buf, u16 len)
{ 
	  if(len<1) {
		  return;
	  }
	  while(len--){
		  USART_SendData(USARTx, *buf++);
		  while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
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
void USART2_DJI_Process(u8 Rev)
{ 
	u8 Msg = 0;
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
				 if(Rev == 0x00) {	  //命令集-激活验证类 -机载设备==>飞控
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
					 //OSQPost(UsartDjiCtrlPumpQsem,(void *)&Msg);
					 OSSemPost(SemDjiCodec);
					 Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_IDLE;
				 }
			 } else {//接收到的数据致使数组越界
				 Uart2.DjiPackageStatus = DJI_PACKAGE_RECV_IDLE;
			 }
			 break;
		 default:
			 break;
		 
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
void USART2_Write_SN_Process(u8 Rev)
{ 
	u8 Msg = 0;
	switch(Uart2.WriteSnPackageStatus) {
		case WRITE_SN_PACKAGE_RECV_IDLE:
			if(Rev == '$') {
				Uart2.RxIndex = 0;
				Uart2.WriteSnPackageStatus = WRITE_SN_PACKAGE_RECV_START;
				memset(Uart2.RxDataBuf,0,RX_MAX_NUM);
			}
			break;
		case WRITE_SN_PACKAGE_RECV_START:
			switch(Rev) {
				case '\r':
				//case '*':
					memcpy(Uart2.RxDataSecondBuf,Uart2.RxDataBuf,Uart2.RxIndex);
					Uart2.WriteSnPackageStatus = WRITE_SN_PACKAGE_RECV_IDLE;
					OSQPost(WriteSNQSem,(void *)&Msg);
					break;
				default:
					Uart2.RxDataBuf[Uart2.RxIndex] = Rev;
					Uart2.RxIndex++;
					if(Uart2.RxIndex==RX_MAX_NUM) {//防止数组溢出	
						Uart2.RxIndex = 0;
						Uart2.WriteSnPackageStatus = WRITE_SN_PACKAGE_RECV_IDLE;
					}
					break;
			}
			break;
		default:
			break;
	} 
}






















