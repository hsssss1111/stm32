/****************************************Copyright (c)****************************************************
**                         							ZhangZhou KENENG electronics Co.,LTD.
**
**
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			app_smem.h
** Last modified Date:  2009-06-18
** Last Version:		1.0
** Descriptions:		采用共享内存的方式实现多任务之间的通信
**
**--------------------------------------------------------------------------------------------------------
** Created by:			heshusheng
** Created date:		2009-06-18
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

#ifndef	_APP_SMEM_H_
#define	_APP_SMEM_H_

#define	SMEM_MAX_NUM		0x10

//控制信息
typedef	struct
{
	uint16 wpoint;		//写入指针
	uint16 rpoint;		//读取指针
	uint16 size;		//数据内容长度
	uint16 len;			//数据区长度
}smem_ctl_type;


//共享内存管理信息
typedef	struct
{
	uint8 exist;	//是否有使用
	uint8 *addr;	//对应的起始地址
}smem_info_type;


/*********************************************************************************************************
** Function name:			smem_init
** Descriptions:			共享内存模块初始化
** input parameters:		param:参数
** Output parameters:       NONE
** Returned value:			NONE
*********************************************************************************************************/
void smem_init( void *);

/*********************************************************************************************************
** Function name:			smem_create
** Descriptions:			创建一共享内存区
** input parameters:		addr:共享内存的地址
**									size:共享内存大小
** Output parameters:       NONE
** Returned value:			-1:失败；other:数据长度
*********************************************************************************************************/
int8 smem_create(uint8 *addr, uint16 size);

/*********************************************************************************************************
** Function name:			smem_get_len
** Descriptions:			获取共享内存区可用的数据长度
** input parameters:		smem:对应的共享内存区
** Output parameters:       NONE
** Returned value:			-1:失败；other:数据长度
*********************************************************************************************************/
uint16 smem_get_len( int8 smem);

/*********************************************************************************************************
** Function name:			smem_read
** Descriptions:			从共享内存区中读取数据
** input parameters:		smem:对应的共享内存区
**									pbuf:数据缓存
**									len:读取的数据长度
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
uint16 smem_read(int8 smem, uint8 *pbuf, uint16 len);

/*********************************************************************************************************
** Function name:			smem_write
** Descriptions:			向共享内存区中写入数据
** input parameters:		smem:对应的共享内存区
**									pbuf:数据缓存
**									len:写入的数据长度
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
uint16 smem_write(int8 smem, uint8 *pbuf, uint16 len);

/*********************************************************************************************************
** Function name:			smem_set_read_point
** Descriptions:			设置共享内存读取位置
** input parameters:		smem:对应的共享内存区
**									pos:设置的读位置
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
int8 smem_set_read_point(int8 smem, uint16 pos);

/*********************************************************************************************************
** Function name:			smem_set_write_point
** Descriptions:			设置共享内存写入位置
** input parameters:		smem:对应的共享内存区
**									pos:设置的写位置
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
int8 smem_set_write_point(int8 smem, uint16 pos);

/*********************************************************************************************************
** Function name:			smem_clear
** Descriptions:			清空共享内存区中的数据
** input parameters:		smem:对应的共享内存区
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
int8 smem_clear(int8 smem);

/*********************************************************************************************************
** Function name:			smem_clear_pos
** Descriptions:			清空某个位置之前的所有数据
** input parameters:		smem:对应的共享内存区
									pos:清空对应位置前的数据
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
int8 smem_clear_pos(int8 smem, uint16 pos);

/*********************************************************************************************************
** Function name:			smem_clear_posback
** Descriptions:			清空某个位置之后的所有数据
** input parameters:		smem:对应的共享内存区
									pos:清空对应位置之后的数据
									len:清空的数据长度
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
int8 smem_clear_posback(int8 smem, uint16 pos, uint16 len);

#endif /*_APP_SMEM_H_*/
