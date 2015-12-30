#ifndef _M100_SDK_H
#define _M100_SDK_H

#define PRO_DATA_MAX_SIZE				128 //TODO: for user define
#define PKG_MAX_SIZE					(PRO_DATA_MAX_SIZE + 16) //data,CRC32,header size

#define SESSION_AND_MEM_COUNT			2


#define _SDK_MAX_RECV_SIZE          (160)
#define _SDK_SOF                    ((unsigned char)(0xAA))
#define _SDK_CRC_HEAD_SIZE          (2)                 // CRC16
#define _SDK_CRC_DATA_SIZE          (4)                 // CRC32
#define _SDK_HEAD_DATA_LEN          (sizeof(SDKHeader) - 2)
#define _SDK_FULL_DATA_SIZE_MIN     (sizeof(SDKHeader) + _SDK_CRC_DATA_SIZE)

#define _SDK_U32_SET(_addr, _val)   (*((unsigned int*)(_addr)) = (_val))


typedef struct ProSendParameter
{
	unsigned short pkg_type : 2;
	unsigned short need_encrypt : 1;
	unsigned int length;
	unsigned char *buf;
}ProSendParameter;

typedef struct Memory_Manage_Unit
{
	unsigned char mmu_index;
	unsigned char usage_flag;
	unsigned short res;
	unsigned long start_addr;
	unsigned long end_addr;
}Memory_Manage_Unit;

typedef struct Session_Queue
{
	unsigned int session_id : 5;
	unsigned int usage_flag : 1;
	unsigned int sent_time : 5;
	unsigned int retry_send_time : 5;
	unsigned int ack_timeout : 16;
	Memory_Manage_Unit *mmu;
	unsigned int pre_seq_num;
	unsigned int pre_timestamp;
}Session_Queue;
typedef struct ProHeader
{
	unsigned int sof : 8; 			// 1byte
	unsigned int length : 10;
	unsigned int version : 6; 		// 2byte
	unsigned int session_id : 5;
	unsigned int is_ack : 1;
	unsigned int reversed0 : 2; 	// always 0

	unsigned int padding : 5;
	unsigned int enc_type : 3;
	unsigned int reversed1 : 24;

	unsigned int sequence_number : 16;
	unsigned int head_crc : 16;
	unsigned int magic[];
}ProHeader;

typedef struct
{
	unsigned int sof : 8; // 1byte

	unsigned int length : 10;
	unsigned int version : 6; // 2byte
	unsigned int session_id : 5;
	unsigned int is_ack : 1;
	unsigned int reversed0 : 2; // always 0

	unsigned int padding : 5;
	unsigned int enc_type : 3;
	unsigned int reversed1 : 24;

	unsigned int sequence_number : 16;
	unsigned int head_crc : 16;
	unsigned int magic[];
} SDKHeader;

typedef struct
{
	unsigned short reuse_index;
	unsigned short reuse_count;
	unsigned short recv_index;
	unsigned char comm_recv_buf[_SDK_MAX_RECV_SIZE];
	// for encrypt
    unsigned char         comm_key[32];
    unsigned char         enc_enabled;
} SDKFilter;

typedef struct
{
	unsigned int	app_id;
	unsigned int	app_api_level;
	unsigned int	app_ver;
	unsigned char		app_bundle_id[32];
}activation_data_t;

typedef struct ProAckParameter
{
	unsigned short session_id : 8;
	unsigned short need_encrypt : 8;
	unsigned short seq_num;
	unsigned int length;
	unsigned char *buf;
}ProAckParameter;

typedef struct ProFrameData_Unit
{
	unsigned char CommandSet;
	unsigned char CommandId;
	unsigned char data[80];
	unsigned short dataLen;
}ProFrameData_Unit;




extern unsigned char str[];
extern ProFrameData_Unit  DataFromMobile;



//uint16_t sdk_stream_crc16_calc(const uint8_t* pMsg, uint32_t nLen);

//uint32_t sdk_stream_crc32_calc(const uint8_t* pMsg, uint32_t nLen);
void DJI_Onboard__API_Activation_Init(void);
void DJI_Onboard_API_Activation(void);
void DJI_Onboard_send(void);
void Pro_Receive_Interface(void);

#endif
