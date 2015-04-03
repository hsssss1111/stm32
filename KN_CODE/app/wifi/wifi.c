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
#include "config.h"
#include "wifi.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include ".\kn_code\driver\uart\uart.h"


#define RECDATABUFF 2048

const char *chn_type[2] = {"TCP", "UDP"};

static uint8 wifi_mode;
static uint8 sok_type;

//wifi  ����
static char ssip_name[30];
static char ssip_pw[30];

//TCP ����
static uint16 local_port;
static uint16 sever_port;
static uint8 sever_ip[4];

static connet_status_type connect_status;

static uint8 wifi_status = 0xff;
static uint8 _recdata[RECDATABUFF];      // ��������

/*******************************************************
 * ��������:ִ��AT����
 * ��������:modem_at_command
 * �������:<pCmd>[uint8 *] AT����
 * �������:<u16CmdLen>[uint16] AT�����
 * �������:<pAnswer>[uint8 *] Ӧ������
 * �������:<u16US>[uint16] �ȴ�ATָ��Ӧ��ʱ��(us)
 * �������:<u16Trys>[uint16] ���Դ���
 * ��   ��  ֵ:>=1�ɹ�,0ʧ��
 *******************************************************/
static uint32 wifi_at_command(char *pCmd, uint8 *pAnswer, uint32 u16US, uint16 u16Trys)
{
	uint8 _u8Rst = 0; // ���ؽ��
	int32 _s32RecvLen = 0; // ���ݳ���
	uint32 _u16TotalCnt = 0; // �ܽ������ݳ���
	uint32 _u16DelayTicks = 0; // �ӳ�ʱ��
	uint32 _u16Runs = 0; // ִ�д���
	uint32 _u32StartTick = 0; // ��ʼʱ��
	uint32 _u32CurTick = 0; // ��ǰʱ��
	uint8 trec[30];
	
	// �ӳ�ticks��
	_u16DelayTicks = ((INT32U) u16US / 1000) * 100L;

	// ִ������
	for (_u16Runs = 0; _u16Runs < u16Trys; _u16Runs++)
	{
		_u16TotalCnt = 0; // �������ݳ�����0
		memset(trec, 0x00, sizeof(trec));
		uartFifoFlush(ID_UART2, RX_FIFO);// �����ڻ������е��������
		uartWrite(ID_UART2, pCmd, strlen(pCmd));// д��ATָ��
		OSTimeDly(5); // �ӳ�5ms
		_u32StartTick = OSTimeGet(); // ��ʼʱ��
		_u32CurTick = _u32StartTick; // ��ǰʱ��

		// ����ȴ�Ӧ��,ֱ�ӷ���
		if (pAnswer == NULL)
		{
			return 1;
		}

		// �ȴ�Ӧ��
		while ((TIME_DIFF_MS(_u32StartTick, _u32CurTick) < _u16DelayTicks))
		{
			if ((_s32RecvLen = uartFifoStatus(ID_UART2, RX_FIFO)) > 0)
			{
				_s32RecvLen = MODEM_MIN(_s32RecvLen, (2048 - _u16TotalCnt));

				if (_s32RecvLen > 0)
				{
					_s32RecvLen = uartRead(ID_UART2, &trec[_u16TotalCnt],_s32RecvLen);
					_u16TotalCnt += _s32RecvLen;
				}
				else if (_s32RecvLen <= 0)
				{
					break;
				}

				if (memstr(trec, _u16TotalCnt, pAnswer) != NULL)
				{
					_u8Rst = 1;
					memcpy(pAnswer, &trec[0], _u16TotalCnt);
					return (_u16TotalCnt == 0 ? _u8Rst : _u16TotalCnt);
				}

				if (memstr(trec, _u16TotalCnt, "ERROR") != NULL)
				{
					_u8Rst = 0;
					break;
				}

				if (memstr(trec, _u16TotalCnt, "CONNECT FAIL") != NULL)
				{
					_u8Rst = 0;
					break;
				}

				// ����������,ʧ��
				if (_u16TotalCnt == sizeof(trec))
				{
					_u8Rst = 0;
					break;
				}
			}

			dog_report(OSTCBCur->OSTCBPrio);
			OSTimeDly(5); // �ӳ�5ms
			_u32CurTick = OSTimeGet(); // ��ǰʱ��
		}
	}

	if (_u16TotalCnt > 0)
	{
		memset(pAnswer, 0, (_u16TotalCnt + 1));
		memcpy(pAnswer, &trec[0], _u16TotalCnt);
	}

	return 0;
}


//Ӳ����λ
void wifi_reset( void )
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);//Ϩ��
	OSTimeDly(100);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);//Ϩ��
}

//��½ĳ��ssid
int8 wifi_connect(char *ssid, char *pw)
{
	char tbuf[50];
	char rbuf[20];
	
	sprintf(tbuf, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pw);
	strcpy((char *) rbuf, "OK\r\n");
	if (wifi_at_command(tbuf, &rbuf[0], 100000, 1) == 0)
	{
		DEBUG("��½ʧ��\r\n");
		return -1;
	}

	DEBUG("��½�ɹ�\r\n");
	return 0;
}

//�˳�ssid
int8 wifi_disc_socket( void )
{
	char rbuf[20];
	
	strcpy((char *) rbuf, "OK\r\n");
	if (wifi_at_command("AT+CWQAP\r\n", &rbuf[0], 10000, 2) == 0)
	{
		DEBUG("�Ͽ�����ʧ��\r\n");
		return -1;
	}

	DEBUG("�Ͽ����ӳɹ�\r\n");
	return 0;
}


int8 wifi_get_ipaddress(uint8 *ip)
{
	char rbuf[100];
	uint8 *_pstr1;
	
	strcpy((char *) rbuf, "OK\r\n");
	if (wifi_at_command("AT+CIFSR\r\n", &rbuf[0], 10000, 1) == 0)
	{
		DEBUG("��ȡIP��ַʧ��\r\n");
		return -1;
	}

	DEBUG("��ȡip��ַ�ɹ�\r\n");
	if ((_pstr1 = (uint8 *)memstr(rbuf, strlen(rbuf), (uint8 *)"+CIFSR:")) != NULL)
	{
		
	}
	//strcpy(ip, rbuf);
	//if ((_pstr1 = (uint8 *)strstr((char *) rbuf, "+CIFSR:")) != NULL)
	//{
	//	strcpy(ip, _pstr1);
	//}
	return 0;
}

/*******************************************************
 * ��������:дGPRS����
 * ��������:modem_write_gprs
 * �������:<smemid>[uint8] �����ڴ�ID
 * �������:<pSendBuf>[uint8 *] ���͵�����
 * �������:<u16SendCnt>[uint16] ���͵����ݳ���
 * ��   ��  ֵ:д�ɹ������ݳ���
 *******************************************************/
static uint16 wifi_write_data(uint8 *pSendBuf, uint16 u16SendCnt)
{
	uint8 _u8TryNum = 0;
	uint16 _u16RstLen = 0;
	uint16 _u16Written = 0;

	while (_u16Written != u16SendCnt && _u8TryNum < (u16SendCnt / 8))
	{
		_u16RstLen = uartWrite(ID_UART2, pSendBuf, u16SendCnt - _u16Written);
		pSendBuf += _u16RstLen;
		_u16Written += _u16RstLen;

		if (_u16Written != u16SendCnt)
		{
			_u8TryNum++;
			OSTimeDly(60);
		}
	}

	// д����ʧ��
	if (_u16Written != u16SendCnt)
	{
		DEBUG("MODEM дҵ������ʧ�ܣ��� %d, д%d, ���� %d \r\n", _u16Written, _u16RstLen, _u8TryNum);
	}

	return _u16Written;
}


//wifi ��������
int8 wifi_send_data(uint16 len, uint8 *data)
{
	char rbuf[50];
	char sbuf[30];
	uint16 _u16RstLen = 0;
	uint32 _u32StaTick = 0;

	if (connect_status.mux_type == 0)
	{
		sprintf(sbuf, " AT+CIPSEND=%d\r\n", len);
	}
	else
	{
		sprintf(sbuf, " AT+CIPSEND=%d,%d\r\n", connect_status.id, len);
	}
	
	strcpy((char *) rbuf, ">");
	if (wifi_at_command(sbuf, &rbuf[0], 20000, 1) == 0)
	{
		DEBUG("��������ʧ��!\r\n");
		return -1;
	}

	_u16RstLen += wifi_write_data(data, len);
	_u32StaTick = OSTimeGet();

	while ((TIME_DIFF_MS(_u32StaTick, OSTimeGet()) < 500))
	{
		if (uartFifoStatus(ID_UART2, RX_FIFO) > 0)
		{
			memset(rbuf, 0x0, sizeof(rbuf));
			uartRead(ID_UART2, &rbuf[0], 6);
			if (memcmp(&rbuf[0], "\r\nOK\r\n", 6) == 0) 
				break;
		}

		OSTimeDly(10);
	}
}


//wifi��������
int8 wifi_read_data(uint8 *id, uint16 *rtlen, uint8 *data)
{
	int32 _s32ValCnt = 0;
	uint8 *_pstr1 = NULL;

	uint8 *_pBuff = NULL;
	
	uint8 *_pRecBuff = &_recdata[0];
	int32 _s32CurCnt = 0;
	int32 _s32RecCnt = 0;
	char *pMarkChar = "+IPD,";
	char clen[8];
	uint8 i;
	
	while ((_s32CurCnt = uartFifoStatus(ID_UART2, RX_FIFO)) > 0)
	{
		_s32CurCnt = MODEM_MIN(_s32CurCnt, (RECDATABUFF - _s32RecCnt));
		if (_s32CurCnt <= 0) break;
		_s32RecCnt += uartRead(ID_UART2, &_pRecBuff[_s32RecCnt], _s32CurCnt);
		dog_report(OSTCBCur->OSTCBPrio);
		OSTimeDly(100);
	}
	
	// GPRS���ݴ���,��������������
	if (_s32RecCnt >= 4)
	{
		_pBuff = &_recdata[0];

		// ���������ַ�
		if ((_pstr1 = (uint8 *)memstr(_pBuff, (uint32)_s32RecCnt, (uint8 *)pMarkChar)) != NULL)
		{
			memset(clen, 0, 8);
			for (i = 0; i < 7; i++)
			{
				if (_pstr1[5+i] == ':')
				{
					break;
				}
				clen[i] = _pstr1[5+i];
			}
			app_atoi(rtlen, clen, 10, 1);
			if ((_pstr1 = (uint8 *)memstr(_pBuff, (uint32)_s32RecCnt, (uint8 *)":")) != NULL)
			{
				memcpy(data, &(_pstr1[1]), *rtlen);
			}
		}
		else if (memstr(_pBuff, (uint32)_s32RecCnt, (uint8 *)"CONNECT") != NULL)
		{
			
		}
	}
}

//��ʼ��APģ��
int8 wifi_init_ap(void )
{
	char apname[20];
	char appw[20];
	char rbuf[20];
	char sbuf[30];
	
	memset(rbuf, 0x0, sizeof(rbuf));
	strcpy(apname, "home-1107");
	strcpy(appw, "123456789");
	strcpy((char *) rbuf, "OK\r\n");
	sprintf(tbuf, "AT+CWSAP=\"%s\",\"%s\",%d,%d\r\n", apname, appw, 5, 3);
	if (wifi_at_command(sbuf, &rbuf[0], 5000, 3) == 0)
	{
		DEBUG("����AP��������\r\n");
	}

	memset(rbuf, 0x0, sizeof(rbuf));
	strcpy((char *) rbuf, "OK\r\n");
	strcpy(sbuf, "AT+CWDHCP=0,1\r\n");
	if (wifi_at_command(sbuf, &rbuf[0], 5000, 3) == 0)
	{
		DEBUG("����DHCP���ش���\r\n");
	}

	return 0;
}

//��ʼ��socket
int8 wifi_init_socket( void )
{
	char tbuf[50];
	char rbuf[20];
		
	if (sok_type == WIFI_SK_UDP)
	{
		sprintf(tbuf, "AT+CIPSTART=\"UDP\",\"%d.%d.%d.%d\",%d,%d\r\n", 
			sever_ip[0], sever_ip[1], 
			sever_ip[2], sever_ip[3], 
			sever_port, local_port);
		strcpy((char *) rbuf, "OK\r\n");
		if (wifi_at_command(tbuf, &rbuf[0], 100000, 1) == 0)
		{
			DEBUG("��½����ʧ��\r\n");
			return -1;
		}

		DEBUG("��½���ӳɹ�\r\n");
		return 0;
	}
	else if (sok_type == WIFI_SK_TCP_CLIENT)
	{
		sprintf(tbuf, "AT+CIPSTART=\"TCP\",\"%d.%d.%d.%d\",%d\r\n", 
			sever_ip[0], sever_ip[1], 
			sever_ip[2], sever_ip[3], 
			sever_port);
		strcpy((char *) rbuf, "OK\r\n");
		if (wifi_at_command(tbuf, &rbuf[0], 100000, 1) == 0)
		{
			DEBUG("��½����ʧ��\r\n");
			return -1;
		}

		DEBUG("��½���ӳɹ�\r\n");
		return 0;
	}
	else
	{
		// ����Ϊ������ģʽ
		memset(rbuf, 0x0, sizeof(rbuf));
		strcpy((char *) rbuf, "OK\r\n");
		if (wifi_at_command("AT+CIPMUX=1\r\n", &rbuf[0], 5000, 3) == 0)
		{
			DEBUG("����Ϊ������ģʽ����\r\n");
			return -1;
		}

		sprintf(tbuf, "AT+CIPSERVER=1,%d\r\n", local_port);
		strcpy((char *) rbuf, "OK\r\n");
		if (wifi_at_command(tbuf, &rbuf[0], 5000, 3) == 0)
		{
			DEBUG("����Ϊ������ģʽ����\r\n");
			return -1;
		}
	}
}


//wifi  ��ʼ��
int8 wifi_init( void )
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//PA4  rst
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA, GPIO_Pin_4);//Ϩ��

	wifi_reset( );

	OSTimeDly(2000);
}

//at ��ʼ��
int8 wifi_dev_init( uint8 mode )
{
	char rbuf[20];
	char sbuf[30];
	
	//ģ���Ƿ�׼������
	strcpy((char *) rbuf, "OK\r\n");
	if (wifi_at_command("AT\r\n", &rbuf[0], 1000, 10) == 0)
	{
		DEBUG("at ������Ч\r\n ");
		return -1;
	}

	// �رջ���
	memset(rbuf, 0x0, sizeof(rbuf));
	strcpy((char *) rbuf, "OK\r\n");
	if (wifi_at_command("ATE0\r\n", &rbuf[0], 5000, 3) == 0)
	{
		DEBUG("�رջ���ATE0����\r\n");
		return -1;
	}

	//����stationģʽ
	memset(rbuf, 0x0, sizeof(rbuf));
	strcpy((char *) rbuf, "OK\r\n");
	sprintf(sbuf, "AT+CWMODE=%d\r\n", mode);
	if (wifi_at_command(sbuf, &rbuf[0], 5000, 3) == 0)
	{
		DEBUG("����stationģʽ����\r\n");
	}

	return 0;
}

/*******************************************************************************************
** Function name: 	  wifi_task
** Descriptions:		  wifi  task
** Input parameters��  pdata:ָ��������
** Output parameters��
** Returned value:	  
*******************************************************************************************/
void wifi_task( void *pdata )
{
	uint8 ip[4];
	uint16 port;
	char ipaddr[40];
	char *sdata = "ddfsfasdfdsa";
	
	wifi_init( );

	wifi_dev_init( );

	wifi_disc_socket( );
	
	//wifi_connect("fzu1002510", "0000000000");


	//wifi_get_ipaddress(ipaddr);
	//DEBUG("��ǰip��ַ:%s\r\n", ipaddr);

	port = 0;
	for (;;)
	{
		wifi_read_data(&port, (uint8 *)ipaddr);
		if (port > 0)
		{
			wifi_send_data(port, ipaddr);
			port = 0;
		}
		
		OSTimeDly(10);
	}
}

