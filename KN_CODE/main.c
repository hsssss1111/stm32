/************************************************************
 * Copyright (c) 2009,漳州科能电器有限公司
 * All rights reserved. 
 *
 * 文件名称：main.c
 * 文件标识：见配置管理计划书 
 * 描述：国网负控项目的C语言入口
 *
 * 当前版本： 1.0
 * 作 者：林冬武
 * 创建日期： 2009-12-1
 *
 * 修改人：
 * 修改日期：
 * 版本：
 * 描述：
 **********************************************************/
#include "config.h"

#define	MAINTSK_PRI 10
#define	MAINTSK_STK_SIZE 256
OS_STK	MAINTSK_STACK[MAINTSK_STK_SIZE];

extern void maintask( void *pdata );

//C 程序入口
int main ( void )
{
	OSInit( );

	OSTaskCreate(maintask, NULL, &MAINTSK_STACK[MAINTSK_STK_SIZE - 1], MAINTSK_PRI);

	OSStart( );
}

