#ifndef _MX25xxx_H
#define _MX25xxx_H



#define Dummy_Byte 0

/* Select SPI FLASH: ChipSelect pin low  */
#define Select_Flash()     GPIO_ResetBits(GPIOB, GPIO_Pin_12)
/* Deselect SPI FLASH: ChipSelect pin high */
#define NotSelect_Flash()    GPIO_SetBits(GPIOB, GPIO_Pin_12)


#define	SECTOR_SIZE			4096
#define	PAGE_SIZE			256



#define	FLASH_ADDR_PRODUCT_NO					(SECTOR_SIZE*19)		//机身编号存储地址
#define	FLASH_ADDR_PERMISSION					(SECTOR_SIZE*24)		//工作权限







void GPIO_MX25_Config(void);
void SPI_Flash_Init(void);
u8 SPI_Flash_ReadByte(void);
u8 SPI_Flash_SendByte(u8 byte);

void wip(void);
void wen(void);
void wdis(void);
void wsr(void);
unsigned char rdsr(void);
u8 FlashReadID(void);
void sect_clr(unsigned long a1);  
void MX25_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize);
void MX25_W_BLOCK(uint32_t addr, u8 *readbuff, uint16_t BlockSize);
void MX25_Write(uint32_t addr, const u8 *readbuff, uint16_t NumByteToWrite);
void MX25_Read(unsigned long addr, unsigned char *readbuff, unsigned int NumByteToWrite);
void SPI_Flash_Init(void);
void Set_Product_Number(u8* sn, u8 Lenth);
u8 Get_Product_Number(void);
void Set_Product_Permission(void);
void Get_Product_Permission(void);

#endif

