/************************************************************
	 * Copyright (c) 2009,漳州科能电器有限公司
 * All rights reserved. 
 *
 * 文件名称：sys_config.h
 * 文件标识：见配置管理计划书 
 * 描述： 本系统相关的环境变量及数据类型声明文件
 *
 * 当前版本： 1.0
 * 创建者：林冬武
 * 创建日期： 2010-3-10
 *
 * 修改人：
 * 修改日期：
 * 修改原因：
 **********************************************************/

#ifndef SYS_CONFIG_H_
#define SYS_CONFIG_H_

//========================各任务优先级定义======================//
#define	DOGTSK_PRI			         4

#define UPTSK_PRI                   10
#define WIFITSK_PRI                11

#define DATASTRTSK_PRI              12
#define DMREADTSK_PRI               13

#define LEDTSK_PRI		            14
//=============================================================//

//=======================程序发布编译时注意宏定义说明=============//
// RS232默认为监控口时编译器里要加上宏定议_DEBUG_
// 用旧标准的停上电检测编译器里要加上宏定义_POWOFF_
// 不同地区版本宏定义如下:
// 国网检测 版本 _GWDL_
// 湖北版本 _HBDL_
// 东石版本 _DSDL_
//=============================================================//

//========================终端版本信息数据======================//
#define CONS_MANUFACTURER_UX "ZZKN"           // 版本前缀

#define CONS_MANUFACTURER_CODE "ZZKN"         // 厂商代号


#define CONS_SOFTWARE "SVER"                  // 软件版本
#define CONS_SOFTWARE_VERSION "V0.1"          // 软件版本
#define CONS_SOFTWARE_RELEASE "\x14\x01\x14"  // 软件发布日期
#define CONS_INNER_VERSION "000-0001-131019"  // 内部版本

#define CONS_REGI_XH "KN1003"                 // 注册型号
#define CONS_HANDWARE_VERSION "V0.4"          // 硬件版本
#define CONS_HANDWARE_RELEASE "\x26\x08\x13"  // 硬件发布日期
#define CONS_CONFIG_INFO "FLASH128"           // 配置容量信息码
#define CONS_PROTOCOL_VERSION "2013"          // 通讯协议版本
//=============================================================//

//====================终端输入、输出及端口配置===================//
#define CONS_MAX_PULSE	2            // 最大脉冲路数
#define CONS_MAX_INSWITCH  4         // 最大开关量输入路数
#define CONS_MAX_VOLTAGEORCURRENT 64 // 最大电压/电流模拟量配路数
#define CONS_MAX_VALIDPORT 31        // 最大有效抄表端口个数(1:交采,2:RS4851,3:RS4852)
#define CONS_MAX_DC  0               // 最大的直流模拟量输入路数
#define CONS_MAX_OUTWITCH  4         // 最大开关量输出路数(轮次)
#define CONS_MAX_METER  16           // 最大的抄电能表/交流采样状置个数
#define CONS_MAX_RECBUF  2048        // 上行通信最大接收缓存区字节数
#define CONS_MAX_SENDBUF 2048        // 上行通信最大发送缓存区字节数
#define CONS_MAX_COMMPORT  2         // 最大的通信端口数量
#define CONS_COMMPORT_01  "\x82\x00\x80\x25\x00\x00\x40\x00\x40\x00\x40\x00"     // 第1个通信口RS485_1 2  9600
#define CONS_COMMPORT_02  "\x83\x00\x80\x25\x00\x00\x40\x00\x40\x00\x40\x00"     // 第2个通信口RS485_2 3  9600
//=============================================================//

//=======================终端支持的其它配置====================//


#define CONS_MAX_METER_NO 1024      // 最多的测量点数
#define CONS_MAX_RUC_COUNT 1024     // 采集器最大个数
#define CONS_MAX_IMPORTS_COUNT 20   // 重点户最多户数

#define CONS_MAX_LAMM_NO  8        // 台区表测量点数
#define CONS_MAX_GROUP_NO 8	       // 最大的总加组号
#define CONS_MAX_TASK_NO  64	   // 最大的任务号
#define CONS_MAX_GROUP_METER 64    // 总加组最大测量点数
#define CONS_MAX_DIFF     8        // 最大有功总电能量差动组最多组数
#define CONS_MAX_RATES    4        // 最大费率数
#define CONS_MAX_METER_DF 1        // 最大测量点数据冻结密度15min
#define CONS_MAX_GROUP_AEF 1       // 最大总加组有功功率数据冻结密度15min
#define CONS_MAX_GROUP_REF 1       // 最大总加组无功功率数据冻结密度15min
#define CONS_MAX_GROUP_APF 1       // 最大总加组有功电能量数据冻结密度15min
#define CONS_MAX_GROUP_RPF 1       // 最大总加组无功电能量数据冻结密度15min
#define CONS_MAX_DAY_DATA 62       // 日数据最多存放天数
#define CONS_MAX_MONTH_DATA 12     // 月数据最多存放月数
#define CONS_MAX_CBDAY_DATA 12     // 抄表日最多存放天数
#define CONS_MAX_TIME_POWERCTRL 3  // 时段功控定值方案
#define CONS_MAX_HARMONIC_NUMS 21  // 谐波检测最高谐波次数
#define CONS_MAX_CAPACITORS_NUMS 8 // 无功补偿电容器组最多组数
#define CONS_ZERO_LIMIT 0.0000000  // 为0限值

#define CONS_MAX_USER_CNO  "\x7F\x00" // 支持的用户大类号标志 0、 1、 2
#define CONS_MAX_USER_SNO  "\x10\x10\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // 支持n号用户大类下的用户小类号个数
#define CONS_RAM_SAVE_INTER_MIN  15 * 60 * OS_TICKS_PER_SEC // 定时保存数据时间间隔(tick)
//=============================================================//

//========================终端支持的参数配置=====================//
#define CONS_PARAM_CONFIG "\x13\xFF\xFF\x7F\xFF\x7F\xFF\x01\x1F\x0F\x0F\x0F\x07\x83\x07\x00\x00\x00\x00\x20"
//=============================================================//

//========================终端支持的控制配置=====================//
#define CONS_CTRL_CONFIG  "\x13\x03\xCF\xCF\xFF\x7F\x00\x1F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x30"
//==============================================================//

//=======================终端支持的1类数据配置====================//
#define CONS_CLASS1_CONFIG "\x17\xFF\x3F\xFF\xCF\xFF\xFF\x01\x03\x00\x00\x0F\xFF\x7F\xFF\x0F\x00\xFF\xFF\xFF\x00\xF3\x03\x03"
//==============================================================//

//=======================终端支持的2类数据配置====================//
#define CONS_CLASS2_CONFIG "\x1C\xFF\x0F\xFF\xFF\x7F\x0C\x3F\x3F\x03\x0F\xFF\x7F\xFF\x3F\x3F\x3F\x00\x00\x0F\xFF\xFF\xFF\xFF\xFF\x0F\x00\xFF\x04"
//==============================================================//

//=======================终端支持的2类整点曲线数据配置====================//
#define CONS_HOURCURVE_CONFIG "\x0D\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x55"
//==============================================================//

//=======================终端支持的2类数据配置====================//
#define CONS_TERMAINAL_CONFIG "\x07\x00\x00\x00\x00\x00\x00\x3F"
//==============================================================//

//=======================终端支持的2类直流模拟量数据配置====================//
#define CONS_ZLMNL_CONFIG "\x12\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x02"
//==============================================================//

//======================终端支持的事件记录配置====================//
#define CONS_EVENT_CONFIG "\x08\xFF\x3F\xFD\xFF\x03\x00\x00\x00"
//==============================================================//

//======================总加组支持的数据项配置====================//
#define CONS_SUMGROUP_CONFIG1 "\x0B\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x0F"
#define CONS_SUMGROUP_CONFIG2 "\x0A\x00\x00\x00\x00\x00\x00\x00\xFF\x03\x0F"
//==============================================================//

//======================脉冲量支持的数据项配置====================//
#define CONS_PLUSE_CONFIG1 "\x13\x00\x00\x00\x01\x0F\xFF\x00\x00\x00\x00\x00\x11\x00\xFF\x00\x00\x0F\x00\x0F"
#define CONS_PLUSE_CONFIG2 "\x19\xFF\x0F\xFF\x43\x43\x00\x00\x00\x00\x00\x11\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x0F\x0F\x0F\xFF\xFF"
//==============================================================//

//======================各类数据抄收数据项配置====================//
#define CONS_READ_CLASS10_CONFIG "\x15\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00"
#define CONS_READ_CLASS20_CONFIG "\x19\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x05\x05\x00\x00"

#define CONS_READ_CLASS16_CONFIG "\x15\x00\x00\x00\xCF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\x00\x00"
#define CONS_READ_CLASS26_CONFIG "\x19\xFF\x4F\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\xFF\x0F"

//==============================================================//

//======================终端数据存储文件配置=====================//
#define FILES_NAME_MAXLEN          164                      // 文件名最大长度
#define FILES_TERMINAL_STAUS       "/param/stausparm.dat"   // 终端状态文件
#define FILES_TERMINAL_PARAM       "/param/baseparam.dat"   // 终端参数文件
#define FILES_TERMINAL_CTRLPARAM   "/param/ctrlparam.dat"   // 终端控制命令参数文件

#define FILES_METER_BASE           "/param/meterbase.dat"   // 电能表参数文件
#define FILES_METER_EVENT          "/param/meterevent.dat"  // 电能表事件文件
#define FILES_METER_STATE          "/param/meterstate.dat"  // 电能表状态文件
#define FILES_METER_PF             "/param/meterpf.dat"     // 电容器参数文件

#define FILES_MDMTSK_DIR           "/param/mdmtsk.dat"      // 台区表任务状态参数
#define FILES_IMPTSK_DIR           "/param/imptsk.dat"      // 重点表任务状态参数
#define FILES_NORTSK_DIR           "/param/nortsk.dat"      // 普通表任务状态参数
#define FILES_CURTSK_DIR           "/param/curtsk.dat"      // 曲线任务状态参数
#define FILES_READSTA_DIR          "/param/readstats.dat"   // 电能表抄读状态参数
#define FILES_REPMETER_DIR         "/param/repmeter.dat"    // 搜到的电能表信息

#define FILES_CLASS1_DIR           "/class1/realdir.dat"    // 1类数据存储目录文件
#define FILES_CLASS1_DATA          "/class1/"               // 1类数据存储目录

#define FILES_CLASS4_DATA          "/class2/nordata.dat"    // 4类抄表数据文件 (集中器1类数据)
#define FILES_CLASS2_FREEZEDIR     "/class2/tqfreezedir.dat"// 2类数据冻结类型存储目录文件
#define FILES_CLASS5_FREEZEDIR     "/class2/nofreezedir.dat"// 5类数据冻结类型存储目录文件 (集中器2类数据)
#define FILES_CLASS2_DAY           "/class2/"               // 2类数据日末存储文件目录
#define FILES_CLASS2_MONTH         "/class2/"               // 2类数据月末存储文件目录
#define FILES_CLASS2_CURVE         "/class2/"               // 2类数据曲线存储文件目录
#define FILES_CLASS2_CBDAY         "/class2/"               // 2类数据抄表日存储文件目录
#define FILES_CLASS2_VOLCUR        "/class2/volcur.dat"     // 2类数据电压电流曲线数据(湖北)
#define FILES_UPDATE_CONFIG        "/class2/update.config"  // 升级程序配置文件
#define FILES_UPDATE_PACKFILE      "/class2/update.bin"     // 升级程序包
#define FILES_READMETER_LOG        "/class2/rmeterlog.dat"  // 抄表日志

#define FILES_CLASS3_IMP		   "/class3/IMP.dat"		// 3类数据重要事件记录文件
#define FILES_CLASS3_COM		   "/class3/COM.dat"		// 3类数据一般事件记录文件

//===============================================================//

#endif /* SYS_CONFIG_H_ */
