/****************************************Copyright(c)********************************************
**
**                            2015-2023, Guangzhou TX-Aviation Technology Co.Ltd.
**
**----------------------------------------File Info----------------------------------------------
** File name : m100_sdk.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : Luzhi
** Created date :2015-8-13 16:19:42
** Version :V1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :
** Modified date :
** Version :
** Description :
************************************************************************************************/
#include "includes.h"
unsigned char Send_Global_Common_Memory[PKG_MAX_SIZE];
SDKFilter serial_sdk = { 0 };

static Session_Queue Send_Session_Common_Tab;
static Session_Queue Send_Session_Tab[SESSION_AND_MEM_COUNT];
static Memory_Manage_Unit Send_MMU_Tab[SESSION_AND_MEM_COUNT];
static unsigned char Send_Global_Memory[SESSION_AND_MEM_COUNT * PKG_MAX_SIZE];


ProFrameData_Unit  DataFromMobile;
unsigned char Activation_Ack = 0;//激活成功为1


static const char *key;

typedef struct tagAES256Context {
    unsigned char key[32];
    unsigned char enckey[32];
    unsigned char deckey[32];
} aes256_context;
#define F(x)   (((x)<<1) ^ ((((x)>>7) & 1) * 0x1b))
#define FD(x)  (((x) >> 1) ^ (((x) & 1) ? 0x8d : 0))
#define BACK_TO_TABLES

#ifdef BACK_TO_TABLES

const unsigned char sbox[256] ={
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};
const unsigned char sboxinv[256] ={
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
    0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
    0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
    0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
    0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
    0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
    0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
    0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
    0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
    0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
    0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
    0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
    0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
    0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
    0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
    0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

#define rj_sbox(x)     sbox[(x)]
#define rj_sbox_inv(x) sboxinv[(x)]

#else /* tableless subroutines */

/* -------------------------------------------------------------------------- */
unsigned char gf_alog(unsigned char x) // calculate anti-logarithm gen 3
{
    unsigned char atb = 1, z;

    while (x--) { z = atb; atb <<= 1; if (z & 0x80) atb ^= 0x1b; atb ^= z; }

    return atb;
} /* gf_alog */

/* -------------------------------------------------------------------------- */
unsigned char gf_log(unsigned char x) // calculate logarithm gen 3
{
    unsigned char atb = 1, i = 0, z;

    do {
        if (atb == x) break;
        z = atb; atb <<= 1; if (z & 0x80) atb ^= 0x1b; atb ^= z;
    } while (++i > 0);

    return i;
} /* gf_log */


/* -------------------------------------------------------------------------- */
unsigned char gf_mulinv(unsigned char x) // calculate multiplicative inverse
{
    return (x) ? gf_alog(255 - gf_log(x)) : 0;
} /* gf_mulinv */

/* -------------------------------------------------------------------------- */
unsigned char rj_sbox(unsigned char x)
{
    unsigned char y, sb;

    sb = y = gf_mulinv(x);
    y = (y << 1) | (y >> 7); sb ^= y;  y = (y << 1) | (y >> 7); sb ^= y;
    y = (y << 1) | (y >> 7); sb ^= y;  y = (y << 1) | (y >> 7); sb ^= y;

    return (sb ^ 0x63);
} /* rj_sbox */

/* -------------------------------------------------------------------------- */
unsigned char rj_sbox_inv(unsigned char x)
{
    unsigned char y, sb;

    y = x ^ 0x63;
    sb = y = (y << 1) | (y >> 7);
    y = (y << 2) | (y >> 6); sb ^= y; y = (y << 3) | (y >> 5); sb ^= y;

    return gf_mulinv(sb);
} /* rj_sbox_inv */

#endif

#if 1
const unsigned short CRC_INIT = 0x3AA3;

const unsigned short crc_tab16[] =
{
	0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
	0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
	0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
	0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
	0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
	0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
	0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
	0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
	0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
	0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
	0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
	0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
	0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
	0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
	0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
	0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
	0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
	0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
	0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
	0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
	0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
	0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
	0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
	0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
	0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
	0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
	0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
	0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
	0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
	0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
	0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
	0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040,
};

const unsigned int crc_tab32[] =
{
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};
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
unsigned short crc16_update(unsigned short crc, unsigned char ch)
{
	unsigned short tmp;
	unsigned short msg;

	msg = 0x00ff & (unsigned short)ch;
	tmp = crc       ^ msg;
	crc = (crc >> 8) ^ crc_tab16[tmp & 0xff];

	return crc;
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
unsigned int crc32_update(unsigned int crc, unsigned char ch)
{
	unsigned int tmp;
	unsigned int msg;
	
	msg = 0x000000ffL & (unsigned int)ch;
	tmp = crc		^ msg;
	crc = (crc >> 8) ^ crc_tab32[tmp & 0xff];
	return crc;
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
unsigned short sdk_stream_crc16_calc(const unsigned char* pMsg, unsigned int nLen)
{
	unsigned int i;
	unsigned short wCRC = CRC_INIT;

	for (i = 0; i < nLen; i++)
	{
		wCRC = crc16_update(wCRC, pMsg[i]);
	}

	return wCRC;
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
unsigned int sdk_stream_crc32_calc(const unsigned char* pMsg, unsigned int nLen)
{
	unsigned int i;
	unsigned int wCRC = CRC_INIT;

	for (i = 0; i < nLen; i++)
	{
		wCRC = crc32_update(wCRC, pMsg[i]);
	}

	return wCRC;
}

#endif
/* -------------------------------------------------------------------------- */
unsigned char rj_xtime(unsigned char x)
{
    return (x & 0x80) ? ((x << 1) ^ 0x1b) : (x << 1);
} /* rj_xtime */

/* -------------------------------------------------------------------------- */
void aes_subBytes(unsigned char *buf)
{
    register unsigned char i = 16;

    while (i--) buf[i] = rj_sbox(buf[i]);
} /* aes_subBytes */

/* -------------------------------------------------------------------------- */
void aes_subBytes_inv(unsigned char *buf)
{
    register unsigned char i = 16;

    while (i--) buf[i] = rj_sbox_inv(buf[i]);
} /* aes_subBytes_inv */

/* -------------------------------------------------------------------------- */
void aes_addRoundKey(unsigned char *buf, unsigned char *key)
{
    register unsigned char i = 16;

    while (i--) buf[i] ^= key[i];
} /* aes_addRoundKey */

/* -------------------------------------------------------------------------- */
void aes_addRoundKey_cpy(unsigned char *buf, unsigned char *key, unsigned char *cpk)
{
    register unsigned char i = 16;

    while (i--)  buf[i] ^= (cpk[i] = key[i]), cpk[16 + i] = key[16 + i];
} /* aes_addRoundKey_cpy */


/* -------------------------------------------------------------------------- */
void aes_shiftRows(unsigned char *buf)
{
    register unsigned char i, j; /* to make it potentially parallelable :) */

    i = buf[1]; buf[1] = buf[5]; buf[5] = buf[9]; buf[9] = buf[13]; buf[13] = i;
    i = buf[10]; buf[10] = buf[2]; buf[2] = i;
    j = buf[3]; buf[3] = buf[15]; buf[15] = buf[11]; buf[11] = buf[7]; buf[7] = j;
    j = buf[14]; buf[14] = buf[6]; buf[6] = j;

} /* aes_shiftRows */

/* -------------------------------------------------------------------------- */
void aes_shiftRows_inv(unsigned char *buf)
{
    register unsigned char i, j; /* same as above :) */

    i = buf[1]; buf[1] = buf[13]; buf[13] = buf[9]; buf[9] = buf[5]; buf[5] = i;
    i = buf[2]; buf[2] = buf[10]; buf[10] = i;
    j = buf[3]; buf[3] = buf[7]; buf[7] = buf[11]; buf[11] = buf[15]; buf[15] = j;
    j = buf[6]; buf[6] = buf[14]; buf[14] = j;

} /* aes_shiftRows_inv */

/* -------------------------------------------------------------------------- */
void aes_mixColumns(unsigned char *buf)
{
    register unsigned char i, a, b, c, d, e;

    for (i = 0; i < 16; i += 4)
    {
        a = buf[i]; b = buf[i + 1]; c = buf[i + 2]; d = buf[i + 3];
        e = a ^ b ^ c ^ d;
        buf[i] ^= e ^ rj_xtime(a^b);   buf[i + 1] ^= e ^ rj_xtime(b^c);
        buf[i + 2] ^= e ^ rj_xtime(c^d); buf[i + 3] ^= e ^ rj_xtime(d^a);
    }
} /* aes_mixColumns */
/* -------------------------------------------------------------------------- */
void aes_mixColumns_inv(unsigned char *buf)
{
    register unsigned char i, a, b, c, d, e, x, y, z;

    for (i = 0; i < 16; i += 4)
    {
        a = buf[i]; b = buf[i + 1]; c = buf[i + 2]; d = buf[i + 3];
        e = a ^ b ^ c ^ d;
        z = rj_xtime(e);
        x = e ^ rj_xtime(rj_xtime(z^a^c));  y = e ^ rj_xtime(rj_xtime(z^b^d));
        buf[i] ^= x ^ rj_xtime(a^b);   buf[i + 1] ^= y ^ rj_xtime(b^c);
        buf[i + 2] ^= x ^ rj_xtime(c^d); buf[i + 3] ^= y ^ rj_xtime(d^a);
    }
} /* aes_mixColumns_inv */

/* -------------------------------------------------------------------------- */
void aes_expandEncKey(unsigned char *k, unsigned char *rc)
{
    register unsigned char i;

    k[0] ^= rj_sbox(k[29]) ^ (*rc);
    k[1] ^= rj_sbox(k[30]);
    k[2] ^= rj_sbox(k[31]);
    k[3] ^= rj_sbox(k[28]);
    *rc = F(*rc);

    for (i = 4; i < 16; i += 4)  k[i] ^= k[i - 4], k[i + 1] ^= k[i - 3],
        k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];
    k[16] ^= rj_sbox(k[12]);
    k[17] ^= rj_sbox(k[13]);
    k[18] ^= rj_sbox(k[14]);
    k[19] ^= rj_sbox(k[15]);

    for (i = 20; i < 32; i += 4) k[i] ^= k[i - 4], k[i + 1] ^= k[i - 3],
        k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];

} /* aes_expandEncKey */

/* -------------------------------------------------------------------------- */
void aes_expandDecKey(unsigned char *k, unsigned char *rc)
{
    unsigned char i;

    for (i = 28; i > 16; i -= 4) k[i + 0] ^= k[i - 4], k[i + 1] ^= k[i - 3],
        k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];

    k[16] ^= rj_sbox(k[12]);
    k[17] ^= rj_sbox(k[13]);
    k[18] ^= rj_sbox(k[14]);
    k[19] ^= rj_sbox(k[15]);

    for (i = 12; i > 0; i -= 4)  k[i + 0] ^= k[i - 4], k[i + 1] ^= k[i - 3],
        k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];

    *rc = FD(*rc);
    k[0] ^= rj_sbox(k[29]) ^ (*rc);
    k[1] ^= rj_sbox(k[30]);
    k[2] ^= rj_sbox(k[31]);
    k[3] ^= rj_sbox(k[28]);
} /* aes_expandDecKey */

/* -------------------------------------------------------------------------- */
void aes256_init(aes256_context *ctx, unsigned char *k)
{
    unsigned char rcon = 1;
    register unsigned char i;

    for (i = 0; i < sizeof(ctx->key); i++) ctx->enckey[i] = ctx->deckey[i] = k[i];
    for (i = 8; --i;) aes_expandEncKey(ctx->deckey, &rcon);
} /* aes256_init */

/* -------------------------------------------------------------------------- */
void aes256_done(aes256_context *ctx)
{
    register unsigned char i;

    for (i = 0; i < sizeof(ctx->key); i++)
        ctx->key[i] = ctx->enckey[i] = ctx->deckey[i] = 0;
} /* aes256_done */




/* -------------------------------------------------------------------------- */
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
void aes256_encrypt_ecb(aes256_context *ctx, unsigned char *buf)
{
    unsigned char i, rcon;

    aes_addRoundKey_cpy(buf, ctx->enckey, ctx->key);
    for (i = 1, rcon = 1; i < 14; ++i)
    {
        aes_subBytes(buf);
        aes_shiftRows(buf);
        aes_mixColumns(buf);
        if (i & 1) aes_addRoundKey(buf, &ctx->key[16]);
        else aes_expandEncKey(ctx->key, &rcon), aes_addRoundKey(buf, ctx->key);
    }
    aes_subBytes(buf);
    aes_shiftRows(buf);
    aes_expandEncKey(ctx->key, &rcon);
    aes_addRoundKey(buf, ctx->key);
} /* aes256_encrypt */

/* -------------------------------------------------------------------------- */
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
void aes256_decrypt_ecb(aes256_context *ctx, unsigned char *buf)
{
    unsigned char i, rcon;

    aes_addRoundKey_cpy(buf, ctx->deckey, ctx->key);
    aes_shiftRows_inv(buf);
    aes_subBytes_inv(buf);

    for (i = 14, rcon = 0x80; --i;)
    {
        if ((i & 1))
        {
            aes_expandDecKey(ctx->key, &rcon);
            aes_addRoundKey(buf, &ctx->key[16]);
        }
        else aes_addRoundKey(buf, ctx->key);
        aes_mixColumns_inv(buf);
        aes_shiftRows_inv(buf);
        aes_subBytes_inv(buf);
    }
    aes_addRoundKey(buf, ctx->key);
} /* aes256_decrypt */
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
typedef void(*ptr_aes256_codec)(aes256_context *ctx, unsigned char *buf);
void sdk_stream_codec(SDKFilter* p_filter, SDKHeader* p_head, ptr_aes256_codec codec_func)
{
    aes256_context ctx;
    unsigned int buf_i;
    unsigned int loop_blk;
    unsigned int data_len;
    unsigned int data_idx;
    unsigned char* data_ptr;

    if (p_head->enc_type == 0)
        return;
    if (p_head->length == sizeof(SDKHeader))
        return;
    if (p_head->length <= sizeof(SDKHeader) + _SDK_CRC_DATA_SIZE)
        return;

    data_ptr = (unsigned char*)p_head + sizeof(SDKHeader);
    data_len = p_head->length - _SDK_CRC_DATA_SIZE - sizeof(SDKHeader);
    loop_blk = data_len / 16;
    data_idx = 0;

    aes256_init(&ctx, p_filter->comm_key);
    for (buf_i = 0; buf_i < loop_blk; buf_i++)
    {
        codec_func(&ctx, data_ptr + data_idx);
        data_idx += 16;
    }
    aes256_done(&ctx);

    if(codec_func == aes256_decrypt_ecb)
    	p_head->length = p_head->length - p_head->padding; //minus padding length;
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
void sdk_stream_recalc_crc(void* p_data)
{
	SDKHeader* p_head = (SDKHeader*)p_data;
	unsigned char* p_byte = (unsigned char*)p_data;
	unsigned int index_of_crc2;
	unsigned int temp;

	if (p_head->sof != _SDK_SOF) return;
	if (p_head->version != 0) return;
	if (p_head->length > _SDK_MAX_RECV_SIZE) return;
	if (p_head->length > sizeof(SDKHeader) && p_head->length < _SDK_FULL_DATA_SIZE_MIN) return;
	
	p_head->head_crc = sdk_stream_crc16_calc(p_byte, _SDK_HEAD_DATA_LEN);
	
	if (p_head->length >= _SDK_FULL_DATA_SIZE_MIN)
	{
		index_of_crc2 = p_head->length - _SDK_CRC_DATA_SIZE;
		
		//_SDK_U32_SET(p_byte + index_of_crc2, sdk_stream_crc32_calc(p_byte, index_of_crc2));
		//*(p_byte + index_of_crc2- 8) = 0x48;
		//*(p_byte + index_of_crc2- 7) = 0x2d;
		temp = sdk_stream_crc32_calc(p_byte, index_of_crc2);
		*(p_byte + index_of_crc2) = temp;
		*(p_byte + index_of_crc2+1) = temp>>8;
		*(p_byte + index_of_crc2+2) = temp>>16;
		*(p_byte + index_of_crc2+3) = temp>>24;;
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
unsigned short sdk_encrypt_interface(unsigned char *pdest, const unsigned char *psrc,
		unsigned short w_len,unsigned char is_ack,unsigned char is_enc,unsigned char session_id,
		unsigned short seq_num)
{
	unsigned short data_len;

	SDKHeader* p_head = (SDKHeader*)pdest;
	
    if (w_len > 1024) {
        return 0;
    }
	
    if (w_len == 0 || psrc == 0)
        data_len = (unsigned short)sizeof(SDKHeader);
    else
        data_len = (unsigned short)sizeof(SDKHeader) + _SDK_CRC_DATA_SIZE + w_len;

    if(is_enc)
    	data_len = data_len + (16 - w_len % 16);

    p_head->sof = _SDK_SOF;
    p_head->length = data_len;
    p_head->version = 0;
    p_head->session_id = session_id;
    p_head->is_ack = is_ack ? 1 : 0;
    p_head->reversed0 = 0;

    p_head->padding = is_enc ? (16 - w_len % 16) : 0;
    p_head->enc_type = is_enc ? 1 : 0;
    p_head->reversed1 = 0;

    p_head->sequence_number = seq_num;
    p_head->head_crc = 0;

    if (psrc && w_len)
        memcpy(pdest + sizeof(SDKHeader), psrc, w_len);
	
    sdk_stream_codec(&serial_sdk, p_head, aes256_encrypt_ecb);
	

    sdk_stream_recalc_crc(pdest);

	return data_len;
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
Memory_Manage_Unit *Request_Send_MMU(unsigned short size)
{
	unsigned char i;
	Memory_Manage_Unit *p2mmu ;

	for(i = 0 ; i < SESSION_AND_MEM_COUNT; i ++)
	{
		if(Send_MMU_Tab[i].usage_flag == 0)
		{
			p2mmu = &Send_MMU_Tab[i];
			//p2mmu->usage_flag = 1;
			p2mmu->end_addr = p2mmu->start_addr + size;
			break;
		}
	}

	return p2mmu;
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
Session_Queue * Request_Send_Session(unsigned short size)
{
	int i;
	Session_Queue *p2session=NULL;
	Memory_Manage_Unit *p2mmu=NULL;

	for(i = 0 ; i < SESSION_AND_MEM_COUNT ; i ++)
	{
		if(Send_Session_Tab[i].usage_flag == 0)
		{
			//Send_Session_Tab[i].usage_flag = 1;
			p2session = &Send_Session_Tab[i];
			break;
		}
	}

	p2mmu = Request_Send_MMU(size);
	if(p2mmu == NULL)
	{
		p2session = NULL;
	}
	else
	{
		p2session->mmu = p2mmu;
	}
	return p2session;
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
void Free_Send_MMU(Memory_Manage_Unit *mmu)
{

	if(mmu->usage_flag == 1)
	{
		mmu->usage_flag = 0;
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
void Free_Send_Session(Session_Queue * session)
{
	if(session->usage_flag == 1)
	{
		Free_Send_MMU(session->mmu);
		session->usage_flag = 0;
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
static void Send_Pro_Data(unsigned char *buf)
{
	ProHeader *pHeader = (ProHeader *)buf;
	USART_Send_Buf(USART2,buf,pHeader->length);
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
int Pro_Send_Interface(ProSendParameter *parameter)
{
	unsigned short ret = 0;
	static unsigned short global_seq_num = 0;
  	Session_Queue *p2session = NULL;
	if(parameter->length > PRO_DATA_MAX_SIZE)
	{
		return -1;
	}
	switch(parameter->pkg_type){//包类型
	#if 0
	case 0:
		ret = sdk_encrypt_interface(Send_Global_Common_Memory,parameter->buf,parameter->length,
				0,parameter->need_encrypt,0,global_seq_num ++);
		if(ret == 0)
		{
			return -1;
		}
		Send_Pro_Data(Send_Global_Common_Memory);
		break;
	case 1:
		if(global_seq_num == Send_Session_Common_Tab.pre_seq_num)
		{
			global_seq_num ++;
		}
		Send_Session_Common_Tab.pre_seq_num = global_seq_num;

		ret = sdk_encrypt_interface(Send_Global_Common_Memory,parameter->buf,parameter->length,
				0,parameter->need_encrypt,1,global_seq_num ++);

		if(ret == 0)
		{
			return -1;
		}

		Send_Pro_Data(Send_Global_Common_Memory);
		break;
		#endif
	case 2:
		
		p2session = Request_Send_Session(parameter->length + sizeof(ProHeader) + 4);
		
		if(p2session)
		{	
			if(global_seq_num == p2session->pre_seq_num)
			{
				global_seq_num ++;
			}
			p2session->pre_seq_num = global_seq_num;
			ret = sdk_encrypt_interface((unsigned char*)p2session->mmu->start_addr,
					parameter->buf,parameter->length,0,parameter->need_encrypt,
					p2session->session_id,global_seq_num ++);
			
			if(ret == 0)
			{
				Free_Send_Session(p2session);
				return -1;
			} 
			Send_Pro_Data((unsigned char*)p2session->mmu->start_addr);
			ret = 0;
		}
		break;
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
static activation_data_t activation_msg = {14,2,0x02030A00,""};
unsigned char str[50];

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
void Pro_Link_Setup(void)
{
	unsigned int i;
	Send_Session_Common_Tab.pre_seq_num = Send_Session_Common_Tab.pre_seq_num;//防止编译警告
	for(i = 0; i < SESSION_AND_MEM_COUNT; i ++)
	{
		Send_Session_Tab[i].session_id = i + 2;
		Send_Session_Tab[i].usage_flag = 0;
		Send_Session_Tab[i].pre_seq_num = 0x10000;

		Send_MMU_Tab[i].mmu_index = i;
		Send_MMU_Tab[i].usage_flag = 0;
		Send_MMU_Tab[i].start_addr = (unsigned long)&Send_Global_Memory[i * PKG_MAX_SIZE];
	}
	Send_Session_Common_Tab.usage_flag = 0;
	Send_Session_Common_Tab.session_id = 1;
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
static void sdk_transform2byte(const char *pstr,unsigned char *pdata)
{
	int i;
	char temp_area[3];
	unsigned int temp8;
	temp_area[0] = temp_area[1] = temp_area[2] = 0;

	for(i = 0 ; i < 32; i++)
	{
		temp_area[0] = pstr[0];
		temp_area[1] = pstr[1];
		sscanf(temp_area,"%x",&temp8);
		pdata[i] = temp8;
		pstr += 2;
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
void sdk_comm_set_key(SDKFilter* p_filter, const char* sz_key)
{
    sdk_transform2byte(sz_key,p_filter->comm_key);
    p_filter->enc_enabled = 1;
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
void sdk_set_encrypt_key_interface(const char* sz_key)
{
	sdk_comm_set_key(&serial_sdk,sz_key);
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
void Pro_Config_Comm_Encrypt_Key(const char *key)
{
	sdk_set_encrypt_key_interface(key);
}
#define MAKE_VERSION(a,b,c,d) (((a << 24)&0xff000000) | ((b << 16)&0x00ff0000) | ((c << 8)&0x0000ff00) | (d&0x000000ff))
#define SDK_VERSION           (MAKE_VERSION(3,1,100,0))




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
void DJI_Onboard_API_Activation_Init(void)
{

	activation_msg.app_id =1008902;
	activation_msg.app_api_level = 2;
	//activation_msg.app_ver = 50357248;
	activation_msg.app_ver = SDK_VERSION;
	
	memcpy(activation_msg.app_bundle_id,"1234567890123456789012", 32);
	key = "818e4fccacb5d597aa4c006a15b7b031185a49ec3f86aa50a023b00d04146a9c";   
	       
	Pro_Config_Comm_Encrypt_Key(key);
	Pro_Link_Setup();
}
static unsigned char Pro_Encode_Data[120];
#define SET_CMD_SIZE	(2u)
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
void App_Send_Data(unsigned char flag, unsigned char is_enc, unsigned char  cmd_set, unsigned char cmd_id,unsigned char *pdata,int len)
{
	ProSendParameter param;
	unsigned char *ptemp = (unsigned char *)Pro_Encode_Data;
	*ptemp++ = cmd_set;
	*ptemp++ = cmd_id;

	memcpy(Pro_Encode_Data + SET_CMD_SIZE,pdata,len);

	param.pkg_type = flag;
	param.length = len + SET_CMD_SIZE;
	param.buf = Pro_Encode_Data;
	param.need_encrypt = is_enc;
	
	Pro_Send_Interface(&param);
}
#define MY_ACTIVATION_SET	0x00
#define API_VERSION_QUERY	0x00
#define API_USER_ACTIVATION	0x01
#define API_INFO_QUERY		0x02
#define	API_SIM_ECHO		0xFF

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
void DJI_Onboard_API_Activation(void)
{
	App_Send_Data( 2, 0, MY_ACTIVATION_SET, API_USER_ACTIVATION,(unsigned char*)&activation_msg,sizeof(activation_msg));
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
void DJI_Onboard_send(void)
{
	App_Send_Data( 2, 0, MY_ACTIVATION_SET,0xFE,(unsigned char*)&str,9);
}

void Pro_Receive_Interface(void)
{
	u8 Msg;
  	int Heard_CRC32=0;
	ProFrameData_Unit  RecData;
	SDKHeader RecHeader ;
	memcpy(&RecHeader, Uart2.RxDataBuf,sizeof(SDKHeader));
	
	if (RecHeader.sof != _SDK_SOF) return;
	if (RecHeader.version != 0) return;
	if (RecHeader.length > _SDK_MAX_RECV_SIZE) return;
	if (RecHeader.length > sizeof(SDKHeader) && RecHeader.length < _SDK_FULL_DATA_SIZE_MIN) return;
	
	if(sdk_stream_crc16_calc((unsigned char*)&RecHeader, _SDK_HEAD_DATA_LEN)!=RecHeader.head_crc) return;//校验帧头
	
	memcpy(&Heard_CRC32,&Uart2.RxDataBuf[RecHeader.length - _SDK_CRC_DATA_SIZE] ,_SDK_CRC_DATA_SIZE);
	if(sdk_stream_crc32_calc((unsigned char*)Uart2.RxDataBuf, RecHeader.length - _SDK_CRC_DATA_SIZE)!=Heard_CRC32) return;//整体校验
	
	RecData.CommandSet = Uart2.RxDataBuf[12];
	RecData.CommandId  = Uart2.RxDataBuf[13];
	RecData.dataLen = RecHeader.length - _SDK_FULL_DATA_SIZE_MIN-2;//透传数据的长度
	if(RecData.dataLen>0) {
		memcpy(&RecData.data , &Uart2.RxDataBuf[14],RecData.dataLen);
	}
	
	memset((unsigned char*)&DataFromMobile,0,sizeof(DataFromMobile));
	memcpy((unsigned char*)&DataFromMobile,(unsigned char*)&RecData,sizeof(RecData));
	
#if 1
	if(RecHeader.is_ack) {
		if((RecData.CommandSet == 0)&&(RecData.CommandId == 0)) {//激活应答
			OSSemPost(SemDjiActivation);
			//LOG_DJI("ACK Activation!\r\n");
		} else if((RecData.CommandSet == 0x01)&&(RecData.CommandId == 0)) {
			//LOG_DJI("ACK release!\r\n");
		} else if((RecData.CommandSet == 0x02)&&(RecData.CommandId == 0)) {
			//LOG_DJI("ACK obtain!\r\n");
		}
		// 0x0000：拒绝获取控制权(未满足获取控制权条件)
		// 0x0001：成功释放控制权
		// 0x0002：成功获得控制权
		// 0x0003：正在获取控制权	
	}else {
		switch(RecData.CommandSet) {
		#if 0
			case 0x01://命令集 控制命令类
				LOG_DJI_STR("Control data!\r\n");
				switch(RecData.CommandId) {
					case 0x00:	//请求获取/释放控制权(飞控至机载设备)
													//返回码
													// 0x0000：拒绝获取控制权(未满足获取控制权条件)
													// 0x0001：成功释放控制权
													// 0x0002：成功获得控制权
													// 0x0003：正在获取控制权		
						switch(RecData.data[0]) {
							case 0x00:
								break;
							case 0x01:
								break;
							case 0x02:
								break;
							case 0x03:
								break;
							default:
								break;
						}
						//USART_Send_Buf(SERIAL_PORT_DEBUG,DataFromDji.data,DataFromDji.dataLen);
						
						break;
					case 0x01:	//切换飞行状态(飞控至机载设备)
						
						break;
					case 0x02:	//查询飞行状态切换结果(飞控至机载设备)
						
						break;
					default:
						break;
				}
				break;
		#endif
			case 0x02://命令集 推送数据类,飞控==>机载设备
				switch(RecData.CommandId) {
					case 0x00:	//飞行数据
						//LOG_DJI("Flight data!\r\n");
						break;
					case 0x02:	//移动设备至机载设备
						LOG_DJI("Mobile data!\r\n");
						if(DataFromMobile.data[0] == '1') {
							Msg = '1';
							OSQPost(UsartDjiCtrlPumpQsem,(void *)&Msg);
						} else if((DataFromMobile.data[0] == '0')){
							Msg = '0';
							OSQPost(UsartDjiCtrlPumpQsem,(void *)&Msg);
						}
						//USART_Send_Buf(SERIAL_PORT_DEBUG,DataFromDji.data,DataFromDji.dataLen);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
#endif
	#if 0
	if(RecHeader.is_ack)
	{
		ProAckParameter Ack;
		Ack.session_id = RecHeader.session_id;
		Ack.seq_num = RecHeader.sequence_number;
		Ack.length = RecHeader.length - _SDK_FULL_DATA_SIZE_MIN;
		Ack.need_encrypt = RecHeader.enc_type;
		
		
		switch(Ack.session_id)
		{
			case 0x02:
				if(Ack.seq_num==0)
				{
					unsigned short temp = (unsigned short)Uart2.RxDataBuf[13]<<Uart2.RxDataBuf[12];
					if(temp==0)
						Activation_Ack = 1;
				}
				break;
//			case 0x01:
//				//if(RecHeader.sequence_number==Send_Session_Tab[1].pre_seq_num)
//				{
//					unsigned short temp = (unsigned short)RecBuff[13]<<8||(unsigned short)RecBuff[12];
//					if(temp==0x0002)
//						GetControl_Ack = 1;
//				}
//				break;
		}
		
		RecData.CommandSet = Uart2.RxDataBuf[12];
		RecData.CommandId = Uart2.RxDataBuf[13];
		RecData.dataLen = RecHeader.length - _SDK_FULL_DATA_SIZE_MIN-2;//透传数据的长度
		if(RecData.dataLen>0) {
			memcpy(&RecData.data , &Uart2.RxDataBuf[14],RecData.dataLen);
		}
		memcpy((unsigned char*)&DataFromMobile,(unsigned char*)&RecData,sizeof(RecData));
		
	}else{
		RecData.CommandSet = Uart2.RxDataBuf[12];
		RecData.CommandId = Uart2.RxDataBuf[13];
		RecData.dataLen = RecHeader.length - _SDK_FULL_DATA_SIZE_MIN-2;//透传数据的长度
		if(RecData.dataLen>0) {
			memcpy(&RecData.data , &Uart2.RxDataBuf[14],RecData.dataLen);
		}
		
		//RecData.readed = 0;  //数据为读取
		
		switch(RecData.CommandSet) {
			//case 0x00://激活验证类
			//	if(RecData.CommandId==0x01) { //激活应答(飞控至机载设备)
			//		memcpy((unsigned char*)&DataFromMobile,(unsigned char*)&RecData,sizeof(RecData));
			//	}
			//	break;
			case 0x02://推送数据类
				if(RecData.CommandId==0x02) {	//移动设备至机载设备
					memset((unsigned char*)&DataFromMobile,0,sizeof(DataFromMobile));
					memcpy((unsigned char*)&DataFromMobile,(unsigned char*)&RecData,sizeof(RecData));
					//USART_Send_Buf(USART1,DataFromMobile.data,RecData.dataLen);
					//App_Send_Data( 2, 0, MY_ACTIVATION_SET,0xFE,DataFromMobile.data,RecData.dataLen);
				}
				break;
			default:
				break;
		}
	}
	#endif
}


