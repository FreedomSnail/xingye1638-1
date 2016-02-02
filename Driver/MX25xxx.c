/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name : MX25xxx.c
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


//#define TxBufferSize1   (countof(TxBuffer1) - 1)
//#define RxBufferSize1   (countof(TxBuffer1) - 1)

#define countof(a)   (sizeof(a) / sizeof(*(a)))

//unsigned char MX25_buffer[4096];



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
void GPIO_MX25_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	/* 配置 SPI2 引脚: SCK, MISO 和 MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 配置PB12为SST25VF016B的片选  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   
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
void wen(void){
	Select_Flash();
	SPI_Flash_SendByte(0x06);
	NotSelect_Flash();
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
void wdis(void){

	Select_Flash();
	SPI_Flash_SendByte(0x04); 
	NotSelect_Flash();
	wip();
	
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
void wsr(void){	
	Select_Flash();
	SPI_Flash_SendByte(0x50);
	NotSelect_Flash(); 
	Select_Flash();
	SPI_Flash_SendByte(0x01);
	SPI_Flash_SendByte(0x00); 
	NotSelect_Flash();
    wip();
}
/************************************************************************************************
** Function name :			
** Description :忙检测
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void wip(void){
	unsigned char a=1;
	while((a&0x01)==1) a=rdsr();	

}
/************************************************************************************************
** Function name :			
** Description :读状态寄存器
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
unsigned char rdsr(void){
	unsigned char busy;
	Select_Flash();
	SPI_Flash_SendByte(0x05);
	busy = SPI_Flash_ReadByte();
	NotSelect_Flash();
	return(busy);
	
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
void MX25_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize){
	unsigned int i=0; 	
	Select_Flash();
	SPI_Flash_SendByte(0x0b);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	SPI_Flash_SendByte(0);
	BlockSize=BlockSize;
	while(i<256){	
		readbuff[i]=SPI_Flash_ReadByte();
		
		i++;
	}
	NotSelect_Flash();	 	
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
void MX25_W_BLOCK(uint32_t addr, u8 *readbuff, uint16_t BlockSize){
	unsigned int i=0;
	BlockSize=BlockSize;
	sect_clr(addr);   								 
	wsr();
  	wen();	
	Select_Flash();    
	SPI_Flash_SendByte(0x02);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	while(i<256){
		SPI_Flash_SendByte(readbuff[i++]);
		SPI_Flash_SendByte(readbuff[i++]);
	}
	NotSelect_Flash();
	wip();
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
void MX25_Write(uint32_t addr, const u8 *readbuff, uint16_t NumByteToWrite)
{
	unsigned int i=0;
	if(NumByteToWrite == 0) {
		return ;
	}
//	sect_clr(addr);   								 
	wsr();
  	wen();	
	Select_Flash();    
	SPI_Flash_SendByte(0x02);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	while(i<NumByteToWrite){
		SPI_Flash_SendByte(readbuff[i++]);
	}
	NotSelect_Flash();
	wip();
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
void MX25_Read(unsigned long addr, unsigned char *readbuff, unsigned int NumByteToWrite)
{
	unsigned int i=0; 	
	Select_Flash();
	SPI_Flash_SendByte(0x0b);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	SPI_Flash_SendByte(0);
	while(i<NumByteToWrite){	
		readbuff[i++]=SPI_Flash_ReadByte();
	}
	NotSelect_Flash();	 	
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
void sect_clr(unsigned long a1){
	wsr();
	wen();     
	Select_Flash();	  
	SPI_Flash_SendByte(0x20);
	SPI_Flash_SendByte((a1&0xffffff)>>16);          //addh
	SPI_Flash_SendByte((a1&0xffff)>>8);          //addl 
	SPI_Flash_SendByte(a1&0xff);                 //wtt
	NotSelect_Flash();
	wip();
	
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
u8 FlashReadID(void)
{
	u8 fac_id,dev_id;
	Select_Flash();	
  	SPI_Flash_SendByte(0x90);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x01);
  	fac_id= SPI_Flash_ReadByte();		          //BFH: 工程码SST
	dev_id= SPI_Flash_ReadByte();	              //41H: 器件型号MX25L1606E    		
	dev_id = dev_id;	//防止编辑器警告
	 
  	NotSelect_Flash();	
  	return fac_id;
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
void SPI_Flash_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  /* 使能SPI2 时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
  /* SPI2配置 */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
  
  /* 使能SPI2  */
  SPI_Cmd(SPI2, ENABLE);   
  NotSelect_Flash();

  
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
u8 SPI_Flash_ReadByte(void)
{
  return (SPI_Flash_SendByte(Dummy_Byte));
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
u8 SPI_Flash_SendByte(u8 byte)
{
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI2, byte);
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPI2);
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
void Copy_System_Data_2_Flash(u32 DataTypeAddr)
{
	u8 buf[4];
	switch(DataTypeAddr) {
		case FLASH_ADDR_PRODUCT_NO:
			sect_clr(FLASH_ADDR_PRODUCT_NO);	
			MX25_Write(FLASH_ADDR_PRODUCT_NO, pumpBoardInfo.deviceSNStr, TXA_SN_LENTH);
			break;
		case FLASH_ADDR_PERMISSION:
			sect_clr(FLASH_ADDR_PERMISSION);	
			#if 0
			buf[0] = GprsCmd.permission/256/256/256;		//第一个8位,枚举变量占用四个字节
			buf[1] = GprsCmd.permission/256/256%256;		//第二个8位
			buf[2] = GprsCmd.permission/256%256;			//第三个8位
			buf[3] = GprsCmd.permission%256;				//第四个8位
			#endif
			switch(pumpBoardInfo.permission) {
				case PERMISSION_ALLOW:
					buf[0] = 0x00;
					break;
				case PERMISSION_PROHIBIT:
					buf[0] = 0xAA;
					break;
				default:
					break;
			}
			MX25_Write(FLASH_ADDR_PERMISSION, buf, 1);
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
** 设置产品编号，机身号码
编号格式:
TXA1509280000000
************************************************************************************************/
void Set_Product_Number(u8* sn, u8 Lenth)
{
	memcpy(pumpBoardInfo.deviceSNStr,sn,Lenth);
	Copy_System_Data_2_Flash(FLASH_ADDR_PRODUCT_NO);
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return : 0,号码正确，1错误
** Others :
** 获取产品编号，机身号码
编号格式:
TXA1509280000000
************************************************************************************************/
u8 Get_Product_Number(void)
{
	u8 i;
	MX25_Read(FLASH_ADDR_PRODUCT_NO, pumpBoardInfo.deviceSNStr,TXA_SN_LENTH);
	//检查格式是否正确
	if((pumpBoardInfo.deviceSNStr[0]!='T')||(pumpBoardInfo.deviceSNStr[1]!='X')||(pumpBoardInfo.deviceSNStr[2]!='A')) {
		return 1;
	}
	for(i=3;i<TXA_SN_LENTH;i++) {
		if((pumpBoardInfo.deviceSNStr[i]<'0')||(pumpBoardInfo.deviceSNStr[i]>'9')) {
			return 1;
		}
	}
	return 0;
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
void Set_Product_Permission(void)
{
	Copy_System_Data_2_Flash(FLASH_ADDR_PERMISSION);
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
void Get_Product_Permission(void)
{
	u8 FlashReadBuf[2];
	MX25_Read(FLASH_ADDR_PERMISSION, FlashReadBuf,1);
	if(FlashReadBuf[0]>0) {
		pumpBoardInfo.permission = PERMISSION_PROHIBIT;
	} else {
		pumpBoardInfo.permission = PERMISSION_ALLOW;
	}
}


