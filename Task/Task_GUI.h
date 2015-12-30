#ifndef _TASK_GUI_H
#define _TASK_GUI_H

#define		TASK_GUI_STK_SIZE	128
#define		TASK_GUI_PRIO		7
#define		GUI_RESOURCES		10


#define		RESULT_STAND				1		//显示界面处理结果，维持当前节点
#define		RESULT_OTHER_NODE			2		//显示界面处理结果，切换到另外一个节点
#define		RESULT_COMMON_MSG_FILTER	3		//公共消息过滤





extern	OS_EVENT **pGuiQSem; 

extern	OS_EVENT *GuiNormalQSem; 
extern	OS_STK TaskGuiStk[];





extern	void Task_GUI(void* p_arg);

#endif
			   

 
