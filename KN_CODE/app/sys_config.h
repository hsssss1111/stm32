/************************************************************
	 * Copyright (c) 2009,���ݿ��ܵ������޹�˾
 * All rights reserved. 
 *
 * �ļ����ƣ�sys_config.h
 * �ļ���ʶ�������ù���ƻ��� 
 * ������ ��ϵͳ��صĻ����������������������ļ�
 *
 * ��ǰ�汾�� 1.0
 * �����ߣ��ֶ���
 * �������ڣ� 2010-3-10
 *
 * �޸��ˣ�
 * �޸����ڣ�
 * �޸�ԭ��
 **********************************************************/

#ifndef SYS_CONFIG_H_
#define SYS_CONFIG_H_

//========================���������ȼ�����======================//
#define	DOGTSK_PRI			         4

#define UPTSK_PRI                   10
#define WIFITSK_PRI                11

#define DATASTRTSK_PRI              12
#define DMREADTSK_PRI               13

#define LEDTSK_PRI		            14
//=============================================================//

//=======================���򷢲�����ʱע��궨��˵��=============//
// RS232Ĭ��Ϊ��ؿ�ʱ��������Ҫ���Ϻ궨��_DEBUG_
// �þɱ�׼��ͣ�ϵ����������Ҫ���Ϻ궨��_POWOFF_
// ��ͬ�����汾�궨������:
// ������� �汾 _GWDL_
// �����汾 _HBDL_
// ��ʯ�汾 _DSDL_
//=============================================================//

//========================�ն˰汾��Ϣ����======================//
#define CONS_MANUFACTURER_UX "ZZKN"           // �汾ǰ׺

#define CONS_MANUFACTURER_CODE "ZZKN"         // ���̴���


#define CONS_SOFTWARE "SVER"                  // ����汾
#define CONS_SOFTWARE_VERSION "V0.1"          // ����汾
#define CONS_SOFTWARE_RELEASE "\x14\x01\x14"  // �����������
#define CONS_INNER_VERSION "000-0001-131019"  // �ڲ��汾

#define CONS_REGI_XH "KN1003"                 // ע���ͺ�
#define CONS_HANDWARE_VERSION "V0.4"          // Ӳ���汾
#define CONS_HANDWARE_RELEASE "\x26\x08\x13"  // Ӳ����������
#define CONS_CONFIG_INFO "FLASH128"           // ����������Ϣ��
#define CONS_PROTOCOL_VERSION "2013"          // ͨѶЭ��汾
//=============================================================//

//====================�ն����롢������˿�����===================//
#define CONS_MAX_PULSE	2            // �������·��
#define CONS_MAX_INSWITCH  4         // ��󿪹�������·��
#define CONS_MAX_VOLTAGEORCURRENT 64 // ����ѹ/����ģ������·��
#define CONS_MAX_VALIDPORT 31        // �����Ч����˿ڸ���(1:����,2:RS4851,3:RS4852)
#define CONS_MAX_DC  0               // ����ֱ��ģ��������·��
#define CONS_MAX_OUTWITCH  4         // ��󿪹������·��(�ִ�)
#define CONS_MAX_METER  16           // ���ĳ����ܱ�/��������״�ø���
#define CONS_MAX_RECBUF  2048        // ����ͨ�������ջ������ֽ���
#define CONS_MAX_SENDBUF 2048        // ����ͨ������ͻ������ֽ���
#define CONS_MAX_COMMPORT  2         // ����ͨ�Ŷ˿�����
#define CONS_COMMPORT_01  "\x82\x00\x80\x25\x00\x00\x40\x00\x40\x00\x40\x00"     // ��1��ͨ�ſ�RS485_1 2  9600
#define CONS_COMMPORT_02  "\x83\x00\x80\x25\x00\x00\x40\x00\x40\x00\x40\x00"     // ��2��ͨ�ſ�RS485_2 3  9600
//=============================================================//

//=======================�ն�֧�ֵ���������====================//


#define CONS_MAX_METER_NO 1024      // ���Ĳ�������
#define CONS_MAX_RUC_COUNT 1024     // �ɼ���������
#define CONS_MAX_IMPORTS_COUNT 20   // �ص㻧��໧��

#define CONS_MAX_LAMM_NO  8        // ̨�����������
#define CONS_MAX_GROUP_NO 8	       // �����ܼ����
#define CONS_MAX_TASK_NO  64	   // ���������
#define CONS_MAX_GROUP_METER 64    // �ܼ�������������
#define CONS_MAX_DIFF     8        // ����й��ܵ���������������
#define CONS_MAX_RATES    4        // ��������
#define CONS_MAX_METER_DF 1        // �����������ݶ����ܶ�15min
#define CONS_MAX_GROUP_AEF 1       // ����ܼ����й��������ݶ����ܶ�15min
#define CONS_MAX_GROUP_REF 1       // ����ܼ����޹��������ݶ����ܶ�15min
#define CONS_MAX_GROUP_APF 1       // ����ܼ����й����������ݶ����ܶ�15min
#define CONS_MAX_GROUP_RPF 1       // ����ܼ����޹����������ݶ����ܶ�15min
#define CONS_MAX_DAY_DATA 62       // ���������������
#define CONS_MAX_MONTH_DATA 12     // ���������������
#define CONS_MAX_CBDAY_DATA 12     // ���������������
#define CONS_MAX_TIME_POWERCTRL 3  // ʱ�ι��ض�ֵ����
#define CONS_MAX_HARMONIC_NUMS 21  // г��������г������
#define CONS_MAX_CAPACITORS_NUMS 8 // �޹��������������������
#define CONS_ZERO_LIMIT 0.0000000  // Ϊ0��ֵ

#define CONS_MAX_USER_CNO  "\x7F\x00" // ֧�ֵ��û�����ű�־ 0�� 1�� 2
#define CONS_MAX_USER_SNO  "\x10\x10\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // ֧��n���û������µ��û�С��Ÿ���
#define CONS_RAM_SAVE_INTER_MIN  15 * 60 * OS_TICKS_PER_SEC // ��ʱ��������ʱ����(tick)
//=============================================================//

//========================�ն�֧�ֵĲ�������=====================//
#define CONS_PARAM_CONFIG "\x13\xFF\xFF\x7F\xFF\x7F\xFF\x01\x1F\x0F\x0F\x0F\x07\x83\x07\x00\x00\x00\x00\x20"
//=============================================================//

//========================�ն�֧�ֵĿ�������=====================//
#define CONS_CTRL_CONFIG  "\x13\x03\xCF\xCF\xFF\x7F\x00\x1F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x30"
//==============================================================//

//=======================�ն�֧�ֵ�1����������====================//
#define CONS_CLASS1_CONFIG "\x17\xFF\x3F\xFF\xCF\xFF\xFF\x01\x03\x00\x00\x0F\xFF\x7F\xFF\x0F\x00\xFF\xFF\xFF\x00\xF3\x03\x03"
//==============================================================//

//=======================�ն�֧�ֵ�2����������====================//
#define CONS_CLASS2_CONFIG "\x1C\xFF\x0F\xFF\xFF\x7F\x0C\x3F\x3F\x03\x0F\xFF\x7F\xFF\x3F\x3F\x3F\x00\x00\x0F\xFF\xFF\xFF\xFF\xFF\x0F\x00\xFF\x04"
//==============================================================//

//=======================�ն�֧�ֵ�2������������������====================//
#define CONS_HOURCURVE_CONFIG "\x0D\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x55"
//==============================================================//

//=======================�ն�֧�ֵ�2����������====================//
#define CONS_TERMAINAL_CONFIG "\x07\x00\x00\x00\x00\x00\x00\x3F"
//==============================================================//

//=======================�ն�֧�ֵ�2��ֱ��ģ������������====================//
#define CONS_ZLMNL_CONFIG "\x12\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x02"
//==============================================================//

//======================�ն�֧�ֵ��¼���¼����====================//
#define CONS_EVENT_CONFIG "\x08\xFF\x3F\xFD\xFF\x03\x00\x00\x00"
//==============================================================//

//======================�ܼ���֧�ֵ�����������====================//
#define CONS_SUMGROUP_CONFIG1 "\x0B\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x0F"
#define CONS_SUMGROUP_CONFIG2 "\x0A\x00\x00\x00\x00\x00\x00\x00\xFF\x03\x0F"
//==============================================================//

//======================������֧�ֵ�����������====================//
#define CONS_PLUSE_CONFIG1 "\x13\x00\x00\x00\x01\x0F\xFF\x00\x00\x00\x00\x00\x11\x00\xFF\x00\x00\x0F\x00\x0F"
#define CONS_PLUSE_CONFIG2 "\x19\xFF\x0F\xFF\x43\x43\x00\x00\x00\x00\x00\x11\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x0F\x0F\x0F\xFF\xFF"
//==============================================================//

//======================�������ݳ�������������====================//
#define CONS_READ_CLASS10_CONFIG "\x15\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00"
#define CONS_READ_CLASS20_CONFIG "\x19\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x05\x05\x00\x00"

#define CONS_READ_CLASS16_CONFIG "\x15\x00\x00\x00\xCF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\x00\x00"
#define CONS_READ_CLASS26_CONFIG "\x19\xFF\x4F\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\xFF\x0F"

//==============================================================//

//======================�ն����ݴ洢�ļ�����=====================//
#define FILES_NAME_MAXLEN          164                      // �ļ�����󳤶�
#define FILES_TERMINAL_STAUS       "/param/stausparm.dat"   // �ն�״̬�ļ�
#define FILES_TERMINAL_PARAM       "/param/baseparam.dat"   // �ն˲����ļ�
#define FILES_TERMINAL_CTRLPARAM   "/param/ctrlparam.dat"   // �ն˿�����������ļ�

#define FILES_METER_BASE           "/param/meterbase.dat"   // ���ܱ�����ļ�
#define FILES_METER_EVENT          "/param/meterevent.dat"  // ���ܱ��¼��ļ�
#define FILES_METER_STATE          "/param/meterstate.dat"  // ���ܱ�״̬�ļ�
#define FILES_METER_PF             "/param/meterpf.dat"     // �����������ļ�

#define FILES_MDMTSK_DIR           "/param/mdmtsk.dat"      // ̨��������״̬����
#define FILES_IMPTSK_DIR           "/param/imptsk.dat"      // �ص������״̬����
#define FILES_NORTSK_DIR           "/param/nortsk.dat"      // ��ͨ������״̬����
#define FILES_CURTSK_DIR           "/param/curtsk.dat"      // ��������״̬����
#define FILES_READSTA_DIR          "/param/readstats.dat"   // ���ܱ���״̬����
#define FILES_REPMETER_DIR         "/param/repmeter.dat"    // �ѵ��ĵ��ܱ���Ϣ

#define FILES_CLASS1_DIR           "/class1/realdir.dat"    // 1�����ݴ洢Ŀ¼�ļ�
#define FILES_CLASS1_DATA          "/class1/"               // 1�����ݴ洢Ŀ¼

#define FILES_CLASS4_DATA          "/class2/nordata.dat"    // 4�೭�������ļ� (������1������)
#define FILES_CLASS2_FREEZEDIR     "/class2/tqfreezedir.dat"// 2�����ݶ������ʹ洢Ŀ¼�ļ�
#define FILES_CLASS5_FREEZEDIR     "/class2/nofreezedir.dat"// 5�����ݶ������ʹ洢Ŀ¼�ļ� (������2������)
#define FILES_CLASS2_DAY           "/class2/"               // 2��������ĩ�洢�ļ�Ŀ¼
#define FILES_CLASS2_MONTH         "/class2/"               // 2��������ĩ�洢�ļ�Ŀ¼
#define FILES_CLASS2_CURVE         "/class2/"               // 2���������ߴ洢�ļ�Ŀ¼
#define FILES_CLASS2_CBDAY         "/class2/"               // 2�����ݳ����մ洢�ļ�Ŀ¼
#define FILES_CLASS2_VOLCUR        "/class2/volcur.dat"     // 2�����ݵ�ѹ������������(����)
#define FILES_UPDATE_CONFIG        "/class2/update.config"  // �������������ļ�
#define FILES_UPDATE_PACKFILE      "/class2/update.bin"     // ���������
#define FILES_READMETER_LOG        "/class2/rmeterlog.dat"  // ������־

#define FILES_CLASS3_IMP		   "/class3/IMP.dat"		// 3��������Ҫ�¼���¼�ļ�
#define FILES_CLASS3_COM		   "/class3/COM.dat"		// 3������һ���¼���¼�ļ�

//===============================================================//

#endif /* SYS_CONFIG_H_ */
