/************************************************************
 * Copyright (c) 2009,���ݿ��ܵ������޹�˾
 * All rights reserved. 
 *
 * �ļ����ƣ�main.c
 * �ļ���ʶ�������ù���ƻ��� 
 * ����������������Ŀ��C�������
 *
 * ��ǰ�汾�� 1.0
 * �� �ߣ��ֶ���
 * �������ڣ� 2009-12-1
 *
 * �޸��ˣ�
 * �޸����ڣ�
 * �汾��
 * ������
 **********************************************************/
#include "config.h"

#define	MAINTSK_PRI 10
#define	MAINTSK_STK_SIZE 256
OS_STK	MAINTSK_STACK[MAINTSK_STK_SIZE];

extern void maintask( void *pdata );

//C �������
int main ( void )
{
	OSInit( );

	OSTaskCreate(maintask, NULL, &MAINTSK_STACK[MAINTSK_STK_SIZE - 1], MAINTSK_PRI);

	OSStart( );
}

