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

//wifi  参数
static char ssip_name[30];
static char ssip_pw[30];

//TCP 参数
static uint16 local_port;
static uint16 sever_port;
static uint8 sever_ip[4];

static connet_status_type connect_status;

static uint8 wifi_status = 0xff;
static uint8 _recdata[RECDATABUFF];      // 接收数据

/*******************************************************
 * 函数功能:执行AT命令
 * 函数名称:modem_at_command
 * 输入参数:<pCmd>[uint8 *] AT命令
 * 输入参数:<u16CmdLen>[uint16] AT命令长度
 * 输入参数:<pAnswer>[uint8 *] 应答命令
 * 输入参数:<u16US>[uint16] 等待AT指令应答时间(us)
 * 输入参数:<u16Trys>[uint16] 重试次数
 * 返   回  值:>=1成功,0失败
 *******************************************************/
static uint32 wifi_at_command(char *pCmd, uint8 *pAnswer, uint32 u16US, uint16 u16Trys)
{
	uint8 _u8Rst = 0; // 返回结果
	int32 _s32RecvLen = 0; // 数据长度
	uint32 _u16TotalCnt = 0; // 总接收数据长度
	uint32 _u16DelayTicks = 0; // 延迟时间
	uint32 _u16Runs = 0; // 执行次数
	uint32 _u32StartTick = 0; // 起始时间
	uint32 _u32CurTick = 0; // 当前时间
	uint8 trec[30];
	
	// 延迟ticks数
	_u16DelayTicks = ((INT32U) u16US / 1000) * 100L;

	// 执行命令
	for (_u16Runs = 0; _u16Runs < u16Trys; _u16Runs++)
	{
		_u16TotalCnt = 0; // 接收数据长度置0
		memset(trec, 0x00, sizeof(trec));
		uartFifoFlush(ID_UART2, RX_FIFO);// 将串口缓冲区中的数据清空
		uartWrite(ID_UART2, pCmd, strlen(pCmd));// 写入AT指令
		OSTimeDly(5); // 延迟5ms
		_u32StartTick = OSTimeGet(); // 起始时间
		_u32CurTick = _u32StartTick; // 当前时间

		// 不需等待应答,直接返回
		if (pAnswer == NULL)
		{
			return 1;
		}

		// 等待应答
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

				// 缓冲区已满,失败
				if (_u16TotalCnt == sizeof(trec))
				{
					_u8Rst = 0;
					break;
				}
			}

			dog_report(OSTCBCur->OSTCBPrio);
			OSTimeDly(5); // 延迟5ms
			_u32CurTick = OSTimeGet(); // 当前时间
		}
	}

	if (_u16TotalCnt > 0)
	{
		memset(pAnswer, 0, (_u16TotalCnt + 1));
		memcpy(pAnswer, &trec[0], _u16TotalCnt);
	}

	return 0;
}


//硬件复位
void wifi_reset( void )
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);//熄灭
	OSTimeDly(100);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);//熄灭
}

//登陆某个ssid
int8 wifi_connect(char *ssid, char *pw)
{
	char tbuf[50];
	char rbuf[20];
	
	sprintf(tbuf, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pw);
	strcpy((char *) rbuf, "OK\r\n");
	if (wifi_at_command(tbuf, &rbuf[0], 100000, 1) == 0)
	{
		DEBUG("登陆失败\r\n");
		return -1;
	}

	DEBUG("登陆成功\r\n");
	return 0;
}

//退出ssid
int8 wifi_disc_socket( void )
{
	char rbuf[20];
	
	strcpy((char *) rbuf, "OK\r\n");
	if (wifi_at_command("AT+CWQAP\r\n", &rbuf[0], 10000, 2) == 0)
	{
		DEBUG("断开连接失败\r\n");
		return -1;
	}

	DEBUG("断开连接成功\r\n");
	return 0;
}


int8 wifi_get_ipaddress(uint8 *ip)
{
	char rbuf[100];
	uint8 *_pstr1;
	
	strcpy((char *) rbuf, "OK\r\n");
	if (wifi_at_command("AT+CIFSR\r\n", &rbuf[0], 10000, 1) == 0)
	{
		DEBUG("获取IP地址失败\r\n");
		return -1;
	}

	DEBUG("获取ip地址成功\r\n");
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
 * 函数功能:写GPRS数据
 * 函数名称:modem_write_gprs
 * 输入参数:<smemid>[uint8] 共享内存ID
 * 输入参数:<pSendBuf>[uint8 *] 发送的数据
 * 输出参数:<u16SendCnt>[uint16] 发送的数据长度
 * 返   回  值:写成功的数据长度
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

	// 写数据失败
	if (_u16Written != u16SendCnt)
	{
		DEBUG("MODEM 写业务数据失败：总 %d, 写%d, 次数 %d \r\n", _u16Written, _u16RstLen, _u8TryNum);
	}

	return _u16Written;
}


//wifi 发送数据
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
		DEBUG("发送数据失败!\r\n");
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


//wifi接收数据
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
	
	// GPRS数据处理,缓冲区中有数据
	if (_s32RecCnt >= 4)
	{
		_pBuff = &_recdata[0];

		// 查找特征字符
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

//初始化AP模块
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
		DEBUG("设置AP参数错误\r\n");
	}

	memset(rbuf, 0x0, sizeof(rbuf));
	strcpy((char *) rbuf, "OK\r\n");
	strcpy(sbuf, "AT+CWDHCP=0,1\r\n");
	if (wifi_at_command(sbuf, &rbuf[0], 5000, 3) == 0)
	{
		DEBUG("设置DHCP开关错误\r\n");
	}

	return 0;
}

//初始化socket
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
			DEBUG("登陆连接失败\r\n");
			return -1;
		}

		DEBUG("登陆连接成功\r\n");
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
			DEBUG("登陆连接失败\r\n");
			return -1;
		}

		DEBUG("登陆连接成功\r\n");
		return 0;
	}
	else
	{
		// 设置为多连接模式
		memset(rbuf, 0x0, sizeof(rbuf));
		strcpy((char *) rbuf, "OK\r\n");
		if (wifi_at_command("AT+CIPMUX=1\r\n", &rbuf[0], 5000, 3) == 0)
		{
			DEBUG("设置为多连接模式错误\r\n");
			return -1;
		}

		sprintf(tbuf, "AT+CIPSERVER=1,%d\r\n", local_port);
		strcpy((char *) rbuf, "OK\r\n");
		if (wifi_at_command(tbuf, &rbuf[0], 5000, 3) == 0)
		{
			DEBUG("设置为服务器模式错误\r\n");
			return -1;
		}
	}
}


//wifi  初始化
int8 wifi_init( void )
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//PA4  rst
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA, GPIO_Pin_4);//熄灭

	wifi_reset( );

	OSTimeDly(2000);
}

//at 初始化
int8 wifi_dev_init( uint8 mode )
{
	char rbuf[20];
	char sbuf[30];
	
	//模块是否准备就绪
	strcpy((char *) rbuf, "OK\r\n");
	if (wifi_at_command("AT\r\n", &rbuf[0], 1000, 10) == 0)
	{
		DEBUG("at 命令无效\r\n ");
		return -1;
	}

	// 关闭回显
	memset(rbuf, 0x0, sizeof(rbuf));
	strcpy((char *) rbuf, "OK\r\n");
	if (wifi_at_command("ATE0\r\n", &rbuf[0], 5000, 3) == 0)
	{
		DEBUG("关闭回显ATE0错误\r\n");
		return -1;
	}

	//设置station模式
	memset(rbuf, 0x0, sizeof(rbuf));
	strcpy((char *) rbuf, "OK\r\n");
	sprintf(sbuf, "AT+CWMODE=%d\r\n", mode);
	if (wifi_at_command(sbuf, &rbuf[0], 5000, 3) == 0)
	{
		DEBUG("设置station模式错误\r\n");
	}

	return 0;
}

/*******************************************************************************************
** Function name: 	  wifi_task
** Descriptions:		  wifi  task
** Input parameters：  pdata:指定参数；
** Output parameters：
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
	//DEBUG("当前ip地址:%s\r\n", ipaddr);

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

