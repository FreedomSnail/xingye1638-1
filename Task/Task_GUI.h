#ifndef _TASK_GUI_H
#define _TASK_GUI_H

#define		TASK_GUI_STK_SIZE	128
#define		TASK_GUI_PRIO		7
#define		GUI_RESOURCES		10


#define		RESULT_STAND				1		//��ʾ���洦������ά�ֵ�ǰ�ڵ�
#define		RESULT_OTHER_NODE			2		//��ʾ���洦�������л�������һ���ڵ�
#define		RESULT_COMMON_MSG_FILTER	3		//������Ϣ����





extern	OS_EVENT **pGuiQSem; 

extern	OS_EVENT *GuiNormalQSem; 
extern	OS_STK TaskGuiStk[];





extern	void Task_GUI(void* p_arg);

#endif
			   

 
