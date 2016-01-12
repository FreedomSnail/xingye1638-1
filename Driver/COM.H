#ifndef _COM_H
#define _COM_H

#define		RX_MAX_NUM				100

#define	SERIAL_PORT_DJI_SDK		USART2
#define	SERIAL_PORT_SIM900A		USART3
#define	SERIAL_PORT_DEBUG		UART4

#define SIM900_AT_CMD_SEND(s) 	USART_OUT(SERIAL_PORT_SIM900A,s)
#define LOG_SIM900(s) 			USART_OUT(SERIAL_PORT_DEBUG,s)
#define LOG_ADC(s) 				//USART_OUT(SERIAL_PORT_DEBUG,s)
#define LOG_WRITE_SN(s) 		USART_OUT(SERIAL_PORT_DEBUG,s)
#define	SEND_SN_2_WIFI(s)		USART_OUT(SERIAL_PORT_DJI_SDK,s)

typedef enum
{
	DJI_PACKAGE_RECV_IDLE,
	DJI_PACKAGE_RECV_START,
	DJI_PACKAGE_RECV_COMMAND_SET,
	DJI_PACKAGE_RECV_COMMAND_ID,
	DJI_PACKAGE_RECV_WAIT_DONE
	
}DjiSDKPackageStatus_TypeEnum;

typedef enum
{
	WRITE_SN_PACKAGE_RECV_IDLE,
	WRITE_SN_PACKAGE_RECV_START

}WriteSnPackageStatus_TypeEnum;


typedef struct 
{
	DjiSDKPackageStatus_TypeEnum	DjiPackageStatus;					//数据包接收状态
	WriteSnPackageStatus_TypeEnum   WriteSnPackageStatus;
	u8  RxFlag;
	u8	RxIndex;														//计算接收个数
	u16 DataLen;
	u8	RxDataBuf[RX_MAX_NUM];
	u8	RxDataSecondBuf[RX_MAX_NUM];
	
	//u8  RecBuff[100] = {0};	
	//接收命令集 0x02 飞控外发的数据，包含0x00 标准数据包(时间戳、姿态、gps等等数据); 0x01 控制权归属切换;0x02 透传数据（移动设备至机载设备）
	//只接收透传的数据，不接收处理飞控发回的其他数据
	//即只接收 命令码为0x02 透传数据（移动设备至机载设备）
	//因此这个数组最大值只设定为100足够
	//u8  RecvIndex;
}UartTypedef;


extern	UartTypedef		Uart2;


void USART1_Config(USART_TypeDef* USARTx,u32 baud);
void USART2_Config(USART_TypeDef* USARTx,u32 baud);
void USART3_Config(USART_TypeDef* USARTx,u32 baud);
void UART4_Config(USART_TypeDef* USARTx,u32 baud);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
char *itoa(long value, char *string, int radix);
void USART_Send_Buf(USART_TypeDef* USARTx, u8* buf, u16 len);
void USART2_DJI_Process(u8 Rev);
void USART2_Write_SN_Process(u8 Recv);

#endif



