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

typedef void (*DOG_OFF_PTR)(void);		// 类函数指针型 

//掉电事件报告
typedef struct
{
	DOG_OFF_PTR rpt;		//报告调用接口
	uint8 pri;				//任务优先级
	char *task;				//所属任务
}dog_offline_type;

//看门狗监控的任务状态
typedef struct
{
	uint16 count;
	BOOLEAN	monitor;
}dog_task_state_type;

//看门狗监控的任务信息
typedef struct
{
	char name[CHAR_LEN];
	uint16	time;
	int	pri;
}dog_task_cfg_type;

/*******************************************************************************************
** Function name: 	  dog_need_reset
** Descriptions:		 主站通知进行强行复位
** Input parameters：  
** Output parameters：
** Returned value:	  
*******************************************************************************************/
int8 dog_need_reset( void );

/*******************************************************************************************
** Function name: 	  dog_offline_rpt
** Descriptions:		  报告任务已经处理完毕
** Input parameters：  pri:对应的任务优先级
** Output parameters：
** Returned value:	  
*******************************************************************************************/
void dog_offline_rpt(uint8 pri);

/*******************************************************************************************
** Function name: 	  dog_isoffline
 ** Descriptions:		 获取是否处于掉电的状态
 ** Input parameters： 
** Output parameters：
** Returned value:	  0-正常供电  1- 电池供电
*******************************************************************************************/
int8 dog_isoffline( void );

/*******************************************************************************************
** Function name: 	  dog_isready
** Descriptions:		  看门狗任务是否初始化完毕
** Input parameters：  
** Output parameters：
** Returned value:	  0-未初始化完毕  1-初始化完毕
*******************************************************************************************/
int8 dog_isready( void );

/*******************************************************************************************
** Function name: 	  dog_task
** Descriptions:		  看门狗监控任务
** Input parameters：  pdata:指定参数；
** Output parameters：
** Returned value:	  
*******************************************************************************************/
void dog_task( void *pdata );

/*******************************************************************************************
** Function name: 	  dog_report
** Descriptions:		  任务报告喂狗
** Input parameters：pri:对应任务的优先级	
** Output parameters：
** Returned value:	  
*******************************************************************************************/
void dog_report( INT8U pri );

/*******************************************************************************************
** Function name: 	  dog_stop_monitor
** Descriptions:		  停止监控对应的 任务
** Input parameters：pri:对应任务的优先级 
** Output parameters：
** Returned value:	  
*******************************************************************************************/
void dog_stop_monitor( INT8U pri );

/*******************************************************************************************
** Function name: 	  dog_resume_monitor
** Descriptions:		  继续监控对应的任务
** Input parameters：pri:对应任务的优先级 
** Output parameters：
** Returned value:	  
*******************************************************************************************/
void dog_resume_monitor( INT8U pri );

/*******************************************************************************************
** Function name: 	  dog_kick
** Descriptions:		  软件喂狗
** Input parameters：
** Output parameters：
** Returned value:
*******************************************************************************************/
void dog_kick( void );

#endif/*_DOG_H_*/
