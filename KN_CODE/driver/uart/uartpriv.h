/***********************************************Copyright (c)*********************************************
**                                Zhangzhou KENENG Equipment Co.,LTD.
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			    uartpriv.h
** Last modified Date:      2009-02-21
** Last Version:		    1.0
** Descriptions:		    uart˽��h�ļ�������uart�ڲ�ģ�����
**
**--------------------------------------------------------------------------------------------------------
** Created by:			    heshusheng
** Created date:		    2009-02-21
** Version:				    1.0
** Descriptions:		    ��ͷ�ļ���CONFIG.h�а���
**
**--------------------------------------------------------------------------------------------------------
** Modified by:		heshusheng		    
** Modified Date:		2009-02-20    
** Version:				 1.01   
** Descriptions:		 ������������ַ���ʽ��Ϊ�ṹ����ʽʡ�����ַ�
**							���Ľ���������			
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
	uint16 wpos;  //д��λ��
	uint16 gpos;  //��ȡλ��
	uint16 nlen;  //��Ч���ݳ���
	uint16 len;  //���泤��
	
	uint8 *buf;
}uart_fifo_type;

/*********************************************************************************************************
   ����Uart������Ŀ(��������չ)
*********************************************************************************************************/
#define __UART_MAX_NUM    5                                             /*  LPC24xx��4��UART            */

/*********************************************************************************************************
   UART������Ϣ�ṹ�ṹ��,�û������޸�
*********************************************************************************************************/
struct __uart_info {                        
    volatile    uint32     *puiAddrBase;                                /*  uart�Ĵ�������ַ            */  
                 
                uint16      uiQueueReviceFifoLen;                       /*  �������FIFO����            */
                uint16      uiQueueSendFifoLen;                         /*  �������FIFO����            */

				uint8      uiUartId;                                   /*  ���ڼ�¼�Լ���UARTͨ����    */
                uint8      uiUartState;                                /*  ��¼�շ�״̬1��ʾ��,0��ʾ�� */
				
                uart_fifo_type  PdqSendBuf;                                 /*  ���Ͷ���                    */
                uart_fifo_type  PdqReviceBuf;                               /*  ���ն���                    */
                                                                                                                       
}; 

typedef struct __uart_info   __UART_INFO;                               /*  ��������                    */
typedef __UART_INFO         *__PUART_INFO;                              /*  ����ָ��                    */

#endif /*_UARTPRIV_H_*/
