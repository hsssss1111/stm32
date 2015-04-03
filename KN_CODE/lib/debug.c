/****************************************Copyright (c)****************************************************
**                         							ZhangZhou KENENG electronics Co.,LTD.
**
**
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			debug.c
** Last modified Date:  2012-12-10
** Last Version:		1.0
** Descriptions:		just now we use the UART1 for debug port.we send the debug messages
**							to the debug port.
**
**--------------------------------------------------------------------------------------------------------
** Created by:			heshusheng
** Created date:		2012-12-10
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
#include "debug.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#ifdef _DEBUG_

//#define PORT_5
static char buffer[1024];

static OS_EVENT *debugmutex;


#ifdef PORT_5
/*********************************************************************************************************
** 函数名称: UART2_puts
** 功能描述: UART发送函数。
** 输入参数: SendBuf	发送缓冲区
** 输出参数: 无
********************************************************************************************************/
static void UART5_puts(char *SendBuf )
{
	while( *SendBuf != '\0' )
	{
		USART_SendData(UART5, *SendBuf++);
		while(!USART_GetFlagStatus(UART5, USART_FLAG_TC));
	}
}
#else
/*********************************************************************************************************
** 函数名称: UART1_puts
** 功能描述: UART发送函数。
** 输入参数: SendBuf	发送缓冲区
** 输出参数: 无
********************************************************************************************************/
static void UART1_puts(char *SendBuf )
{
	while( *SendBuf != '\0' )
	{
		USART_SendData(USART1, *SendBuf++);
		while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	}
}
#endif

/*********************************************************************************************************
** 函数名称: DEBUG
** 功能描述: 打印调试信息到串口1
** 输入参数: 待输出的调试信息
** 输出参数: 无
********************************************************************************************************/
int DEBUG(char *fmt, ...)
{
	va_list argptr;
	int cnt;
	INT8U err;
	
	OSMutexPend( debugmutex,0,&err);
	va_start(argptr, fmt);
	memset(buffer, 0x0, sizeof(buffer));
	cnt = vsprintf(buffer, fmt, argptr);
#ifdef PORT_5
	UART5_puts(buffer);
#else
	UART1_puts(buffer);
#endif
	va_end(argptr);
	OSMutexPost( debugmutex );
	return(cnt);
}

/*********************************************************************************************************
** 函数名称: debug_init
** 功能描述: 调试串口初始化
** 输入参数: 无
** 输出参数: 无
********************************************************************************************************/
void debug_init( void *param)
{
	uint8 err;
	USART_InitTypeDef	USART_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	debugmutex = OSMutexCreate(7, &err);

#ifdef PORT_5
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	//IO配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;		/* TXIO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(GPIOC, &GPIO_InitStructure); 		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		/* RXIO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//对UART5口的初始化
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART5, &USART_InitStructure);
	USART_Cmd(UART5, ENABLE);	
#else
	RCC_APB2PeriphClockCmd((1<<14)|(1<<2)|(1<<0), ENABLE); //clock
	//tx  PA9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//rx  PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//对UART1口的初始化
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);	
#endif
	DEBUG("debug init success!\r\n");
}
#else
/*********************************************************************************************************
** 函数名称: DEBUG
** 功能描述: 打印调试信息到串口1
** 输入参数: 待输出的调试信息
** 输出参数: 无
********************************************************************************************************/
int DEBUG(char *fmt, ...)
{
	return 0;
}

/*********************************************************************************************************
** 函数名称: debug_init
** 功能描述: 调试串口初始化
** 输入参数: 无
** 输出参数: 无
********************************************************************************************************/
void debug_init( void *param)
{
	return;
}

#endif
