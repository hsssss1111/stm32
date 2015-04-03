/************************************************************
 * Copyright (c) 2009,漳州科能电器有限公司
 * All rights reserved. 
 *
 * 文件名称：led.c
 * 文件标识：见配置管理计划书 
 * 描述：led灯显示驱动
 *
 * 当前版本： 1.0
 * 创建者：    heshusheng
 * 创建日期： 2015-1-13
 *
 *
 **********************************************************/
//*====================================================
//*　　　　　　　　　头　文　件
//*====================================================
#include "config.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "led.h"
//*====================================================
//*　　　　　　　　　常量、变量、宏
//*====================================================
static void *led_ptr[LED_QUEUE_LEN];
static int8 led_pool[LED_POOL_LEN];

static OS_EVENT *led_q;
static OS_MEM *led_free_pool;

//状态控制队列
static led_status_type status[LED_ALL];
// 缓存显示值
static uint8 led_buf[LED_ALL]; 

static uint8 rdy;	//初始化完毕

//#define MK_STATE_MASK     (GPIO_PIN0_MASK | GPIO_PIN1_MASK | GPIO_PIN2_MASK) //模块状态识别判断
                               
//*====================================================
//*　　　　　　　　　 函　数　区
//*====================================================
/*******************************************************
 * 函数功能:控制arm外设
 * 函数名称:led_ctl_prestatus
 * 输入参数:LED_CTL_RSTJL等；
 * 输出参数:
 * 返   回  值:
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
* 名    称：LED_get_chnstate
* 功    能：获取当前通道的状态
* 入口参数: idx:对应的索引
				pout:状态输出
							
* 出口参数:	-1失败  0-成功
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
* 名    称：read_mk_state
* 功    能：获取模块当前状态  M_STATE2 | M_STATE1 | M_STATE0
* 入口参数: 无						
* 出口参数:	 0x07-无模块，0x06-有模块，响应标准AT指令，0x04-有模块，不响应AT指令,其他值temp-预留
**********************************************************************************************************/
uint32 read_mk_state(void)
{
	return 0x06;
}

/*******************************************************
 * 函数功能:将缓存的数据刷新到LED显示
 * 函数名称:led_update_display
 * 输入参数:
 * 输出参数:
 * 返   回  值:
 *******************************************************/
static void led_update_display( void )
{
	if (led_buf[LED_WARN] == 0x00)//PC2  告警
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_13);//熄灭
		//GPIO_SetBits(GPIOD, GPIO_Pin_2);//熄灭
	}
	else
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);//点亮
		//GPIO_ResetBits(GPIOD, GPIO_Pin_2);//熄灭
	}
	if (led_buf[LED_ONLINE] == 0x00)//PC3
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_3);//熄灭
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_3);//点亮
	}
	if (led_buf[LED_GPRS_R] == 0x00)//PC7
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_7);//熄灭
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);//点亮
	}
	if (led_buf[LED_GPRS_S] == 0x00)//PC6
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_6);//熄灭
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);//点亮
	}
	if (led_buf[LED_485_R] == 0x00)//PB15
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_15);//熄灭
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);//点亮
	}

	if (led_buf[LED_485_T] == 0x00)//PB14
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_14);//熄灭
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);//点亮
	}

	if (led_buf[LED_SIG_R] == 0x00)//Pc9
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_9);//熄灭
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);//点亮
	}

	if (led_buf[LED_SIG_G] == 0x00)//Pc8
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_8);//熄灭
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);//点亮
	}
	if (led_buf[LED_JH_R] == 0x00)//PB13
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_13);//熄灭
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);//点亮
	}
	if (led_buf[LED_JH_S] == 0x00)//PB12
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);//熄灭
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);//点亮
	}

	return ;
}

/*********************************************************************************************************
* 名    称：LED_get_cmd_buf
* 功    能：从命令池空间中提取一命令存储空间
* 入口参数：cmd:命令参数地址
* 出口参数：0:成功;1:失败
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
* 名    称：LED_free_cmd_buf
* 功    能：释放命令存储空间到命令池中
* 入口参数：cmd:命令参数地址
* 出口参数：0:成功;1:失败
**********************************************************************************************************/
static int8 LED_free_cmd_buf(led_cmd_type *cmd )
{
	if ( OSMemPut(led_free_pool, (void *)cmd) == OS_NO_ERR ) 
		return 0;

	return -1;
}

/*********************************************************************************************************
* 名    称：LED_put_cmd
* 功    能：放置一条命令到 ledisp task 任务的命令队列中
* 入口参数：cmd:命令参数地址
* 出口参数：0:成功;1:失败
**********************************************************************************************************/
static int8 LED_put_cmd(led_cmd_type *cmd)
{
	if (OSQPost(led_q,(void *)cmd) == OS_NO_ERR )//发送内存块中的指针
		return 0;

	return -1;
}

/*********************************************************************************************************
* 名    称：LED_get_init_status
* 功    能：获取init初始化状态
* 入口参数: 无
							
* 出口参数:	0-未初始化完，1初始化
**********************************************************************************************************/
int8 LED_get_init_status( void )
{
	return rdy;
}


/*********************************************************************************************************
* 名    称：LED_output_state
* 功    能：控制LED或者蜂鸣器  (由外部模块调用)
* 入口参数: cmd:亮/灭/频闪
				idx:对应的LED灯id.
				last:持续时间  如果为0xffff,则固定
				prv:私有参数(用于扩展)
							
* 出口参数:	目前只返回0
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
* 名    称：LED_dispinit
* 功    能：led任务初始化，创建任务消息队列及LED灯控制初始化
* 入口参数：NONE
* 出口参数：NONE
**********************************************************************************************************/
static void LED_dispinit(void)
{
	uint8 i;
	INT8U err;
	GPIO_InitTypeDef	GPIO_InitStructure;

	led_q = OSQCreate(led_ptr, LED_QUEUE_LEN);//创建消息队列，长度为LED_QUEUE_LEN条消息
	led_free_pool = OSMemCreate((void *)led_pool, LED_QUEUE_LEN, LED_BLK_LEN, &err);//创建内存分区，用于保存消息，创建LED_QUEUE_LEN个块，每个块大小为LED_BLK_LEN个字节
	
	/******************************通道控制初始化*********************************/
	
	/**********************************LED管脚初始化*********************************/
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

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;		//PB15  485接收数据
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;		//PB14 485发送数据
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//PC9  信号红
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		//PC8  信号绿
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		//PB13 交互收
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;		//PB12 交互发
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//test
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//PD2 交互发
	//GPIO_Init(GPIOD, &GPIO_InitStructure);

	
 /**************************检测通道初始化*******************************************/

/**********************************IO口未用管脚初始化**************************************/
	
	
/*************************************************************************************/

 /**********************************LED管脚状态初始化*********************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		//PB12 交互发
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	//初始状态为灭
	for (i = 0; i < LED_ALL; i++)
	{
		status[i].on_off = 0x00;
		status[i].pstate = LED_STATE_OFF;
		status[i].left = 0xff;
		status[i].cont = 0xffff;
	}

	memset(led_buf, 0x00, LED_ALL);
	led_update_display( );

	
	rdy = 0x01;//初始化完
}

/*********************************************************************************************************
* 名    称：ledctl_task
* 功    能：系统灯控制任务
* 入口参数: pdata:任务私有数据
							
* 出口参数:
**********************************************************************************************************/
void led_task( void *pdata)
{
	led_cmd_type *cmd;
	OS_Q_DATA q_data;
	INT16U num=0;
	INT8U err;
	uint8 i;
	uint8 change = 0x00;
	
	LED_dispinit( );//led任务初始化，创建任务消息队列及LED灯控制初始化
	
	for(;;)
	{
		OSQQuery(led_q,&q_data); 
		if ( (num = q_data.OSNMsgs) != 0) 
		{
			while(num != 0)
			{
				cmd = (led_cmd_type *)OSQPend(led_q, 0, &err);//接收消息队列
				switch(cmd->cmd)
				{
					case LED_CMD_ON://亮灯 
						if (status[cmd->index].pstate != LED_STATE_ONR)
						{
							status[cmd->index].pstate = LED_STATE_ONR;
							status[cmd->index].on_off = 0x01;
							status[cmd->index].left = 0xff;
							status[cmd->index].cont = 0xffff;//持续多长时间
							
							led_buf[cmd->index] = 0x01;
							change = 0x01;
						}
						break;
						
					case LED_CMD_OFF://灭灯
						if (status[cmd->index].pstate != LED_STATE_OFF )
						{
							status[cmd->index].pstate = LED_STATE_OFF;
							status[cmd->index].on_off = 0x00;
							status[cmd->index].left = 0xff;
							status[cmd->index].cont = 0xffff;//持续多长时间
							
							led_buf[cmd->index] = 0x00;
							change = 0x01;
						}
						break;

					case LED_CMD_FLASH://灯频闪
						if (status[cmd->index].pstate != LED_STATE_FHR)
						{
							status[cmd->index].pstate = LED_STATE_FHR;
							status[cmd->index].cont = 50;//持续多长时间
							status[cmd->index].on_off = 0x00;
							
							led_buf[cmd->index] = 0x00;
							
							//频率
							status[cmd->index].left = 10;	
							
							change = 0x01;
						}
						break;
						
					default:
						break;
				}
				LED_free_cmd_buf(cmd);//释放内存块
				num--;
			}
		}

		
		
		for(i = 0; i < LED_ALL; i++)//闪烁倒计时
		{
			//最后一个持续时间tick
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
				//有时间限制的
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
						//轮到亮
						if (status[i].on_off == 0x00)
						{
							status[i].on_off = 0x01;
							led_buf[i] = 0x01;
						}
						else
						{
							//轮到灭
							status[i].on_off = 0x00;
							led_buf[i] = 0x00;
						}	
						
             			 //频率
						 status[i].left = 10;					
						 change = 0x01;
					 }
				}
			}
		}

		if(change == 0x01)//更新LED状态
		{
			led_update_display( );
			change = 0x00;
		}
		OSTimeDly(10);
	}
}

