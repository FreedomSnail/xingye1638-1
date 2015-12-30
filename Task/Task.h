#ifndef _TASK_H
#define _TASK_H


//任务间通讯用到的消息类型定义,往下
#define MSG_TYPE_INIT				1		//菜单节点初始化
#define MSG_TYPE_KEY				2
#define MSG_TYPE_WHEEL				3		//编码器的滚轮旋转消息
#define MSG_TYPE_IR					4		//红外遥控器键盘按键消息
#define MSG_TYPE_RTC				5		//时钟
#define MSG_TYPE_SANDGLASS			6		//沙漏，规定时间内无任何操作返回主界面
#define MSG_TYPE_SETUP_TO			7		//设置时间耗尽
#define MSG_TYPE_EXIT_CH_VALUE		8		//光标退回到通道名称上

#define MSG_MAX_LENGTH				4	//消息字符串长度
#define MSG_TYPE_INDEX				0	//消息类型在数组的位置偏移量
#define MSG_TYPE_RESERVE_INDEX		1	//消息类型保留位
#define MSG_CONTENT_INDEX			2	//信息内容存放在数组的位置偏移量


#define	LED_FLASH_OFF				0
#define	LED_FLASH_SHORT_TIME		3	//闪亮的时间，单位*10ms
#define	LED_FLASH_SHORT_TIME_OFF	20	//闪灭的时间，长一些，否则不明显，单位*10ms
#define	LED_FLASH_LONG_TIME			300	//常亮时间，，单位*10ms



typedef enum
{
	LEVEL_HIGH,
	LEVEL_LOW
}LevelStatus_Enum;

typedef enum
{
	POLAR_P,	//led电平极性，常态出高，亮
	POLAR_N		//
}Polar_Enum;

typedef enum
{
	NORMAL_OPERATION_ACTIVE,
	NORMAL_OPERATION_UNACTIVE,
	SETUP_OPERATION_ACTIVE,		//数字快速设置通道亮度，闪灭
}OperationType_Enum;

typedef struct 
{
	LevelStatus_Enum	LevelStatus;	//电平状态，时刻与led管脚的电平保持一致
	Polar_Enum			Polar;			//电平极性
	u16	FlashTimeset;
}
LED_TypeDef;



#endif



