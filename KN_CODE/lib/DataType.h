/************************************************************
 * Copyright (c) 2009,漳州科能电器有限公司
 * All rights reserved. 
 *
 * 文件名称：DataFormat.h
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

#ifndef DATATYPE_H_
#define DATATYPE_H_

/*******************************************************
 * 函数功能:将字符串数据转为double类型
 * 函数名称:DT_DataTypeToDouble
 * 输入参数:<u8Type>[uint8] 待转换字符串格式
 * 输入参数:<pData>[uint8 *] 待转换字符串
 * 输出参数:<fpValue>[fp64 *] 转换后double类型数据
 * 返   回  值:0 成功， -1 失败
 *******************************************************/
extern int8 DT_DataTypeToDouble(uint8 u8Type, const uint8 *pData, fp64 *fpValue);

/*******************************************************
 * 函数功能:将double类型转为指定格式的字符串数据
 * 函数名称:DT_DataTypeToDouble
 * 输入参数:<u8Type>[uint8] 待转换字符串格式
 * 输入参数:<fpValue>[fp64] 待转换数据
 * 输出参数:<pOutData>[uint8 *] 转换后数据
 * 返   回  值:0 成功， -1 失败
 *******************************************************/
extern int8 DT_DoubleToDataType(uint8 u8Type, const fp64 fpValue, uint8 *pOutData);

/*******************************************************
 * 函数功能:浮点数转为字符串
 * 函数名称:DT_Sprintf
 * 输入参数:<fpValue>[fp64] 转换数据
 * 输入参数:<u8DecimalBit>[uint8] 小数位
 * 输出参数:<pOutData>[uint8 *] 转换后数据
 * 返   回  值:无
 *******************************************************/
extern void DT_Sprintf(const fp64 fpValue, uint8 u8DecimalBit, uint8 *pOutData);

#endif /* DATATYPE_H_ */
