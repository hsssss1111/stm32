/****************************************Copyright (c)****************************************************
**                         							ZhangZhou KENENG electronics Co.,LTD.
**
**
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			app_smem.c
** Last modified Date:  2009-06-18
** Last Version:		1.0
** Descriptions:		采用共享内存的方式实现多任务之间的数据通信
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
#include	"config.h"
#include	"app_smem.h"

static smem_info_type smem_info[SMEM_MAX_NUM];
static OS_EVENT *smem_sem;

/*********************************************************************************************************
** Function name:			smem_init
** Descriptions:			共享内存模块初始化
** input parameters:		param:参数
** Output parameters:       NONE
** Returned value:			NONE
*********************************************************************************************************/
void smem_init( void *param)
{
	param = param;
	smem_sem = OSSemCreate(1);
	memset(smem_info, 0x00, sizeof(smem_info));
}

/*********************************************************************************************************
** Function name:			smem_create
** Descriptions:			创建一共享内存区
** input parameters:		addr:共享内存的地址
**									size:共享内存大小
** Output parameters:       NONE
** Returned value:			-1:失败；other:数据长度
*********************************************************************************************************/
int8 smem_create(uint8 *addr, uint16 size)
{
	INT8U err;
	int8 i = 0;
	smem_ctl_type ctl_info;

	if ( size <= sizeof(smem_ctl_type) )
		return -1;

	OSSemPend(smem_sem, 0, &err);
	for ( i = 0; i < SMEM_MAX_NUM; i++)
	{
		if ( smem_info[i].exist == 0)
		{
			smem_info[i].exist = 1;
			smem_info[i].addr = addr;
			ctl_info.len = size;
			ctl_info.wpoint = sizeof(smem_ctl_type);
			ctl_info.rpoint = sizeof(smem_ctl_type);
			ctl_info.size = 0;
			memcpy(addr, &ctl_info, sizeof(smem_ctl_type));
			goto exit;
		}
	}
	i = -1;
exit:
	OSSemPost(smem_sem);
	return i;
}

/*********************************************************************************************************
** Function name:			smem_get_len
** Descriptions:			获取共享内存区可用的数据长度
** input parameters:		smem:对应的共享内存区
** Output parameters:       NONE
** Returned value:			-1:失败；other:数据长度
*********************************************************************************************************/
uint16 smem_get_len( int8 smem)
{
	INT8U err;
	smem_ctl_type ctl_info;

	OSSemPend(smem_sem, 0, &err);
	if (smem_info[smem].exist == 1)
	{
		memcpy(&ctl_info, smem_info[smem].addr, sizeof(ctl_info));
		OSSemPost(smem_sem);
		return ctl_info.size;
	}
	OSSemPost(smem_sem);
	return 0;
}

/*********************************************************************************************************
** Function name:			smem_read
** Descriptions:			从共享内存区中读取数据
** input parameters:		smem:对应的共享内存区
**									pbuf:数据缓存
**									len:读取的数据长度
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
uint16 smem_read(int8 smem, uint8 *pbuf, uint16 len)
{
	INT8U err;
	smem_ctl_type ctl_info;
	uint8 *addr;

	OSSemPend(smem_sem, 0, &err);

	if ( smem_info[smem].exist != 1)
	{
		OSSemPost(smem_sem);
		return 0;
	}
	memcpy(&ctl_info, smem_info[smem].addr, sizeof(ctl_info));
	if ( ctl_info.size < len )
		len = ctl_info.size;
	addr = smem_info[smem].addr + ctl_info.rpoint;
	memcpy(pbuf, addr, len);
	OSSemPost(smem_sem);
	return len;
}

/*********************************************************************************************************
** Function name:			smem_write
** Descriptions:			向共享内存区中写入数据
** input parameters:		smem:对应的共享内存区
**									pbuf:数据缓存
**									len:写入的数据长度
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
uint16 smem_write(int8 smem, uint8 *pbuf, uint16 len)
{
	INT8U err;
	smem_ctl_type ctl_info;
	uint8 *addr;

	OSSemPend(smem_sem, 0, &err);
	if ( smem_info[smem].exist != 1)
	{
		OSSemPost(smem_sem);
		return 0;
	}
	memcpy(&ctl_info, smem_info[smem].addr, sizeof(ctl_info));
	if ( (ctl_info.size + len) > ctl_info.len )
	{
		OSSemPost(smem_sem);
		return 0;
	}
	addr = smem_info[smem].addr + ctl_info.wpoint;
	memcpy(addr, pbuf, len);
	ctl_info.wpoint += len;
	ctl_info.size += len;
	memcpy(smem_info[smem].addr, &ctl_info, sizeof(ctl_info));

	OSSemPost(smem_sem);
	return len;
}

/*********************************************************************************************************
** Function name:			smem_set_read_point
** Descriptions:			设置共享内存读取位置
** input parameters:		smem:对应的共享内存区
**									pos:设置的读位置
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
int8 smem_set_read_point(int8 smem, uint16 pos)
{
	INT8U err;
	smem_ctl_type ctl_info;

	OSSemPend(smem_sem, 0, &err);

	if ( smem_info[smem].exist != 1)
	{
		OSSemPost(smem_sem);
		return -1;
	}
	memcpy(&ctl_info, smem_info[smem].addr, sizeof(ctl_info));
	if ( ctl_info.len < (pos + sizeof(ctl_info)))
	{
		OSSemPost(smem_sem);
		return -1;
	}
	ctl_info.rpoint = (sizeof(ctl_info) + pos);
	memcpy(smem_info[smem].addr, &ctl_info, sizeof(ctl_info));
	OSSemPost(smem_sem);
	return 0;
}

/*********************************************************************************************************
** Function name:			smem_set_write_point
** Descriptions:			设置共享内存写入位置
** input parameters:		smem:对应的共享内存区
**									pos:设置的写位置
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
int8 smem_set_write_point(int8 smem, uint16 pos)
{
	INT8U err;
	smem_ctl_type ctl_info;

	OSSemPend(smem_sem, 0, &err);

	if ( smem_info[smem].exist != 1)
	{
		OSSemPost(smem_sem);
		return -1;
	}
	memcpy(&ctl_info, smem_info[smem].addr, sizeof(ctl_info));
	if ( ctl_info.len < (pos + sizeof(ctl_info)))
	{
		OSSemPost(smem_sem);
		return -1;
	}
	ctl_info.wpoint = (sizeof(ctl_info) + pos);
	memcpy(smem_info[smem].addr, &ctl_info, sizeof(ctl_info));
	OSSemPost(smem_sem);
	return 0;
}

/*********************************************************************************************************
** Function name:			smem_clear
** Descriptions:			清空共享内存区中的数据
** input parameters:		smem:对应的共享内存区
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
int8 smem_clear(int8 smem)
{
	INT8U err;
	smem_ctl_type ctl_info;

	OSSemPend(smem_sem, 0, &err);
	if ( smem_info[smem].exist != 1)
	{
		OSSemPost(smem_sem);
		return -1;
	}
	memcpy(&ctl_info, smem_info[smem].addr, sizeof(ctl_info));
	ctl_info.size = 0;
	ctl_info.wpoint = sizeof(ctl_info);
	ctl_info.rpoint = sizeof(ctl_info);
	memcpy(smem_info[smem].addr, &ctl_info, sizeof(ctl_info));
	OSSemPost(smem_sem);
	return 0;
}

/*********************************************************************************************************
** Function name:			smem_clear_pos
** Descriptions:			清空某个位置之前的所有数据
** input parameters:		smem:对应的共享内存区
									pos:清空对应位置前的数据
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
int8 smem_clear_pos(int8 smem, uint16 pos)
{
	uint8 *pdata;
	uint16 len;
	INT8U err;
	smem_ctl_type ctl_info;

	OSSemPend(smem_sem, 0, &err);

	if ( smem_info[smem].exist != 1)
	{
		OSSemPost(smem_sem);
		return -1;
	}
	memcpy(&ctl_info, smem_info[smem].addr, sizeof(ctl_info));
	if ( ctl_info.len < (pos + sizeof(ctl_info)))
	{
		OSSemPost(smem_sem);
		return -1;
	}
	//拷贝数据
	len = ctl_info.size - pos;
	pdata = smem_info[smem].addr + sizeof(ctl_info);
	memcpy(pdata, (pdata + pos), len);

	//更新内存控制状态字
	ctl_info.rpoint = sizeof(ctl_info);
	ctl_info.wpoint = sizeof(ctl_info) + len;
	ctl_info.size = len;
	memcpy(smem_info[smem].addr, &ctl_info, sizeof(ctl_info));
	OSSemPost(smem_sem);
	return 0;
}

/*********************************************************************************************************
** Function name:			smem_clear_posback
** Descriptions:			清空某个位置之后的所有数据
** input parameters:		smem:对应的共享内存区
									pos:清空对应位置之后的数据
									clearlen:清空的数据长度
** Output parameters:       NONE
** Returned value:			-1:失败；0:成功
*********************************************************************************************************/
int8 smem_clear_posback(int8 smem, uint16 pos, uint16 clearlen)
{
	uint8 *pdata;
	uint16 len;
	INT8U err;
	smem_ctl_type ctl_info;

	OSSemPend(smem_sem, 0, &err);

	if ( smem_info[smem].exist != 1)
	{
		OSSemPost(smem_sem);
		return -1;
	}
	memcpy(&ctl_info, smem_info[smem].addr, sizeof(ctl_info));
	if ( ctl_info.len < (pos + clearlen + sizeof(ctl_info)))
	{
		OSSemPost(smem_sem);
		return -1;
	}

	//拷贝数据
	len = ctl_info.size - clearlen - pos;  // 拷贝数据长度
	pdata = smem_info[smem].addr + sizeof(ctl_info); // 数据区起始地址
	memcpy((pdata + pos), (pdata + pos + clearlen), len);

	//更新内存控制状态字
	len = ctl_info.size - clearlen;  // 剩余数据
	ctl_info.rpoint = sizeof(ctl_info);
	ctl_info.wpoint = sizeof(ctl_info) + len;
	ctl_info.size = len;
	memcpy(smem_info[smem].addr, &ctl_info, sizeof(ctl_info));
	OSSemPost(smem_sem);
	return 0;
}

