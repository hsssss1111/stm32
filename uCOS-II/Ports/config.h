/****************************************Copyright (c)****************************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                               http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           config.h
** Last modified Date:  2004-09-17
** Last Version:        1.0
** Descriptions:        User Configurable File
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Chenmingji
** Created date:        2004-09-17
** Version: 1.0
** Descriptions:        First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:		heshusheng
** Modified date:		2009-02-23
** Version:				1.1
** Descriptions:		针对realview的配置，为保证realview配置的最大稳定，特
**							将科能添加的头文件的路径全部在头文件实现。
**
*********************************************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE  1
#endif                                                                  /*  TRUE                        */

#ifndef FALSE
#define FALSE 0
#endif                                                                  /*  FALSE                       */

#pragma diag_suppress 870                                               /* 屏蔽invalid multibyte character sequence警告*/

typedef unsigned char  uint8;                                           /*  无符号8位整型变量           */
typedef signed   char  int8;                                            /*  有符号8位整型变量           */
typedef unsigned short uint16;                                          /*  无符号16位整型变量          */
typedef signed   short int16;                                           /*  有符号16位整型变量          */
typedef unsigned int   uint32;                                          /*  无符号32位整型变量          */
typedef signed   int   int32;                                           /*  有符号32位整型变量          */
typedef float          fp32;                                            /*  单精度浮点数（32位长度）    */
typedef double         fp64;                                            /*  双精度浮点数（64位长度）    */

typedef unsigned char  INT8U;                                           /*  无符号8位整型变量           */
typedef signed   char  INT8S;                                           /*  有符号8位整型变量           */
typedef unsigned short INT16U;                                          /*  无符号16位整型变量          */
typedef signed   short INT16S;                                          /*  有符号16位整型变量          */
typedef unsigned int   INT32U;                                          /*  无符号32位整型变量          */
typedef signed   int   INT32S;                                          /*  有符号32位整型变量          */
typedef float          FP32;                                            /*  单精度浮点数（32位长度）    */
typedef double         FP64;                                            /*  双精度浮点数（64位长度）    */

#define assert_param(expr) ((void)0)

/*********************************************************************************************************
  uC/OS-II specital code
  uC/OS-II的特殊代码
*********************************************************************************************************/
#include "Includes.h"

/*********************************************************************************************************
  ARM的特殊代码
  ARM specital code
*********************************************************************************************************/
#include "stm32f10x.h"
#include "system_stm32f10x.h"

/*********************************************************************************************************
  应用程序配置
  Application Program Configurations
*********************************************************************************************************/
#include    <stdarg.h>
#include    <stdio.h>
#include    <rt_sys.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
#include    <rt_misc.h>

#ifdef __cplusplus
    }
#endif                                                                  /*  __cplusplus                 */
#endif                                                                  /*  __CONFIG_H                  */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
