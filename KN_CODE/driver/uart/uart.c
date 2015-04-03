/***********************************************Copyright (c)*********************************************
**                                Guangzou ZLG-MCU Development Co.,LTD.
**
**                                       http://www.zlgmcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			    uart.c
** Last modified Date:      2007-10-15
** Last Version:		    1.0
** Descriptions:		    ���ڵ�C�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:			    wengshujie
** Created date:		    2007-10-15
** Version:				    1.0
** Descriptions:		    ��
**
**--------------------------------------------------------------------------------------------------------
** Modified by:		heshusheng
** Modified Date:		2009-02-20
** Version:				 1.01
** Descriptions:		 ������������ַ���ʽ��Ϊ�ṹ����ʽʡ�����ַ�
**							���Ľ���������
**
**--------------------------------------------------------------------------------------------------------
** Modified by:		heshusheng
** Modified Date:		2013-02-18
** Version:				 1.02
** Descriptions:		 ���봮��4����

*********************************************************************************************************/

#include "config.h"

#include "uart.h"
#include "uartpriv.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

/*********************************************************************************************************
   �������UART�����Ľṹ�壬�ж����������Ҫ��������ṹ��
*********************************************************************************************************/            
static __UART_INFO    __UART1Info;/*  ����UART��һ���ṹ��       */
static __UART_INFO    __UART2Info;
//static __UART_INFO    __UART3Info;
static __UART_INFO    __UART4Info;
static __UART_INFO    __UART5Info;

/*********************************************************************************************************
   ����ʹ��ָ������������ṹ��ָ��,����Ը���������չ
*********************************************************************************************************/
 __PUART_INFO    __GpuiUartInfoTab[__UART_MAX_NUM] = { &__UART1Info, &__UART2Info,
                                                      NULL, &__UART4Info, &__UART5Info};

/*********************************************************************************************************
   ���涨����UART����ֵַ�����������UART���������ڸ�λ�������Ӧ�Ļ���ַ����
*********************************************************************************************************/
const uint32    *__GuiUartBaseAddrTab[__UART_MAX_NUM] = {(uint32 *)USART1, (uint32 *)USART2,
                                                        (uint32 *)USART3, (uint32 *)UART4, (uint32 *)UART5};

/*********************************************************************************************************
   ���涨����UART�������FIFO��Сֵ�������������UART���������ڸ�λ�������Ӧ��ֵ����
*********************************************************************************************************/
const uint16    __GuiQueueReviceFifoTab[__UART_MAX_NUM] = {UART1_REC_LEN,
                                                           UART2_REC_LEN,
                                                           0,
                                                           UART4_REC_LEN,
                                                           UART5_REC_LEN};

/*********************************************************************************************************
   ���涨����UART�������FIFOֵ�������������UART���������ڸ�λ�������Ӧ��ֵ����
*********************************************************************************************************/
const uint16    __GuiQueueSendFifoTab[__UART_MAX_NUM] = {UART1_SEND_LEN, UART2_SEND_LEN,
                                                         0, UART4_SEND_LEN,
                                                         UART5_SEND_LEN};

static uint8 uart1_sbuf[UART1_SEND_LEN];
static uint8 uart2_sbuf[UART2_SEND_LEN];
static uint8 uart4_sbuf[UART4_SEND_LEN];
static uint8 uart5_sbuf[UART5_SEND_LEN];


static uint8 uart1_rbuf[UART1_REC_LEN];
static uint8 uart2_rbuf[UART2_REC_LEN];
static uint8 uart4_rbuf[UART4_REC_LEN];
static uint8 uart5_rbuf[UART5_REC_LEN];

uint8   *__GuiQueueSendBuf[__UART_MAX_NUM] = {uart1_sbuf, uart2_sbuf, 0, uart4_sbuf, uart5_sbuf};
uint8   *__GuiQueueRecBuf[__UART_MAX_NUM] = {uart1_rbuf, uart2_rbuf, 0, uart4_rbuf, uart5_rbuf};


/*********************************************************************************************************
 						��������
*********************************************************************************************************/
/*********************************************************************************************************
 						����ʵ��
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:           uartIsr
** Descriptions:            UART�жϷ������
** input parameters:        	PUartInfo:          ָ��uart��Ϣ�ṹ���ָ��
** Output parameters:       NONE
** Returned value:          ��
*********************************************************************************************************/
void __uartIsr (__PUART_INFO PUartInfo)
{
	USART_TypeDef *puiAddrBase;
	uart_fifo_type *ufifo;
	uint8 temp;
	
	puiAddrBase = (USART_TypeDef *)PUartInfo->puiAddrBase;
	if(USART_GetITStatus(puiAddrBase, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		ufifo = &(PUartInfo->PdqReviceBuf);
		temp = USART_ReceiveData(puiAddrBase);
		if (ufifo->nlen < ufifo->len)
		{
			ufifo->buf[ufifo->wpos] = temp;
			ufifo->nlen++;
			if (++ufifo->wpos == ufifo->len)
			{
				ufifo->wpos = 0;
			}
		}
	}

	if(USART_GetITStatus(puiAddrBase, USART_IT_TXE) != RESET)
	{   
		if (PUartInfo->uiUartState != 0x01)
		{
			PUartInfo->uiUartState = 0;
			USART_ITConfig(puiAddrBase, USART_IT_TXE, DISABLE);
		}
		else
		{
			ufifo = &(PUartInfo->PdqSendBuf);
			if (ufifo->nlen != 0)
			{
				temp = ufifo->buf[ufifo->gpos];
				ufifo->nlen--;
				if (++ufifo->gpos == ufifo->len)
				{
					ufifo->gpos = 0;
				}
				USART_SendData(puiAddrBase, temp);
			}
			else
			{
				PUartInfo->uiUartState = 0;
				USART_ITConfig(puiAddrBase, USART_IT_TXE, DISABLE);
			}
		}
	}
}

/*********************************************************************************************************
** Function name:           uartIsr
** Descriptions:            UART�жϷ������
** input parameters:        uiId      : ���豸��
** Output parameters:       NONE
** Returned value:          ��
*********************************************************************************************************/
void uartIsr (uint32 uiId)
{
	__uartIsr(__GpuiUartInfoTab[uiId]);
}

/*********************************************************************************************************
** Function name:           uart0Isr
** Descriptions:            UART0�ж���ں���
** input parameters:        NONE
** Output parameters:       NONE
** Returned value:          ��
********************************************************************************************************/
void USART1_IRQHandler (void)
{
    uartIsr(ID_UART1);                                                     /*  UART�жϹ�����           */
}

/*********************************************************************************************************
** Function name:           uart1Isr
** Descriptions:            UART1�ж���ں���
** input parameters:        NONE
** Output parameters:       NONE
** Returned value:          ��
*********************************************************************************************************/
void USART2_IRQHandler (void)
{
    uartIsr(ID_UART2);
}

/*********************************************************************************************************
** Function name:           uart2Isr
** Descriptions:            UART2�ж���ں���
** input parameters:        NONE
** Output parameters:       NONE
** Returned value:          ��
*********************************************************************************************************/
void USART3_IRQHandler (void)
{
    uartIsr(ID_UART3);
}

/*********************************************************************************************************
** Function name:           uart3Isr
** Descriptions:            UART3�ж���ں���
** input parameters:        NONE
** Output parameters:       NONE
** Returned value:          ��
*********************************************************************************************************/
void UART4_IRQHandler (void)
{
    uartIsr(ID_UART4);
}
/*********************************************************************************************************
** Function name:           uart3Isr
** Descriptions:            UART3�ж���ں���
** input parameters:        NONE
** Output parameters:       NONE
** Returned value:          ��
*********************************************************************************************************/
void UART5_IRQHandler (void)
{
	 uartIsr(ID_UART5);
}
/*********************************************************************************************************
** Function name:           uart_hw_init
** Descriptions:            ����Ӳ���ĳ�ʼ��
** input parameters:        uiId:       ���豸��
**
** Output parameters:       NONE
** Returned value:			1:    �����ɹ�
**                          0:       ����ʧ��
*********************************************************************************************************/
static int32 __uart_hw_init(uint32 uiId)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	switch( uiId )
	{
		case ID_UART1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE); //clock
			//tx  PA9
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			//rx  PA10
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			NVIC_EnableIRQ(USART1_IRQn);
			break;
			
		case ID_UART2:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//clock
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			
			//tx  PA2
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			//rx  PA3
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			NVIC_EnableIRQ(USART2_IRQn);
			break;
			
		case ID_UART3:
			#ifdef STM32F10X_HD
			#else
			#endif

			NVIC_EnableIRQ(USART3_IRQn);
			break;
			
		case ID_UART4:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE); //clock
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			
			//tx  PC10
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOC, &GPIO_InitStructure);

			//rx  PC11
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOC, &GPIO_InitStructure);

			//PA8  PWM
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			TIM_TimeBaseStructure.TIM_Prescaler = 0;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseStructure.TIM_Period = (72000000 / 38000 ) - 1;
			TIM_TimeBaseStructure.TIM_ClockDivision = 0;
			TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

			TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

			/* Channel 1 Configuration in PWM mode */
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
			TIM_OCInitStructure.TIM_Pulse = (( 5 * (((72000000 / 38000 ) - 1) - 1)) / 10);
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
			TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
			TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
			TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

			TIM_OC1Init(TIM1, &TIM_OCInitStructure);

			/* TIM1 counter enable */
  			TIM_Cmd(TIM1, ENABLE);

			NVIC_EnableIRQ(UART4_IRQn);
			break;

		case ID_UART5:
			RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD), ENABLE); //clock
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
			
			//tx  PC12
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOC, &GPIO_InitStructure);

			//rx  PD2
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOD, &GPIO_InitStructure);

			//en PB5
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
			
			NVIC_EnableIRQ(UART5_IRQn);
			break;
			
		default:
			return UART_NOK;
			//break;
	}
	return UART_OK;
}

/*********************************************************************************************************
** Function name:			__uartMode
** Descriptions:			��ʼ��UART��ģʽ����
** input parameters:		PUartInfo:          ָ��uart��Ϣ�ṹ���ָ��
**                          uiParaData:         ����ָ��
** Output parameters:       NONE
** Returned value:			1:    �����ɹ�
**                          0:       ����ʧ��
*********************************************************************************************************/
static int32 __uartMode (__PUART_INFO PUartInfo, uart_param_type *uiParaData)
{
	USART_TypeDef *puiAddrBase;
	USART_InitTypeDef USART_InitStructure;

	puiAddrBase = (USART_TypeDef *)PUartInfo->puiAddrBase;	                            /*  ��ȡ�ṹ�����              */
                                                                  
	
	USART_InitStructure.USART_BaudRate = uiParaData->baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	if (uiParaData->parity == 0)
	{
		USART_InitStructure.USART_Parity = USART_Parity_No;
	}
	else if (uiParaData->parity == 1)
	{
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
	}
	else
	{
		USART_InitStructure.USART_Parity = USART_Parity_Even;
	}
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(puiAddrBase, &USART_InitStructure);


	return UART_OK;
}
/*********************************************************************************************************
** Function name:			__uartInit
** Descriptions:			��ʼ��UART,�˺�����UartInit����,���ṩ���û�
** input parameters:		PUartInfo:          ָ��uart��Ϣ�ṹ���ָ��
**                          uiParaData:         ����ָ��
**                          uiReBufSize:        uart�Ľ��ջ�������С
**                          uiSeBufSize:        uart�ķ��ͻ�������С
** Output parameters:       NONE
** Returned value:			1:    �����ɹ�
**                          0:       ����ʧ��
*********************************************************************************************************/
static uint32 __uartInit (__PUART_INFO PUartInfo,
                          uart_param_type *uiParaData)
{
    USART_TypeDef *puiAddrBase;
	uint32 uiId;

	puiAddrBase = (USART_TypeDef *)PUartInfo->puiAddrBase;
	uiId = PUartInfo->uiUartId;

	if (UART_NOK == (__uart_hw_init(uiId))) {
    	return UART_NOK;
    }

    if(UART_NOK == (__uartMode(PUartInfo,uiParaData))) {
    	return UART_NOK;
    }

	USART_ITConfig(puiAddrBase, USART_IT_RXNE, ENABLE);


  	/* Enable the USARTy */
  	USART_Cmd(puiAddrBase, ENABLE);

    return UART_OK;
}

/*********************************************************************************************************
** Function name:           __uart_qinit
** Descriptions:            ���ڶ��г�ʼ�� 
** input parameters:        data:��ʼ����������
** Output parameters:       NONE
** Returned value:			1:    �����ɹ�
**                          0:       ����ʧ��
*********************************************************************************************************/
static void __uart_qinit(__PUART_INFO Puartinfo)
{
	uart_fifo_type *tfifo;
	
	tfifo = &(Puartinfo->PdqReviceBuf);
	tfifo->wpos = 0;
	tfifo->gpos = 0;
	tfifo->nlen = 0;
	tfifo->len = Puartinfo->uiQueueReviceFifoLen;
	tfifo->buf = __GuiQueueRecBuf[Puartinfo->uiUartId];

	tfifo = &(Puartinfo->PdqSendBuf);
	tfifo->wpos = 0;
	tfifo->gpos = 0;
	tfifo->nlen = 0;
	tfifo->len = Puartinfo->uiQueueSendFifoLen;
	tfifo->buf = __GuiQueueSendBuf[Puartinfo->uiUartId];

	return;
}

/*********************************************************************************************************
** Function name:           uartInit
** Descriptions:            ���ڳ�ʼ�����ж�����
** input parameters:        data:��ʼ����������
** Output parameters:       NONE
** Returned value:			1:    �����ɹ�
**                          0:       ����ʧ��
*********************************************************************************************************/
void uartInit( void *data )
{
    uart_param_type param;
	INT32S *pbuf;
    uint32 uiId;

	pbuf = (INT32S *)data;
	 uiId = *pbuf++;
    if ( uiId <__UART_MAX_NUM) {
      	memcpy(&param,(void *)pbuf,sizeof(uart_param_type));	/*  ���Ʋ��������ػ���*/	

        __GpuiUartInfoTab[uiId]->puiAddrBase = (uint32 *)__GuiUartBaseAddrTab[uiId];
                                                                        /*  ����ַ                      */

        __GpuiUartInfoTab[uiId]->uiQueueReviceFifoLen = __GuiQueueReviceFifoTab[uiId];
                                                                        /*  ���FIFO���ճ���            */

        __GpuiUartInfoTab[uiId]->uiQueueSendFifoLen = __GuiQueueSendFifoTab[uiId];
                                                                        /*  ���FIFO���ͳ���            */
		__GpuiUartInfoTab[uiId]->uiUartId =  uiId;                      /*  ��¼���Լ���ID��            */


	   __uart_qinit(__GpuiUartInfoTab[uiId]);                          /*���г�ʼ��*/
                                                                        
        __uartInit(__GpuiUartInfoTab[uiId], &param);                   /*���ڳ�ʼ��*/
    }
}

/*********************************************************************************************************
** Function name:           uartSetMode
** Descriptions:            ���ô��ڵĲ���
** input parameters:        uiId      : ���豸��
**                          uiCmd     :���ò�����:CTMODE--����ģʽ;BAUDRATE--���ڲ�����
**                          pcArg     :   ����ָ��
** Output parameters:       NONE
** Returned value:			1:    �����ɹ�
**                          0:       ����ʧ��
*********************************************************************************************************/
extern int32 uartSetMode (uint32 uiId, uart_param_type *pcArg)
{

    uart_param_type param;

    if (uiId<__UART_MAX_NUM) {
        memcpy( &param,pcArg, sizeof(uart_param_type));	/*  ���Ʋ��������ػ���*/

		__uartMode(__GpuiUartInfoTab[uiId], &param);
    }
	return UART_NOK;
}

/*********************************************************************************************************
** Function name:			_uartQueueStatus
** Descriptions:			��ȡQueue���е�״̬
** input parameters:		PUartInfo      : ָ��uart��Ϣ�ṹ���ָ��
**                          uiWhichFifo	  : 1-���Ͷ���;0-���ն���
** Returned value:			���Ͷ����пɲ����Ŀռ��С������ն����п������ݸ���
*********************************************************************************************************/
static int32 _uartQueueStatus (__PUART_INFO PUartInfo, uint32 uiWhichFifo)
{
	//uint16 tlen;
	
	if ((uiWhichFifo == TX_FIFO))
	{
		return (PUartInfo->PdqSendBuf.len - PUartInfo->PdqSendBuf.nlen);
	}
	if (uiWhichFifo == RX_FIFO)
	{
		return PUartInfo->PdqReviceBuf.nlen;
	}
	
	return 0;
}

/*********************************************************************************************************
** Function name:           uartFifoStatus
** Descriptions:            ��ȡQueue���е�״̬
** input parameters:        uiId:           ���豸��
**                          uiWhichFifo:    TX_FIFO-���Ͷ���;RX_FIFO-���ն���
** Returned value:          ���Ͷ����пɲ����Ŀռ��С������ն����п������ݸ���
*********************************************************************************************************/
extern int32 uartFifoStatus (uint32 uiId,uint32 uiWhichFifo)
{
	if (uiId < __UART_MAX_NUM)
		return _uartQueueStatus(__GpuiUartInfoTab[uiId], uiWhichFifo);
	else
		return 0;
}

/*********************************************************************************************************
** Function name:           uartFifoFlush
** Descriptions:            ��ս��ջ���Queue����
** input parameters:        uiId:        UART���豸��
**                          uiWhichFifo: TX_FIFO-���Ͷ���;RX_FIFO-���ն���
** Output parameters:       NONE
** Returned value:			1:    �����ɹ�
**                          0:       ����ʧ��
*********************************************************************************************************/
extern int32 uartFifoFlush (uint32 uiId, uint32 uiWhichFifo)
{
	if (uiId > __UART_MAX_NUM) 
	{
		return UART_NOK;
	}
	else if (uiWhichFifo == RX_FIFO) 
	{
		__GpuiUartInfoTab[uiId]->PdqReviceBuf.gpos = 0;/*  ��ս��ն���                */
		__GpuiUartInfoTab[uiId]->PdqReviceBuf.wpos = 0;
		__GpuiUartInfoTab[uiId]->PdqReviceBuf.nlen = 0;
	}
	else if (uiWhichFifo == TX_FIFO) 
	{
		__GpuiUartInfoTab[uiId]->PdqSendBuf.gpos = 0;/*  ��շ��Ͷ���                */
		__GpuiUartInfoTab[uiId]->PdqSendBuf.wpos = 0;
		__GpuiUartInfoTab[uiId]->PdqSendBuf.nlen = 0;
	}


	return UART_OK;
}

/*********************************************************************************************************
** Function name:           uartRead
** Descriptions:            �Ӵ����豸��ȡ����
** input parameters:        uiId:        ���豸��
**                          puiBuf:      ���淵�����ݵ��ַ���ָ���ַ
**                          uiNum:       ��ȡ�����ݸ���
**                          pRsv:        ��������
** Output parameters:       puiBuf:      �����������׵�ַ
** Returned value:          ʵ�ʶ�ȡ�����ݸ���
*********************************************************************************************************/
extern int32   uartRead (uint32 uiId,
                         uint8 *puiBuf,
                         uint32 uiNum)
{
	uint32 tlen;
	uint32 uiReviceNum = 0;
	uart_fifo_type *ufifo;
	
	if (uiId < __UART_MAX_NUM) 
	{
		ufifo = &(__GpuiUartInfoTab[uiId]->PdqReviceBuf);
		if (uiNum >= ufifo->nlen)
		{
			uiReviceNum = ufifo->nlen;
		}	
		else
		{
			uiReviceNum = uiNum;
		}
		if((ufifo->gpos + uiReviceNum) >= ufifo->len)
		{
			tlen = (ufifo->len - ufifo->gpos);
			memcpy(puiBuf, &(ufifo->buf[ufifo->gpos]), tlen);
			ufifo->gpos = 0;
			puiBuf = &(puiBuf[tlen]);
			memcpy(puiBuf, &(ufifo->buf[ufifo->gpos]), uiReviceNum - tlen);
			ufifo->gpos = (uiReviceNum - tlen);
		}
		else
		{
			memcpy(puiBuf, &(ufifo->buf[ufifo->gpos]), uiReviceNum);
			ufifo->gpos += uiReviceNum;
		}
		ufifo->nlen -= uiReviceNum;
	}
	
	return uiReviceNum;
}

/*********************************************************************************************************
** Function name:			__uartWrite
** Descriptions:			���Ͷ���ֽ�����
** input parameters:		PUartInfo:      ָ��uart��Ϣ�ṹ���ָ��
**                          puiData:        Ҫ���͵����ݵ��׵�ַ
**                          uiNumByte:      �������ݵĸ���
** Output parameters:       NONE
** Returned value:			�ɹ��������ݵĸ���
*********************************************************************************************************/
static uint16 __uartWrite (__PUART_INFO PUartInfo, uint8 *puiData, uint32 uiNumByte)
{
	uart_fifo_type *ufifo;
    USART_TypeDef *puiAddrBase;
    uint32 uiSendNumbyte = 0;
	uint32 tlen;
//	uint8 temp;

	if (uiNumByte == 0)
	{
		return 0;
	}
	
	ufifo = &(PUartInfo->PdqSendBuf);
	if (ufifo->nlen != ufifo->len)
	{
		if (uiNumByte >= (ufifo->len - ufifo->nlen))
		{
			uiSendNumbyte = (ufifo->len - ufifo->nlen);
		}
		else
		{
			uiSendNumbyte = uiNumByte;
		}
		if((ufifo->wpos + uiSendNumbyte) >= ufifo->len)
		{
			tlen = (ufifo->len - ufifo->wpos);
			memcpy(&(ufifo->buf[ufifo->wpos]), puiData, tlen);
			ufifo->wpos = 0;
			puiData = &(puiData[tlen]);
			memcpy(&(ufifo->buf[ufifo->wpos]), puiData, uiSendNumbyte - tlen);
			ufifo->wpos = (uiSendNumbyte - tlen);
		}
		else
		{
			memcpy(&(ufifo->buf[ufifo->wpos]), puiData, uiSendNumbyte);
			ufifo->wpos += uiSendNumbyte;
		}
		ufifo->nlen += uiSendNumbyte;

		if (PUartInfo->uiUartState == 0)
		{
			PUartInfo->uiUartState = 1;
			puiAddrBase = (USART_TypeDef *)PUartInfo->puiAddrBase;
			USART_ITConfig(puiAddrBase, USART_IT_TXE, ENABLE);
		}
	}
	return uiSendNumbyte;
}
/*********************************************************************************************************
** Function name:           uartWrite
** Descriptions:            �򴮿��豸д����
** input parameters:        uiId:       ���豸��
**                          puiBuf:     ������ݵ��ַ���ָ���ַ
**                          uiNum:      һ��д������ݸ���
**                          pRsv:       ��������
** Output parameters:       NONE
** Returned value:          ʵ�ʷ��͵����ݸ���
*********************************************************************************************************/
extern int32    uartWrite (uint32 uiId,
                           uint8 *puiBuf,
                           uint32 uiNum)
{
	if (uiId < __UART_MAX_NUM){
		return (__uartWrite(__GpuiUartInfoTab[uiId], puiBuf, uiNum));
	}
	else
		return 0;
}

#if 0
extern void uart_change2io( uint8 type)
{
#if 1
	if (type == 0x00)
	{
		LPC_IOCON->P3_16  &= ~0x1f;   //p3.16 ;p3.17
		LPC_IOCON->P3_16  |= (0x01<<3);   //p3.16 ;p3.17
		LPC_IOCON->P3_17  &= ~0x1f;
		LPC_IOCON->P3_17  |= (0x01<<3);   //p3.16 ;p3.17
	}
	else
	{
		LPC_IOCON->P3_16  &= ~0x07;   //p3.16 ;p3.17
        LPC_IOCON->P3_16  |= 3;  
		LPC_IOCON->P3_16  &= ~0x18;   //p3.16 ;p3.17
		LPC_IOCON->P3_16  |= (0x02<<3);   //p3.16 ;p3.17
        LPC_IOCON->P3_17  &= ~0x07;
        LPC_IOCON->P3_17  |= 3;   
		LPC_IOCON->P3_17  &= ~0x18;   //p3.16 ;p3.17
		LPC_IOCON->P3_17  |= (0x02<<3);  //p3.16 ;p3.17
	}
#endif
}
#endif
/*********************************************************************************************************
   END FILE
*********************************************************************************************************/


