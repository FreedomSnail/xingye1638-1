#ifndef _SIM900A_H
#define _SIM900A_H 



#define		GSM_RCV_MAX_NUM				50

#define		CIPSEND_END_CHAR			0x1A	//→
#define 	TXA_SN_LENTH		16
#define 	GPRS_BASE_STATION_LONGITUDE_LENTH		10	
#define 	GPRS_BASE_STATION_LATITUDE_LENTH		9	
#define 	GPRS_CMD_SEND_LENTH						(TXA_SN_LENTH+GPRS_BASE_STATION_LONGITUDE_LENTH+GPRS_BASE_STATION_LATITUDE_LENTH+3)// 3个逗号



#define		AT_STRING_COPS				"AT+COPS?\r\n"
#define		AT_STRING_CIPCLOSE			"AT+CIPCLOSE\r\n"
#define		AT_STRING_CIPSHUT			"AT+CIPSHUT\r\n"
#define		AT_STRING_CREG				"AT+CREG?\r\n"
#define		AT_STRING_CGATT				"AT+CGATT?\r\n"
#define		AT_STRING_CMEE				"AT+CMEE?\r\n"
#define		AT_STRING_CSTT				"AT+CSTT=\"CMWAP\"\r\n"
#define		AT_STRING_CIICR				"AT+CIICR\r\n"
#define		AT_STRING_CIFSR				"AT+CIFSR\r\n"
#define		AT_STRING_CIPSTART			"AT+CIPSTART=\"TCP\",\"101.200.162.24\",\"5678\"\r\n"
#define		AT_STRING_CIPSEND			"AT+CIPSEND\r\n"		




typedef enum
{
	GSM_CMD_STAGE_AT_COPS,				//检测移动信号
	GSM_CMD_STAGE_AT_CIPCLOSE,
	GSM_CMD_STAGE_AT_CIPSHUT,
	GSM_CMD_STAGE_AT_CREG,
	GSM_CMD_STAGE_AT_CGATT,
	GSM_CMD_STAGE_AT_CMEE,
	GSM_CMD_STAGE_AT_CSTT,
	GSM_CMD_STAGE_AT_CIICR,
	GSM_CMD_STAGE_AT_CIFSR,
	GSM_CMD_STAGE_AT_CIPSTART,			//连接服务器
	GSM_CMD_STAGE_AT_CIPSEND,			//发送数据
	GSM_CMD_STAGE_REMOTE_RESPOND,		//服务器响应
	GSM_CMD_STAGE_CLOSE_NET,			//关闭网络进入停止状态
	GSM_CMD_STAGE_IDLE					
}GsmCmdStage_TypeEnum;

typedef struct 
{
	GsmCmdStage_TypeEnum	GsmCmdStage;			//AT指令阶段
	u8  RxFlag;
	u8	RxIndex;									//计算接收个数
	u8	RxDataBuf[GSM_RCV_MAX_NUM];					//接收一级缓存
	u8	RxDataAt[GSM_RCV_MAX_NUM];					//接收二级缓存
}GsmCmd_TypeDef;

typedef enum {
	PERMISSION_ALLOW,
	PERMISSION_PROHIBIT
}Permission_TypeEnum;

typedef struct {
	Permission_TypeEnum PermissionLocal;	//本地授权
	Permission_TypeEnum PermissionRemote;	//服务器授权
	
	
	//u8  SNRemote[TXA_SN_LENTH];				//从服务器端接收到的机身编号
	//u32 BaseStationLongitude;				//基站经度
	//u32 BaseStationLatitude;				//基站纬度
	u8 BaseStationLongitude[GPRS_BASE_STATION_LONGITUDE_LENTH];				//基站经度
	u8 BaseStationLatitude[GPRS_BASE_STATION_LATITUDE_LENTH];				//基站纬度
	u8  SNLocal[TXA_SN_LENTH];				//本地机身编号
}GprsCmd_TypeDef;

extern	GsmCmd_TypeDef	GsmCmd;
extern 	GprsCmd_TypeDef GprsCmd;

#endif





