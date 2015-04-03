/************************************************************
 * Copyright (c) 2013,漳州科能电器有限公司
 * All rights reserved. 
 *
 * 文件名称：led.h
 * 文件标识：见配置管理计划书 
 * 描述：led灯显示驱动
 *
 * 当前版本： 1.0
 * 创建者：   ronfull
 * 创建日期： 2013-01-05
 *
 * 当前版本： 1.1
 * 修改人： ronfull
 * 修改日期：2013-03-26
 * 修改原因： 增加GPRS模块命令
 *
 * 当前版本： 1.2
 * 修改人： ronfull
 * 修改日期：2013-04-28
 * 修改原因： 屏蔽掉电检测命令 	//PWDCKE,//p2.12 GPIO1[12] 掉电检测
 *
 * 当前版本： 1.3
 * 修改人： ronfull
 * 修改日期：2013-05-28
 * 修改原因： 增加模块状态识别判断函数
 **********************************************************/
#ifndef	LED_H_
#define	LED_H_
#include "config.h"

enum LED_NUM   //待控制的LED和FMQ
{
	LED_WARN,   //PC2  告警
	LED_ONLINE, //PC3  在线
	LED_GPRS_R, //PC7  GPRS接收数据
	LED_GPRS_S, //PC6 GPRS发送数据
	LED_485_R, //PB15 485接收数据
	LED_485_T, //PB14 485发送数据
	LED_SIG_R, //PC9  信号红
	LED_SIG_G, //PC8  信号绿
	LED_JH_R,  //PB13 交互收
	LED_JH_S,  //PB12 交互发
	
	LAST_LED_NUM,//需在它之前增加通道
};

#define LED_ALL         LAST_LED_NUM

enum CHECK_NUM //待控制的遥信通道
{
	YX_PROG,    //PC 13   编程键
	
	LAST_CHECK_NUM, //需在它之前增加通道
};

#define CHECK_ALL       LAST_CHECK_NUM

enum TD_NUM  //通道控制命令
{
	JL_CTL_RST,         //p2.13 GPIO1[13] 计量芯片复位
 	NET_CTL_RST,        //p4.4 GPIO2[4]以太网芯片复位
  	MK_CTL_VDDON,       //p6.7 GPIO5[15]模块电源开
  	MK_CTL_VDDOFF,      //p6.7 GPIO5[15]模块电源关
	MK_CTL_JRON,        //p6.8 GPIO5[16]   模块加热管开 
  	MK_CTL_JROFF,       //p6.8 GPIO5[16]   模块加热管关
	MK_RST_HI,          //p2.6 GPIO5[6]  GPRS模块复位脚高
	MK_RST_LO,			//p2.6 GPIO5[6]  GPRS模块复位脚低
	MK_ONOFF_HI,	    //p2.7 GPIO0[7]  GPRS模块工作开
	MK_ONOFF_LO,		//p2.7 GPIO0[7]  GPRS模块工作开
};

//任务队列空间参数
#define	LED_BLK_LEN		0x10 
#define	LED_QUEUE_LEN	0x8
#define	LED_POOL_LEN	0x80


//任务命令类型
#define	LED_CMD_ON		  0x00		//亮灯
#define	LED_CMD_OFF		  0x01		//灭灯
#define	LED_CMD_FLASH	  0x02	    //灯频闪

#define	LED_CMD_RDST	  0x03		//获取状态


//当前状态控制 
#define	LED_STATE_ONR	0x00		//亮灯
#define	LED_STATE_FHR	0x01		//频闪灯
#define	LED_STATE_OFF	0x02		//灭灯

//状态控制字
typedef struct
{
	uint8 on_off;	//0=灭  1=亮
	uint8 pstate;	//设置的状态
	uint8 left;		//剩余的停留数  闪烁才有意义
	uint16 cont;	//剩余的持续时间数
}led_status_type;

//命令参数结构
typedef	struct
{
	uint8 cmd;		//命令类型
	uint8 index;	//控制的LED
}led_cmd_type;

/*********************************************************************************************************
* 名    称：LED_output_state
* 功    能：控制LED或者蜂鸣器  (由外部模块调用)
* 入口参数: cmd:亮/灭/频闪
				idx:对应的LED灯id.
				last:持续时间  如果为0xffff,则固定
				prv:私有参数(用于扩展)
							
* 出口参数:	目前只返回0
**********************************************************************************************************/
int8  LED_output_state(uint8 cmd, uint8 idx);

/*********************************************************************************************************
* 名    称：LED_get_chnstate
* 功    能：获取当前通道的状态
* 入口参数: idx:对应的索引
				pout:状态输出
							
* 出口参数:	-1失败  0-成功
**********************************************************************************************************/
int8 LED_get_chnstate(uint8 idx, uint8 *pout);

/*********************************************************************************************************
* 名    称：LED_get_init_status
* 功    能：获取init初始化状态
* 入口参数: 无
							
* 出口参数:	0-未初始化完，1初始化
**********************************************************************************************************/
int8 LED_get_init_status( void );

/*******************************************************
 * 函数功能:控制arm外设
 * 函数名称:led_ctl_prestatus
 * 输入参数:LED_CTL_RSTJL/LED_CTL_RSTNET 等；
 * 输出参数:
 * 返   回  值:
 *******************************************************/
void led_ctl_prestatus(uint8 type);
/*********************************************************************************************************
* 名    称：ledctl_task
* 功    能：系统灯控制任务
* 入口参数: pdata:任务私有数据
							
* 出口参数:
**********************************************************************************************************/
void led_task(void *pdata );

/*********************************************************************************************************
* 名    称：read_mk_state
* 功    能：获取模块当前状态 M_STATE2 | M_STATE1 | M_STATE0
* 入口参数: 无						
* 出口参数:	 0x07-无模块，0x06-有模块，响应标准AT指令，0x04-有模块，不响应AT指令,其他值temp-预留
**********************************************************************************************************/
uint32 read_mk_state(void);

#endif /*LED_H_*/










