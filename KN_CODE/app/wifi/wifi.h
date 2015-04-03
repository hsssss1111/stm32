
/****************************************Copyright (c)****************************************************
**                         							
**
**
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			wifi.c
** Last modified Date:  2015-02-27
** Last Version:		1.0
** Descriptions:		wifi
**
**--------------------------------------------------------------------------------------------------------
** Created by:			heshusheng
** Created date:		2015-02-27
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
#ifndef _WIFI_H_
#define _WIFI_H_

//����wifi����ģʽ
#define WIFI_MODE_STA       0x01
#define WIFI_MODE_AP        0x02
#define WIFI_MODE_AP_STA    0x03

#define WIFI_SK_UDP         0x00
#define WIFI_SK_TCP_CLIENT  0x01
#define WIFI_SK_TCP_SEVER   0x02


#define MODEM_MIN(a , b) ((a < b) ? a : b)          // ��ȡ�������е���Сֵ
// ��������ʱ�ӽ���֮��������(TIME1,TIME2Ϊϵͳʱ�ӽ��ļ�����,���ֵΪ0xFFFFFFFF)
#define TIME_DIFF_MS(TIME1,TIME2) ( (TIME1 > TIME2 ? 0xFFFFFFFF - TIME1 + TIME2 : TIME2 - TIME1) * (1000 / OS_TICKS_PER_SEC))

typedef struct
{
	uint8 status;
	uint8 mux_type;
	uint8 id;
	
}connet_status_type;

/*******************************************************************************************
** Function name: 	  wifi_task
** Descriptions:		  wifi  task
** Input parameters��  pdata:ָ��������
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void wifi_task( void *pdata );

#endif/*_WIFI_H_*/

