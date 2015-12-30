#ifndef _TASK_H
#define _TASK_H


//�����ͨѶ�õ�����Ϣ���Ͷ���,����
#define MSG_TYPE_INIT				1		//�˵��ڵ��ʼ��
#define MSG_TYPE_KEY				2
#define MSG_TYPE_WHEEL				3		//�������Ĺ�����ת��Ϣ
#define MSG_TYPE_IR					4		//����ң�������̰�����Ϣ
#define MSG_TYPE_RTC				5		//ʱ��
#define MSG_TYPE_SANDGLASS			6		//ɳ©���涨ʱ�������κβ�������������
#define MSG_TYPE_SETUP_TO			7		//����ʱ��ľ�
#define MSG_TYPE_EXIT_CH_VALUE		8		//����˻ص�ͨ��������

#define MSG_MAX_LENGTH				4	//��Ϣ�ַ�������
#define MSG_TYPE_INDEX				0	//��Ϣ�����������λ��ƫ����
#define MSG_TYPE_RESERVE_INDEX		1	//��Ϣ���ͱ���λ
#define MSG_CONTENT_INDEX			2	//��Ϣ���ݴ���������λ��ƫ����


#define	LED_FLASH_OFF				0
#define	LED_FLASH_SHORT_TIME		3	//������ʱ�䣬��λ*10ms
#define	LED_FLASH_SHORT_TIME_OFF	20	//�����ʱ�䣬��һЩ���������ԣ���λ*10ms
#define	LED_FLASH_LONG_TIME			300	//����ʱ�䣬����λ*10ms



typedef enum
{
	LEVEL_HIGH,
	LEVEL_LOW
}LevelStatus_Enum;

typedef enum
{
	POLAR_P,	//led��ƽ���ԣ���̬���ߣ���
	POLAR_N		//
}Polar_Enum;

typedef enum
{
	NORMAL_OPERATION_ACTIVE,
	NORMAL_OPERATION_UNACTIVE,
	SETUP_OPERATION_ACTIVE,		//���ֿ�������ͨ�����ȣ�����
}OperationType_Enum;

typedef struct 
{
	LevelStatus_Enum	LevelStatus;	//��ƽ״̬��ʱ����led�ܽŵĵ�ƽ����һ��
	Polar_Enum			Polar;			//��ƽ����
	u16	FlashTimeset;
}
LED_TypeDef;



#endif



