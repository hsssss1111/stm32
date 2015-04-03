/************************************************************
 * Copyright (c) 2009,���ݿ��ܵ������޹�˾
 * All rights reserved. 
 *
 * �ļ����ƣ�maintask.c
 * �ļ���ʶ�������ù���ƻ��� 
 * ������ ������������ģ��
 *
 * ��ǰ�汾�� 1.0
 * �����ߣ��ֶ���
 * �������ڣ� 2010-3-2
 *
 * �޸��ˣ�
 * �޸����ڣ�
 * �޸�ԭ��
 **********************************************************/

//*====================================================
//*������������������ͷ���ġ���
//*====================================================
#include  "config.h"
#include  "maintask.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

static uint8 bootmode = 0x00;
static char hello[1300] = {"dsdfdsafdfdsafdafdfasdfsddsf\r\n"};

extern void dog_kick( void );
extern void uart_change2io( uint8 type);
extern int8 version_poweron_check(void);



//*====================================================
//*������������������ ����������
//*====================================================


/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  RCC->APB2ENR |=  RCC_APB2ENR_IOPDEN;            /* Enable GPIOD clock       */

  GPIOD->ODR   &= ~0x00000004;                    /* switch off LEDs          */
  GPIOD->CRL   &= ~0x00000F00;                    /* PD.2  clear           */
  GPIOD->CRL   |=  0x00000300;                    /* PC.4..7  is output       */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  //if (num < LED_NUM) {
    GPIOD->BSRR = (1<<2);
  //}
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  //if (num < LED_NUM) {
    GPIOD->BRR = (1<<2);
  //}
}


/*********************************************OS_PRIO_SELF * ��������:ϵͳ��������
 * ��������:maintask
 * �������:<*pdata>[void]
 * �������:��
 * ��   ��  ֵ:��
 *******************************************************/
void maintask(void *pdata)
{
	int32 i;
	TSK_INFO *tsk_ptr;
	OS_STK *pbos;

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;


	ili9320_Initializtion();
	
	LED_Init();

	OS_ENTER_CRITICAL(); // ��ֹ�ж�

	// ����������Ϣ��������
	for (i = 0; drv_info[i].ini_entry != 0; i++)
	{
		drv_info[i].ini_entry(drv_info[i].param);
		dog_kick();
	}
	
	OS_EXIT_CRITICAL(); // �����ж�

	// ����������Ϣ�����ض�����������
	for (i = 0; pretsk_info[i].task != 0; i++)
	{
		tsk_ptr = (TSK_INFO*) &pretsk_info[i];
		pbos = (OS_STK*) (tsk_ptr->ptos);
		pbos += tsk_ptr->stk_size - 1;

		OSTaskCreateExt(tsk_ptr->task, (void *)0, pbos, tsk_ptr->prio,
				tsk_ptr->prio, tsk_ptr->ptos, tsk_ptr->stk_size, 0,
				0);

		//dog_kick( );
	}

	while(LED_get_init_status( ) == 0)
	{
		OSTimeDly(10);
	}

	

	// ����������Ϣ�����ض�����������
	for (i = 0; tsk_info[i].task != 0; i++)
	{
		tsk_ptr = (TSK_INFO*) &tsk_info[i];
		pbos = (OS_STK*) (tsk_ptr->ptos);
		pbos += tsk_ptr->stk_size - 1;

		OSTaskCreateExt(tsk_ptr->task, (void *)0, pbos, tsk_ptr->prio,
				tsk_ptr->prio, tsk_ptr->ptos, tsk_ptr->stk_size, 0,
				0);

		//dog_kick( );
	}


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
			
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = (72000000 / 38000 ) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (( 5 * (((72000000 / 38000 ) - 1) - 1)) / 10);
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	/* TIM1enable counter */
	//TIM_Cmd(TIM1, ENABLE);

	 /* TIM1 counter enable */
  	TIM_Cmd(TIM1, ENABLE);

	
	
	for (;;)
	{
		LED_output_state(LED_CMD_FLASH, LED_WARN);
		//TIM_CtrlPWMOutputs(TIM1, ENABLE);
		

		
		OSTimeDly(1000);

		LED_output_state(LED_CMD_FLASH, LED_WARN);
		//TIM_CtrlPWMOutputs(TIM1, DISABLE);
		
		OSTimeDly(1000);
	}
}

//��ȡ��ǰϵͳ������ģʽ
//��Ҫ�ṩ���������������ж���
//0--��������ģʽ
//1---����ģʽ����
int8 mt_get_bootmode( void )
{
	return bootmode;
}

void mt_refresh_bootmode( uint8 mode)
{
	bootmode = mode;

	return;
}

