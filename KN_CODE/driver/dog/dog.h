/****************************************Copyright (c)****************************************************
**                         						ZhangZhou KENENG electronics Co.,LTD.                               
**                                     
**
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			dog.h
** Last modified Date:  2009-02-14
** Last Version:		1.0
** Descriptions:		The software dog for the system.
**
**--------------------------------------------------------------------------------------------------------
** Created by:			heshusheng
** Created date:		2009-02-14
** Version:				1.0
** Descriptions:		The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef _DOG_H_
#define _DOG_H_

#include		"config.h"

#define CHAR_LEN			20
#define MONI_TASK		64

typedef void (*DOG_OFF_PTR)(void);		// �ຯ��ָ���� 

//�����¼�����
typedef struct
{
	DOG_OFF_PTR rpt;		//������ýӿ�
	uint8 pri;				//�������ȼ�
	char *task;				//��������
}dog_offline_type;

//���Ź���ص�����״̬
typedef struct
{
	uint16 count;
	BOOLEAN	monitor;
}dog_task_state_type;

//���Ź���ص�������Ϣ
typedef struct
{
	char name[CHAR_LEN];
	uint16	time;
	int	pri;
}dog_task_cfg_type;

/*******************************************************************************************
** Function name: 	  dog_need_reset
** Descriptions:		 ��վ֪ͨ����ǿ�и�λ
** Input parameters��  
** Output parameters��
** Returned value:	  
*******************************************************************************************/
int8 dog_need_reset( void );

/*******************************************************************************************
** Function name: 	  dog_offline_rpt
** Descriptions:		  ���������Ѿ��������
** Input parameters��  pri:��Ӧ���������ȼ�
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void dog_offline_rpt(uint8 pri);

/*******************************************************************************************
** Function name: 	  dog_isoffline
 ** Descriptions:		 ��ȡ�Ƿ��ڵ����״̬
 ** Input parameters�� 
** Output parameters��
** Returned value:	  0-��������  1- ��ع���
*******************************************************************************************/
int8 dog_isoffline( void );

/*******************************************************************************************
** Function name: 	  dog_isready
** Descriptions:		  ���Ź������Ƿ��ʼ�����
** Input parameters��  
** Output parameters��
** Returned value:	  0-δ��ʼ�����  1-��ʼ�����
*******************************************************************************************/
int8 dog_isready( void );

/*******************************************************************************************
** Function name: 	  dog_task
** Descriptions:		  ���Ź��������
** Input parameters��  pdata:ָ��������
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void dog_task( void *pdata );

/*******************************************************************************************
** Function name: 	  dog_report
** Descriptions:		  ���񱨸�ι��
** Input parameters��pri:��Ӧ��������ȼ�	
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void dog_report( INT8U pri );

/*******************************************************************************************
** Function name: 	  dog_stop_monitor
** Descriptions:		  ֹͣ��ض�Ӧ�� ����
** Input parameters��pri:��Ӧ��������ȼ� 
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void dog_stop_monitor( INT8U pri );

/*******************************************************************************************
** Function name: 	  dog_resume_monitor
** Descriptions:		  ������ض�Ӧ������
** Input parameters��pri:��Ӧ��������ȼ� 
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void dog_resume_monitor( INT8U pri );

/*******************************************************************************************
** Function name: 	  dog_kick
** Descriptions:		  ���ι��
** Input parameters��
** Output parameters��
** Returned value:
*******************************************************************************************/
void dog_kick( void );

#endif/*_DOG_H_*/
