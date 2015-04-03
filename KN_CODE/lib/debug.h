/****************************************Copyright (c)****************************************************
**                         							ZhangZhou KENENG electronics Co.,LTD.                               
**                                     
**
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			debug.h
** Last modified Date:  2009-02-16
** Last Version:		1.0
** Descriptions:		just now we use the UART1 for debug port.we send the debug messages
**							to the debug port.
**
**--------------------------------------------------------------------------------------------------------
** Created by:			heshusheng
** Created date:		2009-02-16
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
#ifndef _DEBUG_H_
#define _DEBUG_H_

/*********************************************************************************************************
** 函数名称: DEBUG
** 功能描述: 打印调试信息到串口1
** 输入参数: 待输出的调试信息
** 输出参数: 无
********************************************************************************************************/
int DEBUG(char *fmt, ...);

/*********************************************************************************************************
** 函数名称: debug_init
** 功能描述: 调试串口初始化
** 输入参数: 无
** 输出参数: 无
********************************************************************************************************/
void debug_init( void *param);

#endif/*_DEBUG_H_*/
