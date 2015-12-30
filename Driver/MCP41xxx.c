/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name :MCP41xxx.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : ף��һ
** Created date :2015��11��23��14:44:02
** Version :V1.1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :ף��һ
** Modified date :2013��4��21��9:16:19
** Version :
** Description :
************************************************************************************************/
#include "includes.h"
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** �ٸ��غͿɿع���ƹܽ�
cs  -- PB6
sck -- PB3
si  -- PB4
************************************************************************************************/
void GPIO_MCP41xxx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO , ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_InitStructure.GPIO_Pin = MCP41XXX_CS_PIN|MCP41XXX_SCK_PIN|MCP41XXX_SI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(MCP41XXX_PORT, &GPIO_InitStructure);

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
void MCP41xxx_Delay(void)
{
	u16 i;
	for(i=0;i<120;i++) {
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
** �������ֵ�λ��
************************************************************************************************/
void MCP41xxx_Write_RES(u8 dat1)
{
	u8 i;
	u8 command=0x11;
	MCP41XXX_CS_L;
	MCP41xxx_Delay(); 
	MCP41XXX_SCK_L;
	MCP41xxx_Delay(); 
	for(i=8;i>0;i--) {//д����
		if((command&0x80) == 0x80) {
			MCP41XXX_SI_H;
		} else {
			MCP41XXX_SI_L;
		}
		command <<= 1; 
		MCP41xxx_Delay();
		MCP41XXX_SCK_H;
		MCP41xxx_Delay();
		MCP41XXX_SCK_L;
		MCP41xxx_Delay();
	}
	for(i=8;i>0;i--) {//д����
		if((dat1&0x80) == 0x80) {
			MCP41XXX_SI_H;
		} else {
			MCP41XXX_SI_L;
		}
	 	dat1 <<= 1;
		MCP41xxx_Delay();
		MCP41XXX_SCK_H;
		MCP41xxx_Delay();
		MCP41XXX_SCK_L;
		MCP41xxx_Delay();
	}
	MCP41XXX_CS_H;
	MCP41xxx_Delay();
}



