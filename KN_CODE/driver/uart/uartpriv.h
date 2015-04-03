/***********************************************Copyright (c)*********************************************
**                                Zhangzhou KENENG Equipment Co.,LTD.
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			    uartpriv.h
** Last modified Date:      2009-02-21
** Last Version:		    1.0
** Descriptions:		    uart私有h文件，仅供uart内部模块调用
**
**--------------------------------------------------------------------------------------------------------
** Created by:			    heshusheng
** Created date:		    2009-02-21
** Version:				    1.0
** Descriptions:		    该头文件在CONFIG.h中包括
**
**--------------------------------------------------------------------------------------------------------
** Modified by:		heshusheng		    
** Modified Date:		2009-02-20    
** Version:				 1.01   
** Descriptions:		 将输入参数由字符形式变为结构体形式省略了字符
**							串的解析工作。			
**
*********************************************************************************************************/
#ifndef _UARTPRIV_H_
#define _UARTPRIV_H_

#define UART1_REC_LEN        256
#define UART1_SEND_LEN       256

#define UART2_REC_LEN        256
#define UART2_SEND_LEN       256

#define UART4_REC_LEN        256
#define UART4_SEND_LEN       256


#define UART5_REC_LEN        256
#define UART5_SEND_LEN       256


typedef struct
{
	uint16 wpos;  //写入位置
	uint16 gpos;  //读取位置
	uint16 nlen;  //有效数据长度
	uint16 len;  //缓存长度
	
	uint8 *buf;
}uart_fifo_type;

/*********************************************************************************************************
   定义Uart器件数目(可能有扩展)
*********************************************************************************************************/
#define __UART_MAX_NUM    5                                             /*  LPC24xx有4个UART            */

/*********************************************************************************************************
   UART器件信息结构结构体,用户不可修改
*********************************************************************************************************/
struct __uart_info {                        
    volatile    uint32     *puiAddrBase;                                /*  uart寄存器基地址            */  
                 
                uint16      uiQueueReviceFifoLen;                       /*  软件接收FIFO长度            */
                uint16      uiQueueSendFifoLen;                         /*  软件发送FIFO长度            */

				uint8      uiUartId;                                   /*  用于记录自己的UART通道号    */
                uint8      uiUartState;                                /*  记录收发状态1表示发,0表示收 */
				
                uart_fifo_type  PdqSendBuf;                                 /*  发送队列                    */
                uart_fifo_type  PdqReviceBuf;                               /*  接收队列                    */
                                                                                                                       
}; 

typedef struct __uart_info   __UART_INFO;                               /*  定义类型                    */
typedef __UART_INFO         *__PUART_INFO;                              /*  定义指针                    */

#endif /*_UARTPRIV_H_*/
