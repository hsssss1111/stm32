/************************************************************
 * Copyright (c) 2009,漳州科能电器有限公司
 * All rights reserved. 
 *
 * 文件名称：DataFormat.c
 * 文件标识：见配置管理计划书 
 * 描述： 数据格式化处理模块
 *
 * 当前版本： 1.0
 * 创建者：林冬武
 * 创建日期： 2010-4-9
 *
 * 修改人：
 * 修改日期：
 * 修改原因：
 **********************************************************/

//*====================================================
//*　　　　　　　　　头　文　件
//*====================================================
#include <math.h>
#include "config.h"
#include "datatype.h"
#include "app_util.h"
#include ".\kn_code\driver\rtc\rtc.h"
#include ".\kn_code\lib\debug.h"

//*====================================================
//*　　　　　　　　　常量、变量、宏
//*====================================================


//*====================================================
//*　　　　　　　　　 函　数　区
//*====================================================

/*******************************************************
 * 函数功能:10的n次幂运算
 * 函数名称:DT_MyPow
 * 输入参数:<s16X1>[int16] 幂运算次数
 * 返   回  值:double类型的幂运算结果
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
 * 函数功能:字符串转为Double类型数据
 * 函数名称:DT_DataTypeToDouble
 * 输入参数:<s16Begin>[int16] 待转换字符串指定起始值
 * 输入参数:<u16Num>[uint16] 待转换字符串数据位长度
 * 输入参数:<pBuf>[uint8 *] 待转换字符串
 * 返   回  值:转换后的double类型数据
 *******************************************************/
static fp64 DT_BaseTypeToDouble(int16 s16Begin, uint16 u16Num, const uint8 * pBuf)
{
	fp64 _fpValue = 0;
	uint16 i = 0, j = 0;
	int16 _s16Zs = 0;  // 指数
	uint8 _u8Tmp = 0;   // 临时变量

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
 * 函数功能:数据格式A02转为Double类型
 * 函数名称:DT_DataType02ToDouble
 * 输入参数:<pData02>[uint8 *] 待转换A02字符串数据
 * 输出参数:<fpValue>[fp64 *] 转换后double类型数据
 * 返   回  值:0
 *******************************************************/
static int8 DT_DataType02ToDouble(const uint8 *pData02, fp64 * fpValue)
{
	fp64 _fpTmp = 0;
	int8 _s8Flag = 1;
	uint8 _u8Zs = 0;

	_fpTmp = DT_BaseTypeToDouble(0, 3, pData02);

	if (((pData02[1] >> 4) & 0x01) == 0) // 符号位
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
 * 函数功能:数据格式A03转为Double类型
 * 函数名称:DT_DataType03ToDouble
 * 输入参数:<pData03>[uint8 *] 待转换A03字符串数据
 * 输出参数:<fpValue>[fp64 *] 转换后double类型数据单位(kwh/厘)
 * 返   回  值:0
 *******************************************************/
static int8 DT_DataType03ToDouble(const uint8 *pData03, fp64 * fpValue)
{
	fp64 _fpTmp = 0;
	int8 _s8Flag = 1;
	uint16 _u16Rate = 0;

	_fpTmp = DT_BaseTypeToDouble(0, 7, pData03);

	if (((pData03[3] >> 4) & 0x01) == 0) // 符号位
	{
		_s8Flag = 1;
	}
	else
	{
		_s8Flag = -1;
	}

	if (((pData03[3] >> 6) & 0x01) == 0) // 单位
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
 * 函数功能:数据格式A04转为Double类型
 * 函数名称:DT_DataType04ToDouble
 * 输入参数:<pData04>[uint8 *] 待转换A04字符串数据
 * 输出参数:<fpValue>[fp64 *] 转换后double类型数据
 * 返   回  值:0
 *******************************************************/
static int8 DT_DataType04ToDouble(const uint8 *pData04, fp64 * fpValue)
{
	fp64 _fpTmp = 0;

	_fpTmp = DT_BaseTypeToDouble(0, 1, pData04); // 个位
	_fpTmp += ((pData04[0] >> 4) & 0x07) * 10; // 十位

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
 * 函数功能:数据格式A05转为Double类型
 * 函数名称:DT_DataType05ToDouble
 * 输入参数:<pData05>[uint8 *] 待转换A05字符串数据
 * 输出参数:<fpValue>[fp64 *] 转换后double类型数据
 * 返   回  值:0
 *******************************************************/
static int8 DT_DataType05ToDouble(const uint8 *pData05, fp64 * fpValue)
{
	fp64 _fpTmp = 0;

	_fpTmp = DT_BaseTypeToDouble(-1, 3, pData05); // 个位
	_fpTmp += ((pData05[1] >> 4) & 0x07) * 100; // 百位

	if (((pData05[1] >> 7) & 0x01) != 0)
	{
		_fpTmp = -1 * _fpTmp;
	}

	*fpValue = _fpTmp;

	return 0;
}

/*******************************************************
 * 函数功能:数据格式A05转为Double类型
 * 函数名称:DT_DataType06ToDouble
 * 输入参数:<pData05>[uint8 *] 待转换A05字符串数据
 * 输出参数:<fpValue>[fp64 *] 转换后double类型数据
 * 返   回  值:0
 *******************************************************/
static int8 DT_DataType06ToDouble(const uint8 *pData06, fp64 * fpValue)
{
	fp64 _fpTmp = 0;

	_fpTmp = DT_BaseTypeToDouble(-2, 3, pData06); // 个位
	_fpTmp += ((pData06[1] >> 4) & 0x07) * 10; // 十位

	if (((pData06[1] >> 7) & 0x01) != 0)
	{
		_fpTmp = -1 * _fpTmp;
	}

	*fpValue = _fpTmp;

	return 0;
}

/*******************************************************
 * 函数功能:数据格式A09转为Double类型
 * 函数名称:DT_DataType09ToDouble
 * 输入参数:<pData09>[uint8 *] 待转换A05字符串数据
 * 输出参数:<fpValue>[fp64 *] 转换后double类型数据
 * 返   回  值:0
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
 * 函数功能:数据格式A25转为Double类型
 * 函数名称:DT_DataType25ToDouble
 * 输入参数:<pData25>[uint8 *] 待转换A25字符串数据
 * 输出参数:<fpValue>[fp64 *] 转换后double类型数据
 * 返   回  值:0
 *******************************************************/
static int8 DT_DataType25ToDouble(const uint8 *pData25, fp64 * fpValue)
{
	fp64 _fpTmp = 0;

	_fpTmp = DT_BaseTypeToDouble(-3, 5, pData25); //
	_fpTmp += ((pData25[2] >> 4) & 0x07) * 100; // 百位

	if (((pData25[2] >> 7) & 0x01) != 0)
	{
		_fpTmp = -1 * _fpTmp;
	}

	*fpValue = _fpTmp;

	return 0;
}

/*******************************************************
 * 函数功能:将字符串数据转为double类型
 * 函数名称:DT_DataTypeToDouble
 * 输入参数:<u8Type>[uint8] 待转换字符串格式
 * 输入参数:<pData>[uint8 *] 待转换字符串
 * 输出参数:<fpValue>[fp64 *] 转换后double类型数据
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为指定格式的字符串
 * 函数名称:DT_BaseDoubleToDataType
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输入参数:<u8IntegerBit>[uint8] 待转换数据整数位
 * 输入参数:<u8IntegerBit>[uint8] 待转换数据小数位
 * 输入参数:<u8IntegerBit>[uint8] 待转换数据字节数
 * 输出参数:<pOutBuf>[uint8 *] 转换后字符串
 * 返   回  值:0 成功， -1 失败
 *******************************************************/
static int8 DT_BaseDoubleToDataType(fp64 fpValue, uint8 u8IntegerBit, uint8 u8DecimalBit, uint8 u8ByteNum, uint8 *pOutBuf)
{
//	uint8 _Tmp01[100]; // 临时变量
//	uint8 _Tmp02[100]; // 临时变量
//	uint8 _u8Char = 0; // 当前字符
//	uint8 _u8Pos = 0; // 当前位置
//	uint16 _u16lb = 0; // 循环计数器
//	uint32 _u32TotalLen = 0; // 总长度
//	uint32 _u32Val1 = 0, _u32Val2 = 0;
//
//	if (u8IntegerBit == 0)
//	{
//		return -1;
//	}
//
//	memset(pOutBuf, 0x0, u8ByteNum); // 将输出字符串数据置0
//	_u32Val1 = DT_MyPow(u8DecimalBit);
//	_u32Val2 = ((uint32)(fpValue)) * _u32Val1;  // 整数部份
//	_u32Val1 = fpValue * _u32Val1 - _u32Val2;   // 小数部份
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

	uint8 _Tmp01[100]; // 临时变量
	uint8 _Tmp02[100]; // 临时变量
	uint8 _u8Char = 0; // 当前字符
	uint8 _u8Pos = 0; // 当前位置
	uint16 _u16lb = 0; // 循环计数器
	uint32 _u32TotalLen = 0; // 总长度

	if (u8IntegerBit == 0)
	{
		return -1;
	}

	memset(pOutBuf, 0x0, u8ByteNum); // 将输出字符串数据置0
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
 * 函数功能:将double类型数据转为A2数据格式
 * 函数名称:DT_DoubleToDataType02
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A2数据格式
 * 返   回  值:0 成功， -1 失败
 *******************************************************/
static int8 DT_DoubleToDataType02(fp64 fpValue, uint8 * pOutData)
{
	uint8 _u8Sign = 0; // 符号标志
	uint32 _u32TmpVal = 0; // 临时变量
	fp64 _fpFabsResult = 0; // 转换数据的绝对值
	div_t _divRst; // 两个整数相除结果

	// 求绝对值
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

	// 数值越限(10^4 * 999)
	if (_fpFabsResult >= 10000000.00)
	{
		return -1;
	}

	if (_fpFabsResult < 1.00)
	{
		if ((_fpFabsResult * 1000) > 999.50)
		{
			_u32TmpVal = (uint32) (_fpFabsResult * 1000); //放大1000倍,但不能四舍五入
		}
		else
		{
			_u32TmpVal = (uint32) (_fpFabsResult * 1000 + 0.5); //放大1000倍
		}

		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0xE0 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(-3)
	}
	else if (_fpFabsResult < 9.95) //精度千分之五
	{
		_u32TmpVal = (uint32) (_fpFabsResult * 100 + 0.5); //放大100倍
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0xC0 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(-2)
	}
	else if (_fpFabsResult < 99.95) //精度万分之五
	{
		_u32TmpVal = (uint32) (_fpFabsResult * 10 + 0.5); //放大10倍
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0xA0 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(-1)
	}
	else if (_fpFabsResult < 999.50) //精度万分之五
	{
		_u32TmpVal = (uint32) (_fpFabsResult + 0.5); //四舍五入,并转换为整数
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0x80 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(0)
	}
	else if (_fpFabsResult < 9995.00) //精度万分之五
	{
		_u32TmpVal = (uint32) (_fpFabsResult / 10 + 0.5); //四舍五入,并转换为整数
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0x60 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(1)
	}
	else if (_fpFabsResult < 99950.00) //精度万分之五
	{
		_u32TmpVal = (uint32) (_fpFabsResult / 100 + 0.5); //四舍五入,并转换为整数
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0x40 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(2)
	}
	else if (_fpFabsResult < 999500.00) //精度万分之五
	{
		_u32TmpVal = (uint32) (_fpFabsResult / 1000 + 0.5); //四舍五入,并转换为整数
		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0x20 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); // 10^(3)
	}
	else
	{
		if (_fpFabsResult < 9995000.00) //精度万分之五
		{
			_u32TmpVal = (uint32) (_fpFabsResult / 10000 + 0.5); //四舍五入,并转换为整数
		}
		else //精度千分之一
		{
			_u32TmpVal = (uint32) (_fpFabsResult / 10000); //不能四舍五入,否则不能表达此数,并转换为整数
		}

		_divRst = div(_u32TmpVal, 100);
		pOutData[0] = Bin2BCD(_divRst.rem & 0xFF);
		pOutData[1] = 0x00 + (Bin2BCD(_divRst.quot & 0xFF) & 0x0F); //10^(4)
	}

	//改变符号位
	if (_u8Sign == 1)
	{
		BitSet(pOutData[1], 4); // 负数
	}

	return 0;
}

/*******************************************************
 * 函数功能:将double类型数据转为A3数据格式
 * 函数名称:DT_DoubleToDataType03
 * 输入参数:<fpValue>[uint8] 待转换double类型数据(kwh/厘)
 * 输出参数:<pOutBuf>[uint8 *] 转换后A3数据格式
 * 返   回  值:0 成功， -1 失败
 *******************************************************/
static int8 DT_DoubleToDataType03(fp64 fpValue, uint8 * pOutData)
{
	uint8 _u8Sign = 0; // 符号标志
	uint8 _u8Uint = 0; // 单位(MWh/Kwh)(元/厘)
	uint8 _u8lb = 0; // 循环计数器
	uint32 _u32TmpVal = 0; // 临时变量
	fp64 _fpFabsResult = 0; // 转换数据的绝对值
	div_t _divRst; // 两个整数相除结果

	// 求绝对值
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

	// 数值越限
	if (_fpFabsResult >= 10000000000.00)
	{
		return -1;
	}

	// 确定G的符号
	if (_fpFabsResult >= 10000000.00)
	{
		_u8Uint = 1; //MWh/元
		_fpFabsResult = _fpFabsResult / 1000;

	}

	if ((_fpFabsResult + 0.5) > 10000000.00)
	{
		_u32TmpVal = (uint32) _fpFabsResult; //不进行四舍五入,先转换为整数
	}
	else
	{
		_u32TmpVal = (uint32) (_fpFabsResult + 0.5); //进行四舍五入,并转换为整数
	}

	for (_u8lb = 0; _u8lb < 4; _u8lb++)
	{
		_divRst = div(_u32TmpVal, 100);
		pOutData[_u8lb] = Bin2BCD(_divRst.rem & 0xFF);
		_u32TmpVal = _divRst.quot;
	}

	if (_u8Sign == 1)
	{
		BitSet(pOutData[3], 4); //负数
	}

	if (_u8Uint == 1)
	{
		BitSet(pOutData[3], 6); //MWh/元
	}

	return 0;
}

/*******************************************************
 * 函数功能:将double类型数据转为A4数据格式
 * 函数名称:DT_DoubleToDataType04
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A4数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A5数据格式
 * 函数名称:DT_DoubleToDataType05
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A5数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A6数据格式
 * 函数名称:DT_DoubleToDataType06
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A6数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A7数据格式
 * 函数名称:DT_DoubleToDataType07
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A7数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A8数据格式
 * 函数名称:DT_DoubleToDataType08
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A8数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A9数据格式
 * 函数名称:DT_DoubleToDataType09
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A9数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A10数据格式
 * 函数名称:DT_DoubleToDataType10
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A10数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A11数据格式
 * 函数名称:DT_DoubleToDataType11
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A11数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A12数据格式
 * 函数名称:DT_DoubleToDataType12
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A12数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A13数据格式
 * 函数名称:DT_DoubleToDataType13
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A13数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A14数据格式
 * 函数名称:DT_DoubleToDataType14
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A14数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A22数据格式
 * 函数名称:DT_DoubleToDataType22
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A22数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A23数据格式
 * 函数名称:DT_DoubleToDataType23
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A23数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A25数据格式
 * 函数名称:DT_DoubleToDataType25
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A25数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A26数据格式
 * 函数名称:DT_DoubleToDataType26
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A26数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型数据转为A27数据格式
 * 函数名称:DT_DoubleToDataType27
 * 输入参数:<fpValue>[uint8] 待转换double类型数据
 * 输出参数:<pOutBuf>[uint8 *] 转换后A26数据格式
 * 返   回  值:0 成功， -1 失败
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
 * 函数功能:将double类型转为指定格式的字符串数据
 * 函数名称:DT_DataTypeToDouble
 * 输入参数:<u8Type>[uint8] 待转换字符串格式
 * 输入参数:<fpValue>[fp64] 待转换数据
 * 输出参数:<pOutData>[uint8 *] 转换后数据
 * 返   回  值:0 成功， -1 失败
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
		DEBUG("数据格式化错误%d,%f\r\n", u8Type, fpValue);
		DEBUG("任务号%d\r\n", OSTCBCur->OSTCBPrio);
	}

	return _u8Rst;
}

/*******************************************************
 * 函数功能:浮点数转为字符串
 * 函数名称:DT_Sprintf
 * 输入参数:<fpValue>[fp64] 转换数据
 * 输入参数:<u8DecimalBit>[uint8] 小数位
 * 输出参数:<pOutData>[uint8 *] 转换后数据
 * 返   回  值:无
 *******************************************************/
void DT_Sprintf(const fp64 fpValue, uint8 u8DecimalBit, uint8 *pOutData)
{
	uint8 _Tmp01[100]; // 临时变量
	uint32 _u32TotalLen = 0; // 总长度
	uint32 _u32Val1 = 0, _u32Val2 = 0;

	_u32Val1 = DT_MyPow(u8DecimalBit);
	_u32Val2 = ((uint32)(fpValue)) * _u32Val1;  // 整数部份
	_u32Val1 = fpValue * _u32Val1 - _u32Val2;   // 小数部份
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
