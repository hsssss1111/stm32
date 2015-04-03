/************************************************************
 * Copyright (c) 2009,���ݿ��ܵ������޹�˾
 * All rights reserved. 
 *
 * �ļ����ƣ�maintask.h
 * �ļ���ʶ�������ù���ƻ��� 
 * ������ ��Ҫ�������ļ������� 
 *
 * ��ǰ�汾�� 1.0
 * �����ߣ��ֶ���
 * �������ڣ� 2010-3-2
 *
 * �޸��ˣ�
 * �޸����ڣ�
 * �޸�ԭ��
 **********************************************************/

#ifndef MAINTASK_H_
#define MAINTASK_H_

#include ".\kn_code\app\sys_config.h"
#include ".\kn_code\driver\uart\uart.h"
#include ".\kn_code\lib\debug.h"
#include ".\kn_code\driver\dog\dog.h"
#include ".\kn_code\driver\led\led.h"
#include ".\kn_code\app\wifi\wifi.h"

/*-----------------------------------������ز�������----------------------------------------*/

/****************************************************************************/
// �������ջ���ȶ���
/****************************************************************************/
#define	DOGTSK_STK_SIZE		       128
#define	LEDTSK_STK_SIZE		       128
#define	COMMTSK_STK_SIZE		  1024
#define WIFITSK_STK_SIZE          1024
#define UPTSK_STK_SIZE            2048
#define DMREADTSK_STK_SIZE        2048
#define DATASTRTSK_STK_SIZE       2048

/****************************************************************************/
// �������ջ����
/****************************************************************************/
__align(8) OS_STK	DOGTSK_STACK[DOGTSK_STK_SIZE];
__align(8) OS_STK	LEDTSK_STACK[LEDTSK_STK_SIZE];
__align(8) OS_STK	WIFITSK_STACK[WIFITSK_STK_SIZE];
//__align(8) OS_STK	DATASTRTSK_STACK[DATASTRTSK_STK_SIZE];
//__align(8) OS_STK	UPTSK_STACK[UPTSK_STK_SIZE];
//__align(8) OS_STK	DMREADTSK_STACK[DMREADTSK_STK_SIZE];

/*******************************************************************************************************/
// ����ָ�����Ͷ���
/*******************************************************************************************************/
typedef void (*TSK_PTR)(void *param);  // �ຯ��ָ����
typedef void (*INIT_PTR)(void *param); // ��ʼ������ָ��

/*******************************************************************************************************/
// ������ز�������
/*******************************************************************************************************/

// ������Ϣ�ṹ
typedef struct
{
	INIT_PTR ini_entry; // ������ʼ���������
	INT32S param[6]; //���ݵĲ���
} DRV_INFO;

// ������Ϣ�����б�
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
// ������ز�������
/*******************************************************************************************************/

// ������������ṹ
typedef struct
{
	TSK_PTR task; // ��������
	OS_STK *ptos; // �����ջջ��ָ��
	INT32U stk_size; // �����ջ����
	INT8U prio; // �������ȼ�

} TSK_INFO;

/****************************************************************************/
// Ԥ����Ҫ������������Ϣ�����б�
/****************************************************************************/
const TSK_INFO pretsk_info[] = {
{ (TSK_PTR)dog_task,		  (OS_STK*)&DOGTSK_STACK[0],		 DOGTSK_STK_SIZE,		  DOGTSK_PRI,		  },
{ (TSK_PTR)led_task,          (OS_STK*)&LEDTSK_STACK[0],		 LEDTSK_STK_SIZE, 	      LEDTSK_PRI,		  },
{ (INIT_PTR)0,                (OS_STK*)0,                        0,                       0,                  }
};

/****************************************************************************/
// ������Ϣ�����б�
/****************************************************************************/
const TSK_INFO tsk_info[] = {
	{ (TSK_PTR)wifi_task,	      (OS_STK*)&WIFITSK_STACK[0],       WIFITSK_STK_SIZE,       WIFITSK_PRI,       },
	//{ (TSK_PTR)up_task,	          (OS_STK*)&UPTSK_STACK[0],          UPTSK_STK_SIZE,          UPTSK_PRI,          },
	//{ (TSK_PTR)dmread_task,	      (OS_STK*)&DMREADTSK_STACK[0],      DMREADTSK_STK_SIZE,      DMREADTSK_PRI,      },
	//{ (TSK_PTR)datastr_task,	  (OS_STK*)&DATASTRTSK_STACK[0],     DATASTRTSK_STK_SIZE,     DATASTRTSK_PRI,     },
	{ (INIT_PTR)0,                (OS_STK*)0,                        0,                       0,                  }
};

#endif /* MAINTASK_H_ */

