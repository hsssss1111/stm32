/************************************************************
 * Copyright (c) 2009,���ݿ��ܵ������޹�˾
 * All rights reserved. 
 *
 * �ļ����ƣ�DataFormat.c
 * �ļ���ʶ�������ù���ƻ��� 
 * ������ ���ݸ�ʽ������ģ��
 *
 * ��ǰ�汾�� 1.0
 * �����ߣ��ֶ���
 * �������ڣ� 2010-4-9
 *
 * �޸��ˣ�
 * �޸����ڣ�
 * �޸�ԭ��
 **********************************************************/

//*====================================================
//*������������������ͷ���ġ���
//*====================================================
#include <math.h>
#include "config.h"
#include "datatype.h"
#include "app_util.h"
#include ".\kn_code\driver\rtc\rtc.h"
#include ".\kn_code\lib\debug.h"

//*====================================================
//*��������������������������������
//*====================================================


//*====================================================
//*������������������ ����������
//*====================================================

/*******************************************************
 * ��������:10��n��������
 * ��������:DT_MyPow
 * �������:<s16X1>[int16] ���������
 * ��   ��  ֵ:double���͵���������
 *******************************************************/
static fp64 DT_MyPow(int16 s16X1)
{
	if (s16X1 >= 0)
	{
		return pow((double)10, s16X1);
	}

	if (s16X1 < 0)
	{
		return (1 / pow((double)10, abs(s16X1)));
	}

	return 0.0;
}

/*******************************************************
 * ��������:�ַ���תΪDouble��������
 * ��������:DT_DataTypeToDouble
 * �������:<s16Begin>[int16] ��ת���ַ���ָ����ʼֵ
 * �������:<u16Num>[uint16] ��ת���ַ�������λ����
 * �������:<pBuf>[uint8 *] ��ת���ַ���
 * ��   ��  ֵ:ת�����double��������
 *******************************************************/
static fp64 DT_BaseTypeToDouble(int16 s16Begin, uint16 u16Num, const uint8 * pBuf)
{
	fp64 _fpValue = 0;
	uint16 i = 0, j = 0;
	int16 _s16Zs = 0;  // ָ��
	uint8 _u8Tmp = 0;   // ��ʱ����

	j = 0;
	_fpValue = 0;

	for (i = 0, _s16Zs = s16Begin; i < u16Num; i++, _s16Zs++)
	{
		if (i % 2 == 0)
		{
			_u8Tmp = pBuf[j] & 0x0F;
		}
		else
		{
			_u8Tmp = (pBuf[j] >> 4) & 0x0F;
			j++;
		}

		_fpValue += _u8Tmp * DT_MyPow( _s16Zs);
	}

	return _fpValue;
}

/*******************************************************
 * ��������:���ݸ�ʽA02תΪDouble����
 * ��������:DT_DataType02ToDouble
 * �������:<pData02>[uint8 *] ��ת��A02�ַ�������
 * �������:<fpValue>[fp64 *] ת����double��������
 * ��   ��  ֵ:0
 *******************************************************/
static int8 DT_DataType02ToDouble(const uint8 *pData02, fp64 * fpValue)
{
	fp64 _fpTmp = 0;
	int8 _s8Flag = 1;
	uint8 _u8Zs = 0;

	_fpTmp = DT_BaseTypeToDouble(0, 3, pData02);

	if (((pData02[1] >> 4) & 0x01) == 0) // ����λ
	{
		_s8Flag = 1;
	}
	else
	{
		_s8Flag = -1;
	}

	_u8Zs = (pData02[1] >> 5) & 0x07;

	*fpValue = _s8Flag * _fpTmp * pow((double)10, 4 - _u8Zs);

	return 0;
}

/*******************************************************
 * ��������:���ݸ�ʽA03תΪDouble����
 * ��������:DT_DataType03ToDouble
 * �������:<pData03>[uint8 *] ��ת��A03�ַ�������
 * �������:<fpValue>[fp64 *] ת����double�������ݵ�λ(kwh/��)
 * ��   ��  ֵ:0
 *******************************************************/
static int8 DT_DataType03ToDouble(const uint8 *pData03, fp64 * fpValue)
{
	fp64 _fpTmp = 0;
	int8 _s8Flag = 1;
	uint16 _u16Rate = 0;

	_fpTmp = DT_BaseTypeToDouble(0, 7, pData03);

	if (((pData03[3] >> 4) & 0x01) == 0) // ����λ
	{
		_s8Flag = 1;
	}
	else
	{
		_s8Flag = -1;
	}

	if (((pData03[3] >> 6) & 0x01) == 0) // ��λ
	{
		_u16Rate = 1;
	}
	else
	{
		_u16Rate = 1000;
	}

	*fpValue = _s8Flag * _fpTmp * _u16Rate;

	return 0;
}

/*******************************************************
 * ��������:���ݸ�ʽA04תΪDouble����
 * ��������:DT_DataType04ToDouble
 * �������:<pData04>[uint8 *] ��ת��A04�ַ�������
 * �������:<fpValue>[fp64 *] ת����double��������
 * ��   ��  ֵ:0
 *******************************************************/
static int8 DT_DataType04ToDouble(const uint8 *pData04, fp64 * fpValue)
{
	fp64 _fpTmp = 0;

	_fpTmp = DT_BaseTypeToDouble(0, 1, pData04); // ��λ
	_fpTmp += ((pData04[0] >> 4) & 0x07) * 10; // ʮλ

	if (((pData04[0] >> 7) & 0x01) == 0)
	{
		_fpTmp = _fpTmp / 100 + 1;
	}
	else
	{
		_fpTmp = 1 - _fpTmp / 100;
	}

	*fpValue = _fpTmp;

	return 0;
}

/*******************************************************
 * ��������:���ݸ�ʽA05תΪDouble����
 * ��������:DT_DataType05ToDouble
 * �������:<pData05>[uint8 *] ��ת��A05�ַ�������
 * �������:<fpValue>[fp64 *] ת����double��������
 * ��   ��  ֵ:0
 *******************************************************/
static int8 DT_DataType05ToDouble(const uint8 *pData05, fp64 * fpValue)
{
	fp64 _fpTmp = 0;

	_fpTmp = DT_BaseTypeToDouble(-1, 3, pData05); // ��λ
	_fpTmp += ((pData05[1] >> 4) & 0x07) * 100; // ��λ

	if (((pData05[1] >> 7) & 0x01) != 0)
	{
		_fpTmp = -1 * _fpTmp;
	}

	*fpValue = _fpTmp;

	return 0;
}

/*******************************************************
 * ��������:���ݸ�ʽA05תΪDouble����
 * ��������:DT_DataType06ToDouble
 * �������:<pData05>[uint8 *] ��ת��A05�ַ�������
 * �������:<fpValue>[fp64 *] ת����double��������
 * ��   ��  ֵ:0
 *******************************************************/
static int8 DT_DataType06ToDouble(const uint8 *pData06, fp64 * fpValue)
{
	fp64 _fpTmp = 0;

	_fpTmp = DT_BaseTypeToDouble(-2, 3, pData06); // ��λ
	_fpTmp += ((pData06[1] >> 4) & 0x07) * 10; // ʮλ

	if (((pData06[1] >> 7) & 0x01) != 0)
	{
		_fpTmp = -1 * _fpTmp;
	}

	*fpValue = _fpTmp;

	return 0;
}

/*******************************************************
 * ��������:���ݸ�ʽA09תΪDouble����
 * ��������:DT_DataType09ToDouble
 * �������:<pData09>[uint8 *] ��ת��A05�ַ�������
 * �������:<fpValue>[fp64 *] ת����double��������
 * ��   ��  ֵ:0
 *******************************************************/
static int8 DT_DataType09ToDouble(const uint8 *pData09, fp64 * fpValue)
{
	fp64 _fpTmp = 0;

	_fpTmp = DT_BaseTypeToDouble(-4, 5, pData09);
	_fpTmp += ((pData09[2] >> 4) & 0x07) * 10;

	if (((pData09[2] >> 7) & 0x01) != 0)
	{
		_fpTmp = -1 * _fpTmp;
	}

	*fpValue = _fpTmp;

	return 0;
}

/*******************************************************
 * ��������:���ݸ�ʽA25תΪDouble����
 * ��������:DT_DataType25ToDouble
 * �������:<pData25>[uint8 *] ��ת��A25�ַ�������
 * �������:<fpValue>[fp64 *] ת����double��������
 * ��   ��  ֵ:0
 *******************************************************/
static int8 DT_DataType25ToDouble(const uint8 *pData25, fp64 * fpValue)
{
	fp64 _fpTmp = 0;

	_fpTmp = DT_BaseTypeToDouble(-3, 5, pData25); //
	_fpTmp += ((pData25[2] >> 4) & 0x07) * 100; // ��λ

	if (((pData25[2] >> 7) & 0x01) != 0)
	{
		_fpTmp = -1 * _fpTmp;
	}

	*fpValue = _fpTmp;

	return 0;
}

/*******************************************************
 * ��������:���ַ�������תΪdouble����
 * ��������:DT_DataTypeToDouble
 * �������:<u8Type>[uint8] ��ת���ַ�����ʽ
 * �������:<pData>[uint8 *] ��ת���ַ���
 * �������:<fpValue>[fp64 *] ת����double��������
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
int8 DT_DataTypeToDouble(uint8 u8Type, const uint8 *pData, fp64 * fpValue)
{
	int8 _u8Rst = 0;

	switch(u8Type)
	{
		case 2:
		{
			_u8Rst = DT_DataType02ToDouble(pData, fpValue);
			break;
		}
		case 3:
		{
			_u8Rst =  DT_DataType03ToDouble(pData, fpValue);
			break;
		}
		case 4:
		{
			_u8Rst =  DT_DataType04ToDouble(pData, fpValue);
			break;
		}
		case 5:
		{
			_u8Rst =  DT_DataType05ToDouble(pData, fpValue);
			break;
		}
		case 6:
		{
			_u8Rst =  DT_DataType06ToDouble(pData, fpValue);
			break;
		}
		case 7:
		{
			*fpValue = DT_BaseTypeToDouble(-1, 4, pData);
			break;
		}
		case 8:
		{
			*fpValue = DT_BaseTypeToDouble(0, 4, pData);
			break;
		}
		case 9:
		{
			_u8Rst = DT_DataType09ToDouble(pData, fpValue);
			break;
		}
		case 10:
		{
			*fpValue = DT_BaseTypeToDouble(0, 6, pData);
			break;
		}
		case 11:
		{
			*fpValue = DT_BaseTypeToDouble(-2, 8, pData);
			break;
		}
		case 12:
		{
			*fpValue = DT_BaseTypeToDouble(0, 12, pData);
			break;
		}
		case 13:
		{
			*fpValue = DT_BaseTypeToDouble(-4, 8, pData);
			break;
		}
		case 14:
		{
			*fpValue = DT_BaseTypeToDouble(-4, 10, pData);
			break;
		}
		case 22:
		{
			*fpValue = DT_BaseTypeToDouble(-1, 2, pData);
			break;
		}
		case 23:
		{
			*fpValue = DT_BaseTypeToDouble(-4, 6, pData);
			break;
		}
		case 25:
		{
			_u8Rst =  DT_DataType25ToDouble(pData, fpValue);
			break;
		}
		case 26:
		{
			*fpValue = DT_BaseTypeToDouble(-3, 4, pData);
			break;
		}
		default:
		{
			_u8Rst = -1;
			break;
		}
	}

	return _u8Rst;
}

/*******************************************************
 * ��������:��double��������תΪָ����ʽ���ַ���
 * ��������:DT_BaseDoubleToDataType
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<u8IntegerBit>[uint8] ��ת����������λ
 * �������:<u8IntegerBit>[uint8] ��ת������С��λ
 * �������:<u8IntegerBit>[uint8] ��ת�������ֽ���
 * �������:<pOutBuf>[uint8 *] ת�����ַ���
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_BaseDoubleToDataType(fp64 fpValue, uint8 u8IntegerBit, uint8 u8DecimalBit, uint8 u8ByteNum, uint8 *pOutBuf)
{
//	uint8 _Tmp01[100]; // ��ʱ����
//	uint8 _Tmp02[100]; // ��ʱ����
//	uint8 _u8Char = 0; // ��ǰ�ַ�
//	uint8 _u8Pos = 0; // ��ǰλ��
//	uint16 _u16lb = 0; // ѭ��������
//	uint32 _u32TotalLen = 0; // �ܳ���
//	uint32 _u32Val1 = 0, _u32Val2 = 0;
//
//	if (u8IntegerBit == 0)
//	{
//		return -1;
//	}
//
//	memset(pOutBuf, 0x0, u8ByteNum); // ������ַ���������0
//	_u32Val1 = DT_MyPow(u8DecimalBit);
//	_u32Val2 = ((uint32)(fpValue)) * _u32Val1;  // ��������
//	_u32Val1 = fpValue * _u32Val1 - _u32Val2;   // С������
//	sprintf((char *) _Tmp02, "%d", _u32Val1);
//	_u32TotalLen = strlen((char *) _Tmp02);
//
//	if (_u32TotalLen < u8DecimalBit)
//	{
//		sprintf((char *) _Tmp01, "%d.%-s%0*d", (uint32)(fpValue), _Tmp02, u8DecimalBit - _u32TotalLen, 0);
//	}
//	else
//	{
//		sprintf((char *) _Tmp01, "%d.%d", (uint32)(fpValue),  _u32Val1);
//	}
//
//	_u32TotalLen = strlen((char *) _Tmp01);
//
//	for (_u16lb = 0; (_u16lb < (u8IntegerBit + u8DecimalBit)) && (_u32TotalLen > 0); _u16lb++)
//	{
//		_u8Char = _Tmp01[_u32TotalLen - 1];
//
//		if (_u8Char == '.')
//		{
//			_u32TotalLen--;
//			_u8Char = _Tmp01[_u32TotalLen - 1];
//		}
//
//		if ((_u16lb % 2) == 0)
//		{
//			pOutBuf[_u8Pos] &= 0xF0;
//			pOutBuf[_u8Pos] |= (_u8Char & 0x0F);
//		}
//		else
//		{
//			pOutBuf[_u8Pos] &= 0x0F;
//			pOutBuf[_u8Pos] |= ((_u8Char << 4) & 0xF0);
//			_u8Pos++;
//		}
//
//		_u32TotalLen--;
//	}
//
//	return 0;

	uint8 _Tmp01[100]; // ��ʱ����
	uint8 _Tmp02[100]; // ��ʱ����
	uint8 _u8Char = 0; // ��ǰ�ַ�
	uint8 _u8Pos = 0; // ��ǰλ��
	uint16 _u16lb = 0; // ѭ��������
	uint32 _u32TotalLen = 0; // �ܳ���

	if (u8IntegerBit == 0)
	{
		return -1;
	}

	memset(pOutBuf, 0x0, u8ByteNum); // ������ַ���������0
	sprintf((char *) _Tmp02, "%%%d.%df", u8IntegerBit, u8DecimalBit);
	sprintf((char *) _Tmp01, (char *) _Tmp02, fpValue);
	_u32TotalLen = strlen((char *) _Tmp01);

	for (_u16lb = 0; (_u16lb < (u8IntegerBit + u8DecimalBit)) && (_u32TotalLen > 0); _u16lb++)
	{
		_u8Char = _Tmp01[_u32TotalLen - 1];

		if (_u8Char == '.')
		{
			_u32TotalLen--;
			_u8Char = _Tmp01[_u32TotalLen - 1];
		}

		if ((_u16lb % 2) == 0)
		{
			pOutBuf[_u8Pos] &= 0xF0;
			pOutBuf[_u8Pos] |= (_u8Char & 0x0F);
		}
		else
		{
			pOutBuf[_u8Pos] &= 0x0F;
			pOutBuf[_u8Pos] |= ((_u8Char << 4) & 0xF0);
			_u8Pos++;
		}

		_u32TotalLen--;
	}

	return 0;
}
//
/*******************************************************
 * ��������:��double��������תΪA2���ݸ�ʽ
 * ��������:DT_DoubleToDataType02
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A2���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType02(fp64 fpValue, uint8 * pOutData)
{
	uint8 _u8Sign = 0; // ���ű�־
	uint32 _u32TmpVal = 0; // ��ʱ����
	fp64 _fpFabsResult = 0; // ת�����ݵľ���ֵ
	div_t _divRst; // ��������������

	// �����ֵ
	if (fpValue < 0)
	{
		_u8Sign = 1;
		_fpFabsResult = -1 * fpValue;
	}
	else
	{
		_u8Sign = 0;
		_fpFabsResult = fpValue;
	}

	// ��ֵԽ��(10^4 * 999)
	if (_fpFabsResult >= 10000000.00)
	{
		return -1;
	}

	if (_fpFabsResult < 1.00)
	{
		if ((_fpFabsResult * 1000) > 999.50)
		{
			_u32TmpVal = (uint32) (_fpFabsResult * 1000); //�Ŵ�1000��,��������������
		}
		else
		{
			_u32TmpVal = (uint32) (_fpFabsResult * 1000 + 0.5); //�Ŵ�1000��
		}

		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0xE0 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(-3)
	}
	else if (_fpFabsResult < 9.95) //����ǧ��֮��
	{
		_u32TmpVal = (uint32) (_fpFabsResult * 100 + 0.5); //�Ŵ�100��
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0xC0 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(-2)
	}
	else if (_fpFabsResult < 99.95) //�������֮��
	{
		_u32TmpVal = (uint32) (_fpFabsResult * 10 + 0.5); //�Ŵ�10��
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0xA0 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(-1)
	}
	else if (_fpFabsResult < 999.50) //�������֮��
	{
		_u32TmpVal = (uint32) (_fpFabsResult + 0.5); //��������,��ת��Ϊ����
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0x80 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(0)
	}
	else if (_fpFabsResult < 9995.00) //�������֮��
	{
		_u32TmpVal = (uint32) (_fpFabsResult / 10 + 0.5); //��������,��ת��Ϊ����
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0x60 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(1)
	}
	else if (_fpFabsResult < 99950.00) //�������֮��
	{
		_u32TmpVal = (uint32) (_fpFabsResult / 100 + 0.5); //��������,��ת��Ϊ����
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0x40 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(2)
	}
	else if (_fpFabsResult < 999500.00) //�������֮��
	{
		_u32TmpVal = (uint32) (_fpFabsResult / 1000 + 0.5); //��������,��ת��Ϊ����
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0x20 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(3)
	}
	else
	{
		if (_fpFabsResult < 9995000.00) //�������֮��
		{
			_u32TmpVal = (uint32) (_fpFabsResult / 10000 + 0.5); //��������,��ת��Ϊ����
		}
		else //����ǧ��֮һ
		{
			_u32TmpVal = (uint32) (_fpFabsResult / 10000); //������������,�����ܱ�����,��ת��Ϊ����
		}

		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0x00 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); //10^(4)
	}

	//�ı����λ
	if (_u8Sign == 1)
	{
		BitSet(pOutData[1], 4); // ����
	}

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA3���ݸ�ʽ
 * ��������:DT_DoubleToDataType03
 * �������:<fpValue>[uint8] ��ת��double��������(kwh/��)
 * �������:<pOutBuf>[uint8 *] ת����A3���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType03(fp64 fpValue, uint8 * pOutData)
{
	uint8 _u8Sign = 0; // ���ű�־
	uint8 _u8Uint = 0; // ��λ(MWh/Kwh)(Ԫ/��)
	uint8 _u8lb = 0; // ѭ��������
	uint32 _u32TmpVal = 0; // ��ʱ����
	fp64 _fpFabsResult = 0; // ת�����ݵľ���ֵ
	div_t _divRst; // ��������������

	// �����ֵ
	if (fpValue < 0)
	{
		_u8Sign = 1;
		_fpFabsResult = -1 * fpValue;
	}
	else
	{
		_u8Sign = 0;
		_fpFabsResult = fpValue;
	}

	// ��ֵԽ��
	if (_fpFabsResult >= 10000000000.00)
	{
		return -1;
	}

	// ȷ��G�ķ���
	if (_fpFabsResult >= 10000000.00)
	{
		_u8Uint = 1; //MWh/Ԫ
		_fpFabsResult = _fpFabsResult / 1000;

	}

	if ((_fpFabsResult + 0.5) > 10000000.00)
	{
		_u32TmpVal = (uint32) _fpFabsResult; //��������������,��ת��Ϊ����
	}
	else
	{
		_u32TmpVal = (uint32) (_fpFabsResult + 0.5); //������������,��ת��Ϊ����
	}

	for (_u8lb = 0; _u8lb < 4; _u8lb++)
	{
		_divRst = div(_u32TmpVal, 100);
		pOutData[_u8lb] = Bin2BCD(_divRst.rem & 0xFF);
		_u32TmpVal = _divRst.quot;
	}

	if (_u8Sign == 1)
	{
		BitSet(pOutData[3], 4); //����
	}

	if (_u8Uint == 1)
	{
		BitSet(pOutData[3], 6); //MWh/Ԫ
	}

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA4���ݸ�ʽ
 * ��������:DT_DoubleToDataType04
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A4���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType04(fp64 fpValue, uint8 * pOutBuf)
{
	fp64 _fpValue = 0;
	uint8 _u8Sign = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else if (fpValue == 0)
	{
		*pOutBuf = 0x00;
		return 0;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 2)
	{
		return -1;
	}

	if (_fpValue < 1)
	{
		_u8Sign = 1;
		_fpValue = (1 - _fpValue) * 100;
	}
	else
	{
		_u8Sign = 0;
		_fpValue = (_fpValue - 1) * 100;
	}

	if (_fpValue >= 80)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 2, 0, 1, pOutBuf);

	if (_u8Sign == 1)
	{
		BitSet(pOutBuf[0], 7);
	}

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA5���ݸ�ʽ
 * ��������:DT_DoubleToDataType05
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A5���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType05(fp64 fpValue, uint8 * pOutBuf)
{
	fp64 _fpValue = 0;
	uint8 _u8Sign = 0;

	if (fpValue < 0)
	{
		_u8Sign = 1;
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 1000)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 3, 1, 2, pOutBuf);

	if (_u8Sign == 1)
	{
		BitSet(pOutBuf[1], 7);
	}

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA6���ݸ�ʽ
 * ��������:DT_DoubleToDataType06
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A6���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType06(fp64 fpValue, uint8 * pOutBuf)
{
	fp64 _fpValue = 0;
	uint8 _u8Sign = 0;

	if (fpValue < 0)
	{
		_u8Sign = 1;
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 100)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 2, 2, 2, pOutBuf);

	if (_u8Sign == 1)
	{
		BitSet(pOutBuf[1], 7);
	}

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA7���ݸ�ʽ
 * ��������:DT_DoubleToDataType07
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A7���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType07(fp64 fpValue, uint8 * pOutBuf)
{
	fp64 _fpValue = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 1000)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 3, 1, 2, pOutBuf);

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA8���ݸ�ʽ
 * ��������:DT_DoubleToDataType08
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A8���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType08(fp64 fpValue, uint8 * pOutBuf)
{
	fp64 _fpValue = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 10000)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 4, 0, 2, pOutBuf);

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA9���ݸ�ʽ
 * ��������:DT_DoubleToDataType09
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A9���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType09(fp64 fpValue, uint8 * pOutBuf)
{
	fp64 _fpValue = 0;
	uint8 _u8Sign = 0;

	if (fpValue < 0)
	{
		_u8Sign = 1;
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 100)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 2, 4, 3, pOutBuf);

	if (_u8Sign == 1)
	{
		BitSet(pOutBuf[2], 7);
	}

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA10���ݸ�ʽ
 * ��������:DT_DoubleToDataType10
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A10���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType10(fp64 fpValue, uint8 * pOutBuf)
{
	fp64 _fpValue = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 1000000)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 6, 0, 3, pOutBuf);

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA11���ݸ�ʽ
 * ��������:DT_DoubleToDataType11
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A11���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType11(fp64 fpValue, uint8 * pOutData)
{
	fp64 _fpValue = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 1000000)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 6, 2, 4, pOutData);

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA12���ݸ�ʽ
 * ��������:DT_DoubleToDataType12
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A12���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType12(fp64 fpValue, uint8 * pOutData)
{
	fp64 _fpValue = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 1000000000000.0)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 12, 0, 6, pOutData);

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA13���ݸ�ʽ
 * ��������:DT_DoubleToDataType13
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A13���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType13(fp64 fpValue, uint8 * pOutData)
{
	fp64 _fpValue = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 10000)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 4, 4, 4, pOutData);

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA14���ݸ�ʽ
 * ��������:DT_DoubleToDataType14
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A14���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType14(fp64 fpValue, uint8 * pOutData)
{
	fp64 _fpValue = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 1000000)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 6, 4, 5, pOutData);

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA22���ݸ�ʽ
 * ��������:DT_DoubleToDataType22
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A22���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType22(fp64 fpValue, uint8 * pOutData)
{
	fp64 _fpValue = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 10)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 1, 1, 1, pOutData);

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA23���ݸ�ʽ
 * ��������:DT_DoubleToDataType23
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A23���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType23(fp64 fpValue, uint8 * pOutData)
{
	fp64 _fpValue = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 100)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 2, 4, 3, pOutData);

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA25���ݸ�ʽ
 * ��������:DT_DoubleToDataType25
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A25���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType25(fp64 fpValue, uint8 * pOutBuf)
{
	fp64 _fpValue = 0;
	uint8 _u8Sign = 0;

	if (fpValue < 0)
	{
		_u8Sign = 1;
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 1000)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 3, 3, 3, pOutBuf);

	if (_u8Sign == 1)
	{
		BitSet(pOutBuf[2], 7);
	}

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA26���ݸ�ʽ
 * ��������:DT_DoubleToDataType26
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A26���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType26(fp64 fpValue, uint8 * pOutData)
{
	fp64 _fpValue = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 100)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 2, 2, 2, pOutData);

	return 0;
}

/*******************************************************
 * ��������:��double��������תΪA27���ݸ�ʽ
 * ��������:DT_DoubleToDataType27
 * �������:<fpValue>[uint8] ��ת��double��������
 * �������:<pOutBuf>[uint8 *] ת����A26���ݸ�ʽ
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
static int8 DT_DoubleToDataType27(fp64 fpValue, uint8 * pOutData)
{
	fp64 _fpValue = 0;

	if (fpValue < 0)
	{
		_fpValue = -1 * fpValue;
	}
	else
	{
		_fpValue = fpValue;
	}

	if (_fpValue >= 100000000)
	{
		return -1;
	}

	DT_BaseDoubleToDataType(_fpValue, 8, 0, 4, pOutData);

	return 0;
}

/*******************************************************
 * ��������:��double����תΪָ����ʽ���ַ�������
 * ��������:DT_DataTypeToDouble
 * �������:<u8Type>[uint8] ��ת���ַ�����ʽ
 * �������:<fpValue>[fp64] ��ת������
 * �������:<pOutData>[uint8 *] ת��������
 * ��   ��  ֵ:0 �ɹ��� -1 ʧ��
 *******************************************************/
int8 DT_DoubleToDataType(uint8 u8Type, const fp64 fpValue, uint8 *pOutData)
{
	int8 _u8Rst = 0;

	switch (u8Type)
	{
		case 2:
		{
			_u8Rst = DT_DoubleToDataType02(fpValue, pOutData);
			break;
		}
		case 3:
		{
			_u8Rst = DT_DoubleToDataType03(fpValue, pOutData);
			break;
		}
		case 4:
		{
			_u8Rst = DT_DoubleToDataType04(fpValue, pOutData);
			break;
		}
		case 5:
		{
			_u8Rst = DT_DoubleToDataType05(fpValue, pOutData);
			break;
		}
		case 6:
		{
			_u8Rst = DT_DoubleToDataType06(fpValue, pOutData);
			break;
		}
		case 7:
		{
			_u8Rst = DT_DoubleToDataType07(fpValue, pOutData);
			break;
		}
		case 8:
		{
			_u8Rst = DT_DoubleToDataType08(fpValue, pOutData);
			break;
		}
		case 9:
		{
			_u8Rst = DT_DoubleToDataType09(fpValue, pOutData);
			break;
		}
		case 10:
		{
			_u8Rst = DT_DoubleToDataType10(fpValue, pOutData);
			break;
		}
		case 11:
		{
			_u8Rst = DT_DoubleToDataType11(fpValue, pOutData);
			break;
		}
		case 12:
		{
			_u8Rst = DT_DoubleToDataType12(fpValue, pOutData);
			break;
		}
		case 13:
		{
			_u8Rst = DT_DoubleToDataType13(fpValue, pOutData);
			break;
		}
		case 14:
		{
			_u8Rst = DT_DoubleToDataType14(fpValue, pOutData);
			break;
		}
		case 22:
		{
			_u8Rst = DT_DoubleToDataType22(fpValue, pOutData);
			break;
		}
		case 23:
		{
			_u8Rst = DT_DoubleToDataType23(fpValue, pOutData);
			break;
		}
		case 25:
		{
			_u8Rst = DT_DoubleToDataType25(fpValue, pOutData);
			break;
		}
		case 26:
		{
			_u8Rst = DT_DoubleToDataType26(fpValue, pOutData);
			break;
		}
		case 27:
		{
			_u8Rst = DT_DoubleToDataType27(fpValue, pOutData);
			break;
		}
	}

	if (_u8Rst == -1)
	{
		DEBUG("���ݸ�ʽ������%d,%f\r\n", u8Type, fpValue);
		DEBUG("�����%d\r\n", OSTCBCur->OSTCBPrio);
	}

	return _u8Rst;
}

/*******************************************************
 * ��������:������תΪ�ַ���
 * ��������:DT_Sprintf
 * �������:<fpValue>[fp64] ת������
 * �������:<u8DecimalBit>[uint8] С��λ
 * �������:<pOutData>[uint8 *] ת��������
 * ��   ��  ֵ:��
 *******************************************************/
void DT_Sprintf(const fp64 fpValue, uint8 u8DecimalBit, uint8 *pOutData)
{
	uint8 _Tmp01[100]; // ��ʱ����
	uint32 _u32TotalLen = 0; // �ܳ���
	uint32 _u32Val1 = 0, _u32Val2 = 0;

	_u32Val1 = DT_MyPow(u8DecimalBit);
	_u32Val2 = ((uint32)(fpValue)) * _u32Val1;  // ��������
	_u32Val1 = fpValue * _u32Val1 - _u32Val2;   // С������
	sprintf((char *) _Tmp01, "%d", _u32Val1);
	_u32TotalLen = strlen((char *) _Tmp01);

	if (_u32TotalLen < u8DecimalBit)
	{
		sprintf((char *) pOutData, "%d.%-s%0*d", (uint32)(fpValue), _Tmp01, u8DecimalBit - _u32TotalLen, 0);
	}
	else if (u8DecimalBit == 0)
	{
		sprintf((char *) pOutData, "%d", (uint32)(fpValue));
	}
	else
	{
		sprintf((char *) pOutData, "%d.%d", (uint32)(fpValue),  _u32Val1);
	}
}
