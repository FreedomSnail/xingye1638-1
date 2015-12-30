#ifndef _SIM900A_H
#define _SIM900A_H 



#define		GSM_RCV_MAX_NUM				50

#define		CIPSEND_END_CHAR			0x1A	//��
#define 	TXA_SN_LENTH		16
#define 	GPRS_BASE_STATION_LONGITUDE_LENTH		10	
#define 	GPRS_BASE_STATION_LATITUDE_LENTH		9	
#define 	GPRS_CMD_SEND_LENTH						(TXA_SN_LENTH+GPRS_BASE_STATION_LONGITUDE_LENTH+GPRS_BASE_STATION_LATITUDE_LENTH+3)// 3������



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
	GSM_CMD_STAGE_AT_COPS,				//����ƶ��ź�
	GSM_CMD_STAGE_AT_CIPCLOSE,
	GSM_CMD_STAGE_AT_CIPSHUT,
	GSM_CMD_STAGE_AT_CREG,
	GSM_CMD_STAGE_AT_CGATT,
	GSM_CMD_STAGE_AT_CMEE,
	GSM_CMD_STAGE_AT_CSTT,
	GSM_CMD_STAGE_AT_CIICR,
	GSM_CMD_STAGE_AT_CIFSR,
	GSM_CMD_STAGE_AT_CIPSTART,			//���ӷ�����
	GSM_CMD_STAGE_AT_CIPSEND,			//��������
	GSM_CMD_STAGE_REMOTE_RESPOND,		//��������Ӧ
	GSM_CMD_STAGE_CLOSE_NET,			//�ر��������ֹͣ״̬
	GSM_CMD_STAGE_IDLE					
}GsmCmdStage_TypeEnum;

typedef struct 
{
	GsmCmdStage_TypeEnum	GsmCmdStage;			//ATָ��׶�
	u8  RxFlag;
	u8	RxIndex;									//������ո���
	u8	RxDataBuf[GSM_RCV_MAX_NUM];					//����һ������
	u8	RxDataAt[GSM_RCV_MAX_NUM];					//���ն�������
}GsmCmd_TypeDef;

typedef enum {
	PERMISSION_ALLOW,
	PERMISSION_PROHIBIT
}Permission_TypeEnum;

typedef struct {
	Permission_TypeEnum PermissionLocal;	//������Ȩ
	Permission_TypeEnum PermissionRemote;	//��������Ȩ
	
	
	//u8  SNRemote[TXA_SN_LENTH];				//�ӷ������˽��յ��Ļ�����
	//u32 BaseStationLongitude;				//��վ����
	//u32 BaseStationLatitude;				//��վγ��
	u8 BaseStationLongitude[GPRS_BASE_STATION_LONGITUDE_LENTH];				//��վ����
	u8 BaseStationLatitude[GPRS_BASE_STATION_LATITUDE_LENTH];				//��վγ��
	u8  SNLocal[TXA_SN_LENTH];				//���ػ�����
}GprsCmd_TypeDef;

extern	GsmCmd_TypeDef	GsmCmd;
extern 	GprsCmd_TypeDef GprsCmd;

#endif





