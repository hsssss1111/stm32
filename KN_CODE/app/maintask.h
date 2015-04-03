/************************************************************
 * Copyright (c) 2009,漳州科能电器有限公司
 * All rights reserved. 
 *
 * 文件名称：maintask.h
 * 文件标识：见配置管理计划书 
 * 描述： 简要描述本文件的内容 
 *
 * 当前版本： 1.0
 * 创建者：林冬武
 * 创建日期： 2010-3-2
 *
 * 修改人：
 * 修改日期：
 * 修改原因：
 **********************************************************/

#ifndef MAINTASK_H_
#define MAINTASK_H_

#include ".\kn_code\app\sys_config.h"
#include ".\kn_code\driver\uart\uart.h"
#include ".\kn_code\lib\debug.h"
#include ".\kn_code\driver\dog\dog.h"
#include ".\kn_code\driver\led\led.h"
#include ".\kn_code\app\wifi\wifi.h"

/*-----------------------------------任务相关参数定义----------------------------------------*/

/****************************************************************************/
// 各任务堆栈长度定义
/****************************************************************************/
#define	DOGTSK_STK_SIZE		       128
#define	LEDTSK_STK_SIZE		       128
#define	COMMTSK_STK_SIZE		  1024
#define WIFITSK_STK_SIZE          1024
#define UPTSK_STK_SIZE            2048
#define DMREADTSK_STK_SIZE        2048
#define DATASTRTSK_STK_SIZE       2048

/****************************************************************************/
// 各任务堆栈定义
/****************************************************************************/
__align(8) OS_STK	DOGTSK_STACK[DOGTSK_STK_SIZE];
__align(8) OS_STK	LEDTSK_STACK[LEDTSK_STK_SIZE];
__align(8) OS_STK	WIFITSK_STACK[WIFITSK_STK_SIZE];
//__align(8) OS_STK	DATASTRTSK_STACK[DATASTRTSK_STK_SIZE];
//__align(8) OS_STK	UPTSK_STACK[UPTSK_STK_SIZE];
//__align(8) OS_STK	DMREADTSK_STACK[DMREADTSK_STK_SIZE];

/*******************************************************************************************************/
// 函数指针类型定义
/*******************************************************************************************************/
typedef void (*TSK_PTR)(void *param);  // 类函数指针型
typedef void (*INIT_PTR)(void *param); // 初始化函数指针

/*******************************************************************************************************/
// 驱动相关参数定义
/*******************************************************************************************************/

// 驱动信息结构
typedef struct
{
	INIT_PTR ini_entry; // 驱动初始化函数入口
	INT32S param[6]; //传递的参数
} DRV_INFO;

// 驱动信息管理列表
DRV_INFO drv_info[] =
{
	{ debug_init, 			{0}                            },//"debug_driver"
	//{ app_init, 			{0}                            },//"app_driver"
	//{ uartInit, 			{ID_UART1, 2400,8,1,2,0}          },//"RS485_driver"
	{ uartInit, 		{ID_UART2, 115200,8,1,0,0}          },//"wifi driver"
	//{ uartInit, 			{ID_UART3, 115200,8,1,0,0}        },//"modem_driver"
	//{ uartInit, 			{UART4, 9600,8,1,2,0}          },//"locomm_driver"
	//{ i2cInit, 				{100000}                       },//"i2c_driver"
	//{ rtc_init, 			{0}                            },//"rtc_driver"
	{ (INIT_PTR)0,   		{0}                            }
};

/*******************************************************************************************************/
// 任务相关参数定义
/*******************************************************************************************************/

// 创建任务参数结构
typedef struct
{
	TSK_PTR task; // 任务函数名
	OS_STK *ptos; // 任务堆栈栈顶指针
	INT32U stk_size; // 任务堆栈长度
	INT8U prio; // 任务优先级

} TSK_INFO;

/****************************************************************************/
// 预先需要创建的任务信息管理列表
/****************************************************************************/
const TSK_INFO pretsk_info[] = {
{ (TSK_PTR)dog_task,		  (OS_STK*)&DOGTSK_STACK[0],		 DOGTSK_STK_SIZE,		  DOGTSK_PRI,		  },
{ (TSK_PTR)led_task,          (OS_STK*)&LEDTSK_STACK[0],		 LEDTSK_STK_SIZE, 	      LEDTSK_PRI,		  },
{ (INIT_PTR)0,                (OS_STK*)0,                        0,                       0,                  }
};

/****************************************************************************/
// 任务信息管理列表
/****************************************************************************/
const TSK_INFO tsk_info[] = {
	{ (TSK_PTR)wifi_task,	      (OS_STK*)&WIFITSK_STACK[0],       WIFITSK_STK_SIZE,       WIFITSK_PRI,       },
	//{ (TSK_PTR)up_task,	          (OS_STK*)&UPTSK_STACK[0],          UPTSK_STK_SIZE,          UPTSK_PRI,          },
	//{ (TSK_PTR)dmread_task,	      (OS_STK*)&DMREADTSK_STACK[0],      DMREADTSK_STK_SIZE,      DMREADTSK_PRI,      },
	//{ (TSK_PTR)datastr_task,	  (OS_STK*)&DATASTRTSK_STACK[0],     DATASTRTSK_STK_SIZE,     DATASTRTSK_PRI,     },
	{ (INIT_PTR)0,                (OS_STK*)0,                        0,                       0,                  }
};

#endif /* MAINTASK_H_ */

