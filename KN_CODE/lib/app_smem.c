/****************************************Copyright (c)****************************************************
**                         							ZhangZhou KENENG electronics Co.,LTD.
**
**
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			app_smem.c
** Last modified Date:  2009-06-18
** Last Version:		1.0
** Descriptions:		���ù����ڴ�ķ�ʽʵ�ֶ�����֮�������ͨ��
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
** Descriptions:			�����ڴ�ģ���ʼ��
** input parameters:		param:����
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
** Descriptions:			����һ�����ڴ���
** input parameters:		addr:�����ڴ�ĵ�ַ
**									size:�����ڴ��С
** Output parameters:       NONE
** Returned value:			-1:ʧ�ܣ�other:���ݳ���
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
** Descriptions:			��ȡ�����ڴ������õ����ݳ���
** input parameters:		smem:��Ӧ�Ĺ����ڴ���
** Output parameters:       NONE
** Returned value:			-1:ʧ�ܣ�other:���ݳ���
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
** Descriptions:			�ӹ����ڴ����ж�ȡ����
** input parameters:		smem:��Ӧ�Ĺ����ڴ���
**									pbuf:���ݻ���
**									len:��ȡ�����ݳ���
** Output parameters:       NONE
** Returned value:			-1:ʧ�ܣ�0:�ɹ�
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
** Descriptions:			�����ڴ�����д������
** input parameters:		smem:��Ӧ�Ĺ����ڴ���
**									pbuf:���ݻ���
**									len:д������ݳ���
** Output parameters:       NONE
** Returned value:			-1:ʧ�ܣ�0:�ɹ�
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
** Descriptions:			���ù����ڴ��ȡλ��
** input parameters:		smem:��Ӧ�Ĺ����ڴ���
**									pos:���õĶ�λ��
** Output parameters:       NONE
** Returned value:			-1:ʧ�ܣ�0:�ɹ�
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
** Descriptions:			���ù����ڴ�д��λ��
** input parameters:		smem:��Ӧ�Ĺ����ڴ���
**									pos:���õ�дλ��
** Output parameters:       NONE
** Returned value:			-1:ʧ�ܣ�0:�ɹ�
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
** Descriptions:			��չ����ڴ����е�����
** input parameters:		smem:��Ӧ�Ĺ����ڴ���
** Output parameters:       NONE
** Returned value:			-1:ʧ�ܣ�0:�ɹ�
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
** Descriptions:			���ĳ��λ��֮ǰ����������
** input parameters:		smem:��Ӧ�Ĺ����ڴ���
									pos:��ն�Ӧλ��ǰ������
** Output parameters:       NONE
** Returned value:			-1:ʧ�ܣ�0:�ɹ�
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
	//��������
	len = ctl_info.size - pos;
	pdata = smem_info[smem].addr + sizeof(ctl_info);
	memcpy(pdata, (pdata + pos), len);

	//�����ڴ����״̬��
	ctl_info.rpoint = sizeof(ctl_info);
	ctl_info.wpoint = sizeof(ctl_info) + len;
	ctl_info.size = len;
	memcpy(smem_info[smem].addr, &ctl_info, sizeof(ctl_info));
	OSSemPost(smem_sem);
	return 0;
}

/*********************************************************************************************************
** Function name:			smem_clear_posback
** Descriptions:			���ĳ��λ��֮�����������
** input parameters:		smem:��Ӧ�Ĺ����ڴ���
									pos:��ն�Ӧλ��֮�������
									clearlen:��յ����ݳ���
** Output parameters:       NONE
** Returned value:			-1:ʧ�ܣ�0:�ɹ�
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

	//��������
	len = ctl_info.size - clearlen - pos;  // �������ݳ���
	pdata = smem_info[smem].addr + sizeof(ctl_info); // ��������ʼ��ַ
	memcpy((pdata + pos), (pdata + pos + clearlen), len);

	//�����ڴ����״̬��
	len = ctl_info.size - clearlen;  // ʣ������
	ctl_info.rpoint = sizeof(ctl_info);
	ctl_info.wpoint = sizeof(ctl_info) + len;
	ctl_info.size = len;
	memcpy(smem_info[smem].addr, &ctl_info, sizeof(ctl_info));
	OSSemPost(smem_sem);
	return 0;
}

