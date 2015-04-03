/************************************************************
 * Copyright (c) 2009,���ݿ��ܵ������޹�˾
 * All rights reserved. 
 *
 * �ļ����ƣ�led.c
 * �ļ���ʶ�������ù���ƻ��� 
 * ������led����ʾ����
 *
 * ��ǰ�汾�� 1.0
 * �����ߣ�    heshusheng
 * �������ڣ� 2015-1-13
 *
 *
 **********************************************************/
//*====================================================
//*������������������ͷ���ġ���
//*====================================================
#include "config.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "led.h"
//*====================================================
//*��������������������������������
//*====================================================
static void *led_ptr[LED_QUEUE_LEN];
static int8 led_pool[LED_POOL_LEN];

static OS_EVENT *led_q;
static OS_MEM *led_free_pool;

//״̬���ƶ���
static led_status_type status[LED_ALL];
// ������ʾֵ
static uint8 led_buf[LED_ALL]; 

static uint8 rdy;	//��ʼ�����

//#define MK_STATE_MASK     (GPIO_PIN0_MASK | GPIO_PIN1_MASK | GPIO_PIN2_MASK) //ģ��״̬ʶ���ж�
                               
//*====================================================
//*������������������ ����������
//*====================================================
/*******************************************************
 * ��������:����arm����
 * ��������:led_ctl_prestatus
 * �������:LED_CTL_RSTJL�ȣ�
 * �������:
 * ��   ��  ֵ:
 *******************************************************/
void led_ctl_prestatus(uint8 type)
{
	switch(type)
	{		
		default:
			break;
	}
}

/*********************************************************************************************************
* ��    �ƣ�LED_get_chnstate
* ��    �ܣ���ȡ��ǰͨ����״̬
* ��ڲ���: idx:��Ӧ������
				pout:״̬���
							
* ���ڲ���:	-1ʧ��  0-�ɹ�
**********************************************************************************************************/
int8 LED_get_chnstate(uint8 idx, uint8 *pout)
{
	if (idx == YX_PROG)
	{
		*pout = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
	}

	return 0x00;
}

/*********************************************************************************************************
* ��    �ƣ�read_mk_state
* ��    �ܣ���ȡģ�鵱ǰ״̬  M_STATE2 | M_STATE1 | M_STATE0
* ��ڲ���: ��						
* ���ڲ���:	 0x07-��ģ�飬0x06-��ģ�飬��Ӧ��׼ATָ�0x04-��ģ�飬����ӦATָ��,����ֵtemp-Ԥ��
**********************************************************************************************************/
uint32 read_mk_state(void)
{
	return 0x06;
}

/*******************************************************
 * ��������:�����������ˢ�µ�LED��ʾ
 * ��������:led_update_display
 * �������:
 * �������:
 * ��   ��  ֵ:
 *******************************************************/
static void led_update_display( void )
{
	if (led_buf[LED_WARN] == 0x00)//PC2  �澯
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_13);//Ϩ��
		//GPIO_SetBits(GPIOD, GPIO_Pin_2);//Ϩ��
	}
	else
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);//����
		//GPIO_ResetBits(GPIOD, GPIO_Pin_2);//Ϩ��
	}
	if (led_buf[LED_ONLINE] == 0x00)//PC3
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_3);//Ϩ��
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_3);//����
	}
	if (led_buf[LED_GPRS_R] == 0x00)//PC7
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_7);//Ϩ��
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);//����
	}
	if (led_buf[LED_GPRS_S] == 0x00)//PC6
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_6);//Ϩ��
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);//����
	}
	if (led_buf[LED_485_R] == 0x00)//PB15
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_15);//Ϩ��
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);//����
	}

	if (led_buf[LED_485_T] == 0x00)//PB14
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_14);//Ϩ��
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);//����
	}

	if (led_buf[LED_SIG_R] == 0x00)//Pc9
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_9);//Ϩ��
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);//����
	}

	if (led_buf[LED_SIG_G] == 0x00)//Pc8
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_8);//Ϩ��
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);//����
	}
	if (led_buf[LED_JH_R] == 0x00)//PB13
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_13);//Ϩ��
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);//����
	}
	if (led_buf[LED_JH_S] == 0x00)//PB12
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);//Ϩ��
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);//����
	}

	return ;
}

/*********************************************************************************************************
* ��    �ƣ�LED_get_cmd_buf
* ��    �ܣ�������ؿռ�����ȡһ����洢�ռ�
* ��ڲ�����cmd:���������ַ
* ���ڲ�����0:�ɹ�;1:ʧ��
**********************************************************************************************************/
static int8 LED_get_cmd_buf(led_cmd_type **cmd)
{
	INT8U err;
	
	*cmd = (led_cmd_type *)OSMemGet(led_free_pool, &err);

	if ( *cmd == 0)
		return -1;

	return 0;
}

/*********************************************************************************************************
* ��    �ƣ�LED_free_cmd_buf
* ��    �ܣ��ͷ�����洢�ռ䵽�������
* ��ڲ�����cmd:���������ַ
* ���ڲ�����0:�ɹ�;1:ʧ��
**********************************************************************************************************/
static int8 LED_free_cmd_buf(led_cmd_type *cmd )
{
	if ( OSMemPut(led_free_pool, (void *)cmd) == OS_NO_ERR ) 
		return 0;

	return -1;
}

/*********************************************************************************************************
* ��    �ƣ�LED_put_cmd
* ��    �ܣ�����һ����� ledisp task ��������������
* ��ڲ�����cmd:���������ַ
* ���ڲ�����0:�ɹ�;1:ʧ��
**********************************************************************************************************/
static int8 LED_put_cmd(led_cmd_type *cmd)
{
	if (OSQPost(led_q,(void *)cmd) == OS_NO_ERR )//�����ڴ���е�ָ��
		return 0;

	return -1;
}

/*********************************************************************************************************
* ��    �ƣ�LED_get_init_status
* ��    �ܣ���ȡinit��ʼ��״̬
* ��ڲ���: ��
							
* ���ڲ���:	0-δ��ʼ���꣬1��ʼ��
**********************************************************************************************************/
int8 LED_get_init_status( void )
{
	return rdy;
}


/*********************************************************************************************************
* ��    �ƣ�LED_output_state
* ��    �ܣ�����LED���߷�����  (���ⲿģ�����)
* ��ڲ���: cmd:��/��/Ƶ��
				idx:��Ӧ��LED��id.
				last:����ʱ��  ���Ϊ0xffff,��̶�
				prv:˽�в���(������չ)
							
* ���ڲ���:	Ŀǰֻ����0
**********************************************************************************************************/
int8  LED_output_state(uint8 cmd, uint8 idx)
{
	led_cmd_type *pcmd;
	
	if (idx >= LED_ALL) 
	{
		return -1;
	}

	if (LED_get_cmd_buf(&pcmd) == 0x0)
	{
		pcmd->cmd = cmd;
		pcmd->index = idx;
		LED_put_cmd(pcmd);
		return 0;
	}

	return -1;
}



/*********************************************************************************************************
* ��    �ƣ�LED_dispinit
* ��    �ܣ�led�����ʼ��������������Ϣ���м�LED�ƿ��Ƴ�ʼ��
* ��ڲ�����NONE
* ���ڲ�����NONE
**********************************************************************************************************/
static void LED_dispinit(void)
{
	uint8 i;
	INT8U err;
	GPIO_InitTypeDef	GPIO_InitStructure;

	led_q = OSQCreate(led_ptr, LED_QUEUE_LEN);//������Ϣ���У�����ΪLED_QUEUE_LEN����Ϣ
	led_free_pool = OSMemCreate((void *)led_pool, LED_QUEUE_LEN, LED_BLK_LEN, &err);//�����ڴ���������ڱ�����Ϣ������LED_QUEUE_LEN���飬ÿ�����СΪLED_BLK_LEN���ֽ�
	
	/******************************ͨ�����Ƴ�ʼ��*********************************/
	
	/**********************************LED�ܽų�ʼ��*********************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		//PC2  WARN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(GPIOD, &GPIO_InitStructure); 		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		//PC3 ONLINE
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		//PC7  gprs_r
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		//PC6  gprs_s
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;		//PB15  485��������
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;		//PB14 485��������
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//PC9  �źź�
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		//PC8  �ź���
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		//PB13 ������
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;		//PB12 ������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//test
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//PD2 ������
	//GPIO_Init(GPIOD, &GPIO_InitStructure);

	
 /**************************���ͨ����ʼ��*******************************************/

/**********************************IO��δ�ùܽų�ʼ��**************************************/
	
	
/*************************************************************************************/

 /**********************************LED�ܽ�״̬��ʼ��*********************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		//PB12 ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	//��ʼ״̬Ϊ��
	for (i = 0; i < LED_ALL; i++)
	{
		status[i].on_off = 0x00;
		status[i].pstate = LED_STATE_OFF;
		status[i].left = 0xff;
		status[i].cont = 0xffff;
	}

	memset(led_buf, 0x00, LED_ALL);
	led_update_display( );

	
	rdy = 0x01;//��ʼ����
}

/*********************************************************************************************************
* ��    �ƣ�ledctl_task
* ��    �ܣ�ϵͳ�ƿ�������
* ��ڲ���: pdata:����˽������
							
* ���ڲ���:
**********************************************************************************************************/
void led_task( void *pdata)
{
	led_cmd_type *cmd;
	OS_Q_DATA q_data;
	INT16U num=0;
	INT8U err;
	uint8 i;
	uint8 change = 0x00;
	
	LED_dispinit( );//led�����ʼ��������������Ϣ���м�LED�ƿ��Ƴ�ʼ��
	
	for(;;)
	{
		OSQQuery(led_q,&q_data); 
		if ( (num = q_data.OSNMsgs) != 0) 
		{
			while(num != 0)
			{
				cmd = (led_cmd_type *)OSQPend(led_q, 0, &err);//������Ϣ����
				switch(cmd->cmd)
				{
					case LED_CMD_ON://���� 
						if (status[cmd->index].pstate != LED_STATE_ONR)
						{
							status[cmd->index].pstate = LED_STATE_ONR;
							status[cmd->index].on_off = 0x01;
							status[cmd->index].left = 0xff;
							status[cmd->index].cont = 0xffff;//�����೤ʱ��
							
							led_buf[cmd->index] = 0x01;
							change = 0x01;
						}
						break;
						
					case LED_CMD_OFF://���
						if (status[cmd->index].pstate != LED_STATE_OFF )
						{
							status[cmd->index].pstate = LED_STATE_OFF;
							status[cmd->index].on_off = 0x00;
							status[cmd->index].left = 0xff;
							status[cmd->index].cont = 0xffff;//�����೤ʱ��
							
							led_buf[cmd->index] = 0x00;
							change = 0x01;
						}
						break;

					case LED_CMD_FLASH://��Ƶ��
						if (status[cmd->index].pstate != LED_STATE_FHR)
						{
							status[cmd->index].pstate = LED_STATE_FHR;
							status[cmd->index].cont = 50;//�����೤ʱ��
							status[cmd->index].on_off = 0x00;
							
							led_buf[cmd->index] = 0x00;
							
							//Ƶ��
							status[cmd->index].left = 10;	
							
							change = 0x01;
						}
						break;
						
					default:
						break;
				}
				LED_free_cmd_buf(cmd);//�ͷ��ڴ��
				num--;
			}
		}

		
		
		for(i = 0; i < LED_ALL; i++)//��˸����ʱ
		{
			//���һ������ʱ��tick
			if (status[i].cont == 0x01)
			{
				status[i].cont = 0x00;
				
				status[i].on_off = 0x00;
				status[i].pstate = LED_STATE_OFF;
				led_buf[i] = 0x00;

				change = 0x01;
			}
			else if (status[i].cont == 0x00)
			{
				//do nothing.
			}
			else
			{
				//��ʱ�����Ƶ�
				if (status[i].cont != 0xffff)
				{
					status[i].cont--;
				}
				
				if( status[i].left != 0xff)
				{
					if (status[i].left != 0 )
						status[i].left--;
				    else
					{
						//�ֵ���
						if (status[i].on_off == 0x00)
						{
							status[i].on_off = 0x01;
							led_buf[i] = 0x01;
						}
						else
						{
							//�ֵ���
							status[i].on_off = 0x00;
							led_buf[i] = 0x00;
						}	
						
             			 //Ƶ��
						 status[i].left = 10;					
						 change = 0x01;
					 }
				}
			}
		}

		if(change == 0x01)//����LED״̬
		{
			led_update_display( );
			change = 0x00;
		}
		OSTimeDly(10);
	}
}

