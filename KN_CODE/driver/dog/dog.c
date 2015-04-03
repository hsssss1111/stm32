/****************************************Copyright (c)****************************************************
**                         							ZhangZhou KENENG electronics Co.,LTD.                               
**                                     
**
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			dog.c
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
** Modified by:		���ģ���¸���������Ʒ���ܷ���������!
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
/*
		���Ź�����:ʵ�ֶ���ӵ�dog_cfg�б������ļ�أ����Ź�
		��40msΪ��λ��ÿ�����Ź���������40msΪ��С��λ��ʱ϶��
		Ҫ��ÿ�����Ź��ڹ���ǰ���Ź����񱨸��������Լ���Ӧ
		�ļ��������ڵȴ��ź������������ʱ�򣬶��ں˽��д���
		ֹͣ�˶���Ӧ����ļ�ء�ʵ�ֿ��Ź��ĺϷ�����
*/
#include "config.h"
#include "dog.h"
#include "stm32f10x_wwdg.h"
#include "stm32f10x_rcc.h"
#include "..\kn_code\lib\debug.h"
#include "..\kn_code\lib\app_util.h"


/*------------------------��������-----------------------------*/

static dog_task_state_type dog_state[MONI_TASK];
static const dog_task_cfg_type dog_cfg[] =//����ģʽ�µ�֪ͨ����
{
	//{ "UP_TASK", 	  1600, UPTSK_PRI     },
	//{ "DMREAD_TASK",  1600, DMREADTSK_PRI },
	//{ "DATASTR_TASK", 1600, DATASTRTSK_PRI},

	{ NULL, 			 0,  0  }
};

static uint8 offline[8];	//����Ԥ��״̬
static uint8 init;		//�Ƿ��ʼ�����

static uint8 of;		//�Ƿ������

static uint8 reset;
const static dog_offline_type off[] = 
{
	//{up_set_offline, UPTSK_PRI, "UPTSK"},
	//{datastr_set_offline, DATASTRTSK_PRI, "DATASTRTSK"},
	{NULL, NULL },
};

/*------------------------�궨��-----------------------------*/

//���Ź������ص������������
#define DOG_MAX_TASK	(sizeof(dog_cfg)/sizeof(dog_cfg[0]))

/*------------------------����ʵ��-----------------------------*/

/*******************************************************************************************
** Function name: 	  dog_need_reset
** Descriptions:		 ��վ֪ͨ����ǿ�и�λ
** Input parameters��  
** Output parameters��
** Returned value:	  
*******************************************************************************************/
int8 dog_need_reset( void )
{

	reset = 0x01;

	return 0x00;
}

/*******************************************************************************************
** Function name: 	  dog_offline_rpt
** Descriptions:		  ���������Ѿ��������
** Input parameters��  pri:��Ӧ���������ȼ�
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void dog_offline_rpt(uint8 pri)
{
	offline[pri / 8] |=  (1 << (pri % 8));

	return ;
}

/*******************************************************************************************
** Function name: 	  dog_isoffline
 ** Descriptions:		 ��ȡ�Ƿ��ڵ����״̬
 ** Input parameters�� 
** Output parameters��
** Returned value:	  0-��������  1- ��ع���
*******************************************************************************************/
int8 dog_isoffline( void )
{
	if (of == 0x01)
		return 0x01;
	else
		return 0x00;
}

/*******************************************************************************************
** Function name: 	  dog_sendoffline
** Descriptions:		  ֪ͨ�ⲿ���� �����е���ع���
** Input parameters��  
** Output parameters��
** Returned value:	  
*******************************************************************************************/
static int8 dog_sendoffline( void )
{
	uint8 i = 0;
	
	while(off[i].task != NULL)
	{
		off[i].rpt();
		i++;
	}

	return 0;
}

/*******************************************************************************************
** Function name: 	  dog_pri2rpt
** Descriptions:		  �����ȼ�ת��Ϊ��Ӧ����Ŀ����
** Input parameters��  pri:�������ȼ�
** Output parameters��
** Returned value:	  ��Ŀ����
*******************************************************************************************/
static int dog_pri2rpt( INT8U pri )
{
	int i;

	for(i = 0; i < DOG_MAX_TASK; i++)
	{
		if( pri == dog_cfg[i].pri )
			return i;
	}

	return -1;
}

/*******************************************************************************************
** Function name: 	  dog_kick
** Descriptions:		  ���ι��
** Input parameters��   
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void dog_kick( void )
{
#ifdef DOG
	WWDG_SetCounter(0x7d);
#endif
	return;
}


/*******************************************************************************************
** Function name: 	  dog_report
** Descriptions:		  ���񱨸�ι��
** Input parameters��pri:��Ӧ��������ȼ�	
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void dog_report( INT8U pri )
{
	int rpt;
	
	rpt = dog_pri2rpt( pri );

	//�Ǹ��Ϸ���ص�����
	if ((rpt >= 0) && (rpt < DOG_MAX_TASK))
	{
		dog_state[rpt].count = dog_cfg[rpt].time;
	}
	
}

/*******************************************************************************************
** Function name: 	  dog_stop_monitor
** Descriptions:		  ֹͣ��ض�Ӧ�� ����
** Input parameters��pri:��Ӧ��������ȼ� 
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void dog_stop_monitor( INT8U pri )
{
	int rpt;
	//INT8U err;
	
	rpt = dog_pri2rpt( pri );
	//�Ǹ��Ϸ���ص�����
	if ((rpt >= 0) && (rpt < DOG_MAX_TASK))
	{
		//OSSemPend(d_sem, 0, &err);
		dog_state[rpt].monitor = FALSE;
		//OSSemPost(d_sem);
	}
}

/*******************************************************************************************
** Function name: 	  dog_check_offline
** Descriptions:		  �ж��Ƿ�ͣ����
** Input parameters�� 
** Output parameters��
** Returned value:	  0-û����   1-����
*******************************************************************************************/
int8 dog_check_offline( void )
{

	return 0x00;
}

/*******************************************************************************************
** Function name: 	  dog_do_poweroff
** Descriptions:		  ��ϵͳ�µ�
** Input parameters�� 
** Output parameters��
** Returned value:	   
*******************************************************************************************/
static void dog_do_poweroff( void )
{
	int8 ofline;
	DEBUG("�ر�ϵͳ bye bye!\r\n");
	
	//��վҪ����и�λ
	if (reset == 0x01)
	{
		//�ڹ�������£����и�λ
		OS_ENTER_CRITICAL( );
		for(;;) ;
	}
	
	ofline = dog_check_offline( );
	//���������ֱ�����ܽ�
	if (ofline == 0x01)
	{
		OS_ENTER_CRITICAL( );
		for(;;) ;
	}
	else
	{
		OS_ENTER_CRITICAL( );
		//�ڹ�������£����и�λ
		for(;;) ;
	}
}

/*******************************************************************************************
** Functmion name: 	  dog_resume_monitor
** Descriptions:		  ������ض�Ӧ������
** Input parameters��pri:��Ӧ��������ȼ� 
** Output parameters��

*******************************************************************************************/
void dog_resume_monitor( INT8U pri )
{
	int rpt;

	rpt = dog_pri2rpt( pri );

	if ((rpt >= 0) && (rpt < DOG_MAX_TASK))
	{
		dog_state[rpt].count = dog_cfg[rpt].time;
		dog_state[rpt].monitor = TRUE;
	}
}

/*******************************************************************************************
** Function name: 	  dog_isready
** Descriptions:		  ���Ź������Ƿ��ʼ�����
** Input parameters��  
** Output parameters��
** Returned value:	  0-δ��ʼ�����  1-��ʼ�����
*******************************************************************************************/
int8 dog_isready( void )
{
	return init;
}

/*******************************************************************************************
** Function name: 	  dog_init
** Descriptions:		  ���Ź���ʼ��
** Input parameters��  
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void dog_init( void )
{
#ifdef DOG
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x7f);
	WWDG_Enable(0x7d);
#endif

	memset(offline, 0x00, 8);
	
	init = 0x01;//��ʼ�����
}

/*******************************************************************************************
** Function name: 	  dog_task
** Descriptions:		  ���Ź��������
** Input parameters��  pdata:ָ��������
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void dog_task( void *pdata )
{	
	int pos = 0;
	uint8 i;
	int8 online;
	uint16 wait;
	
	dog_init( );
	dog_kick( );//�����ڲ����Ź�
	
	for(;;)
	{
		pos = 0x00;
		do
		{
			if ( dog_state[pos].monitor == TRUE )
			{
				if ( --dog_state[pos].count == 0x00 )
				{
					//ʱ�������ѵ�������д���
					//����ӳ��Ը�λ�Ĵ���
					DEBUG("����%d ����\r\n", pos);
					reset = 0x01;
					dog_do_poweroff( );
				}
			}
			else
			{
				//����ص����� ��λʱ��
				dog_state[pos].count = dog_cfg[pos].time;
			}
			pos++;
		}while(pos < DOG_MAX_TASK );
	

		//��ع��糬ʱ�ˣ�֪ͨ����
		if (reset == 0x01)
		{
			DEBUG("���͸�λ��Ϣ!!\r\n");
			memset(offline, 0x00, 0x08);
			dog_sendoffline( );
			
			//wait to power off.
			wait = 0x00;

			for(;;)
			{
				for (i = 0; off[i].task != NULL; i++)
				{
					if (!BitGet(offline[off[i].pri / 8], (off[i].pri % 8)))
					{
						break;
					}
				}

				//poweroff  if all task ready  or   timeout.
				if ((off[i].task == NULL) || ( wait == 1500))
				{
					dog_do_poweroff( );
				}

				of = dog_check_offline( );
				if (of == 0x01)
				{
					DEBUG("�����緢��!!!\r\n");
					//@todo ��������
					dog_do_poweroff( );
				}
		
				wait++;
				OSTimeDly(5);
				dog_kick( );
			}
		}

		of = dog_check_offline( );
		if (of == 0x01)
		{
			DEBUG("�����緢��!!!\r\n");
			//@todo ��������
			dog_do_poweroff( );
		}
			
		dog_kick( );
		OSTimeDly(10);//wait 10ms.
	}
}

