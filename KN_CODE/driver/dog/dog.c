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
** Modified by:		这个模块下个集中器产品不能放在驱动层!
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
/*
		看门狗任务:实现对添加到dog_cfg列表的任务的监控，看门狗
		以40ms为单位，每个看门狗可设置以40ms为最小单位的时隙；
		要求每个看门狗在挂起前向看门狗任务报告或者清空自己对应
		的计数器。在等待信号量或者邮箱的时候，对内核进行处理
		停止了对相应任务的监控。实现看门狗的合法管理。
*/
#include "config.h"
#include "dog.h"
#include "stm32f10x_wwdg.h"
#include "stm32f10x_rcc.h"
#include "..\kn_code\lib\debug.h"
#include "..\kn_code\lib\app_util.h"


/*------------------------数据引用-----------------------------*/

static dog_task_state_type dog_state[MONI_TASK];
static const dog_task_cfg_type dog_cfg[] =//正常模式下的通知任务
{
	//{ "UP_TASK", 	  1600, UPTSK_PRI     },
	//{ "DMREAD_TASK",  1600, DMREADTSK_PRI },
	//{ "DATASTR_TASK", 1600, DATASTRTSK_PRI},

	{ NULL, 			 0,  0  }
};

static uint8 offline[8];	//掉线预备状态
static uint8 init;		//是否初始化完毕

static uint8 of;		//是否掉电了

static uint8 reset;
const static dog_offline_type off[] = 
{
	//{up_set_offline, UPTSK_PRI, "UPTSK"},
	//{datastr_set_offline, DATASTRTSK_PRI, "DATASTRTSK"},
	{NULL, NULL },
};

/*------------------------宏定义-----------------------------*/

//看门狗任务监控的最大任务数量
#define DOG_MAX_TASK	(sizeof(dog_cfg)/sizeof(dog_cfg[0]))

/*------------------------函数实现-----------------------------*/

/*******************************************************************************************
** Function name: 	  dog_need_reset
** Descriptions:		 主站通知进行强行复位
** Input parameters：  
** Output parameters：
** Returned value:	  
*******************************************************************************************/
int8 dog_need_reset( void )
{

	reset = 0x01;

	return 0x00;
}

/*******************************************************************************************
** Function name: 	  dog_offline_rpt
** Descriptions:		  报告任务已经处理完毕
** Input parameters：  pri:对应的任务优先级
** Output parameters：
** Returned value:	  
*******************************************************************************************/
void dog_offline_rpt(uint8 pri)
{
	offline[pri / 8] |=  (1 << (pri % 8));

	return ;
}

/*******************************************************************************************
** Function name: 	  dog_isoffline
 ** Descriptions:		 获取是否处于掉电的状态
 ** Input parameters： 
** Output parameters：
** Returned value:	  0-正常供电  1- 电池供电
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
** Descriptions:		  通知外部任务 即将切掉电池供电
** Input parameters：  
** Output parameters：
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
** Descriptions:		  将优先级转化为对应的项目索引
** Input parameters：  pri:任务优先级
** Output parameters：
** Returned value:	  项目索引
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
** Descriptions:		  软件喂狗
** Input parameters：   
** Output parameters：
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
** Descriptions:		  任务报告喂狗
** Input parameters：pri:对应任务的优先级	
** Output parameters：
** Returned value:	  
*******************************************************************************************/
void dog_report( INT8U pri )
{
	int rpt;
	
	rpt = dog_pri2rpt( pri );

	//是个合法监控的任务
	if ((rpt >= 0) && (rpt < DOG_MAX_TASK))
	{
		dog_state[rpt].count = dog_cfg[rpt].time;
	}
	
}

/*******************************************************************************************
** Function name: 	  dog_stop_monitor
** Descriptions:		  停止监控对应的 任务
** Input parameters：pri:对应任务的优先级 
** Output parameters：
** Returned value:	  
*******************************************************************************************/
void dog_stop_monitor( INT8U pri )
{
	int rpt;
	//INT8U err;
	
	rpt = dog_pri2rpt( pri );
	//是个合法监控的任务
	if ((rpt >= 0) && (rpt < DOG_MAX_TASK))
	{
		//OSSemPend(d_sem, 0, &err);
		dog_state[rpt].monitor = FALSE;
		//OSSemPost(d_sem);
	}
}

/*******************************************************************************************
** Function name: 	  dog_check_offline
** Descriptions:		  判断是否停电了
** Input parameters： 
** Output parameters：
** Returned value:	  0-没掉电   1-掉电
*******************************************************************************************/
int8 dog_check_offline( void )
{

	return 0x00;
}

/*******************************************************************************************
** Function name: 	  dog_do_poweroff
** Descriptions:		  给系统下电
** Input parameters： 
** Output parameters：
** Returned value:	   
*******************************************************************************************/
static void dog_do_poweroff( void )
{
	int8 ofline;
	DEBUG("关闭系统 bye bye!\r\n");
	
	//主站要求进行复位
	if (reset == 0x01)
	{
		//在供电情况下，进行复位
		OS_ENTER_CRITICAL( );
		for(;;) ;
	}
	
	ofline = dog_check_offline( );
	//掉电情况下直接拉管脚
	if (ofline == 0x01)
	{
		OS_ENTER_CRITICAL( );
		for(;;) ;
	}
	else
	{
		OS_ENTER_CRITICAL( );
		//在供电情况下，进行复位
		for(;;) ;
	}
}

/*******************************************************************************************
** Functmion name: 	  dog_resume_monitor
** Descriptions:		  继续监控对应的任务
** Input parameters：pri:对应任务的优先级 
** Output parameters：

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
** Descriptions:		  看门狗任务是否初始化完毕
** Input parameters：  
** Output parameters：
** Returned value:	  0-未初始化完毕  1-初始化完毕
*******************************************************************************************/
int8 dog_isready( void )
{
	return init;
}

/*******************************************************************************************
** Function name: 	  dog_init
** Descriptions:		  看门狗初始化
** Input parameters：  
** Output parameters：
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
	
	init = 0x01;//初始化完毕
}

/*******************************************************************************************
** Function name: 	  dog_task
** Descriptions:		  看门狗监控任务
** Input parameters：  pdata:指定参数；
** Output parameters：
** Returned value:	  
*******************************************************************************************/
void dog_task( void *pdata )
{	
	int pos = 0;
	uint8 i;
	int8 online;
	uint16 wait;
	
	dog_init( );
	dog_kick( );//启动内部看门狗
	
	for(;;)
	{
		pos = 0x00;
		do
		{
			if ( dog_state[pos].monitor == TRUE )
			{
				if ( --dog_state[pos].count == 0x00 )
				{
					//时间周期已到，需进行处理
					//可添加尝试复位的代码
					DEBUG("任务%d 挂了\r\n", pos);
					reset = 0x01;
					dog_do_poweroff( );
				}
			}
			else
			{
				//不监控的任务 复位时间
				dog_state[pos].count = dog_cfg[pos].time;
			}
			pos++;
		}while(pos < DOG_MAX_TASK );
	

		//电池供电超时了，通知掉电
		if (reset == 0x01)
		{
			DEBUG("发送复位消息!!\r\n");
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
					DEBUG("检测掉电发生!!!\r\n");
					//@todo 保存数据
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
			DEBUG("检测掉电发生!!!\r\n");
			//@todo 保存数据
			dog_do_poweroff( );
		}
			
		dog_kick( );
		OSTimeDly(10);//wait 10ms.
	}
}

