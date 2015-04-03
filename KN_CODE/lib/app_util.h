/****************************************Copyright (c)****************************************************
**                         							ZhangZhou KENENG electronics Co.,LTD.
**
**
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			app_util.h
** Last modified Date:  2009-05-15
** Last Version:		1.0
** Descriptions:		�ṩ��APP����е��õ�һЩ���������ӿ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:		heshusheng
** Created date:		2009-05-15
** Version:				1.0
** Descriptions:		The original version
**
**--------------------------------------------------------------------------------------------------------
	2009-05-27  by heshusheng		  add function app_checksum.
	2009-06-03  by heshusheng       add function app_hex_reverse.

*********************************************************************************************************/

#ifndef	_APP_UTIL_H_
#define	_APP_UTIL_H_

#include <time.h>

#define	SEC_PER_DAY	0x00015180
#define	MIN5_IN_SEC	0x12c
#define	ASIZE(x)	 (sizeof(x)/sizeof(x[0]))

// ��������ʱ�ӽ���֮���������(TIME1,TIME2Ϊϵͳʱ�ӽ��ļ�����,���ֵΪ0xFFFFFFFF)
#define TIME_DIFF_SEC(TIME1,TIME2) ( (TIME1 > TIME2 ? 0xFFFFFFFF - TIME1 + TIME2 : TIME2 - TIME1) / OS_TICKS_PER_SEC )

// ��������ʱ�ӽ���֮�����(TIME1,TIME2Ϊϵͳʱ�ӽ��ļ�����,���ֵΪ0xFFFFFFFF)
#define TIME_DIFF_TICK(TIME1,TIME2) ( TIME1 > TIME2 ? 0xFFFFFFFF - TIME1 + TIME2 : TIME2 - TIME1 )

// ��������ʱ�ӽ���֮��������(TIME1,TIME2Ϊϵͳʱ�ӽ��ļ�����,���ֵΪ0xFFFFFFFF)
#define TIME_DIFF_MS(TIME1,TIME2) ( (TIME1 > TIME2 ? 0xFFFFFFFF - TIME1 + TIME2 : TIME2 - TIME1) * (1000 / OS_TICKS_PER_SEC))

// λ�����궨��
#define BitGet(Number,Pos) ((Number >> Pos) & 1)   //�õ�ĳ����ĳλ
#define BitSet(Number,Pos) (Number |= (1<<(Pos)))  //��ĳλ��1
#define BitClr(Number,Pos) (Number &= ~(1<<(Pos))) //��ĳλ��0
#define BitRes(Number,Pos) (Number ^= (1<<(Pos)))  //��Number��POSλȡ��

#define MinTwo(a,b) ((a < b) ? a : b) //��ȡ�������е���Сֵ
#define MaxTwo(a,b) ((a > b) ? a : b) //��ȡ�������е����ֵ
#define MinThr(a,b,c) (MinTwo(MinTwo(a,b), c))  //��ȡ�������е���Сֵ
#define MaxThr(a,b,c) (MaxTwo(MaxTwo(a,b), c))  //��ȡ�������е���dd  ֵ

#define DSECOND 1 //��
#define DMINUTE 2 //��
#define DHOUR 3   //ʱ
#define DDAY 4    //��
#define DMONTH 5  //��
#define DYEAR 6   //��
#define DWEEK 7   //��

/*********************************************************************************************************
* ��    �ƣ�app_crc32
* ��    �ܣ�CRC32λУ��
* ��ڲ���:crc:У��ǰ��ֵ
						buf:���ݻ���
						len:��Ч���ݳ���
							
* ���ڲ���:������CRC��ֵ
**********************************************************************************************************/
uint32 app_crc32 (uint32 crc, const char *buf, uint32 len);

/*********************************************************************************************************
* ��    �ƣ�app_crc16
* ��    �ܣ�CRC16λУ��
* ��ڲ���:
						buf:���ݻ���
						len:��Ч���ݳ���
							
* ���ڲ���:������CRC��ֵ
**********************************************************************************************************/
uint16 app_crc16(uint8 *buf, int len);

/*********************************************************************************************************
* ��    �ƣ�app_crc16_r
* ��    �ܣ���λ CRC16λУ��
* ��ڲ���:
						buf:���ݻ���
						len:��Ч���ݳ���
							
* ���ڲ���:������CRC��ֵ
**********************************************************************************************************/
uint16 app_crc16_r(uint8 *buf, int len);

/*********************************************************************************************************
* ��    �ƣ�app_itoa
* ��    �ܣ�n���Ʊ��뵽asc�����ת��
* ��ڲ���: v:��ת����n������
						 s:ת�����asc��
						 r:v������r���Ƶģ�
						 h:Ԥ��������λ�����粻����ǰ�油asc�ַ�'0'
						 flag:���صĵ������ݵĳ���
* ���ڲ���:ָ�򷵻����ݵ���һ����ַ
**********************************************************************************************************/
int8 app_atoi
(
  void *val_arg_ptr,      /*  value returned  */
  char *s,                        /*  points to string to eval  */
  unsigned int r,                        /*  radix */
  uint8 flag
);

/*********************************************************************************************************
* ��    �ƣ�app_atoi
* ��    �ܣ�asc ��n���Ʊ����ת��
* ��ڲ���: val_arg_ptr:ָ�򷵻ش洢��������
							s:ָ����������ַ�����(���ո���������)
							r:����������r���Ƶ�
							
* ���ڲ���:�����õ������ݸ���
**********************************************************************************************************/
char *app_itoa(uint32 v,char *s,uint16 r,uint8 h);

/*********************************************************************************************************
* ��    �ƣ�app_checksum
* ��    �ܣ�����У���
* ��ڲ�����src:��Ϣ����ʼָ��
							len:��Ϣ�򳤶�
* ���ڲ���:����У�������
**********************************************************************************************************/
uint8 app_checksum(int8 *src, uint32 len);

/*********************************************************************************************************
* ��    �ƣ�app_hex_reverse
* ��    �ܣ���hex��Ϣ���з�����
* ��ڲ�����pdata:��Ϣ��ʼָ��
							len:��Ϣ�򳤶�
* ���ڲ���:0-�ɹ�  1-ʧ��
**********************************************************************************************************/
uint8 app_hex_reverse(uint8 *pdata, uint16 len);

/*********************************************************************************************************
* ��    �ƣ�app_timeconv
* ��    �ܣ����洢��ʱ���ʽת��Ϊ�����ʱ���С
* ��ڲ�����time:ָ��ʱ��洢ָ��
							
* ���ڲ���:���ؼ����ƫ��ʱ��
**********************************************************************************************************/
time_t app_timeconv( uint8 *time);

/*********************************************************************************************************
* ��    �ƣ�app_timereconv
* ��    �ܣ�������ʱ��Ĵ�Сת��Ϊ��׼ʱ���ʽ
* ��ڲ�����src:����ʱ��Ĵ�С
							
* ���ڲ���:time:���ر�׼ʱ���ʽ
**********************************************************************************************************/
int8 app_timereconv( uint8 *time, time_t sr);

/*********************************************************************************************************
* ��    �ƣ�app_timeconv
* ��    �ܣ����洢��ʱ���ʽת��Ϊ�����ʱ���С
* ��ڲ�����time:ָ��ʱ��洢ָ��BCD(ssmmHHddMMyy)

* ���ڲ���:���ؼ����ƫ��ʱ��
**********************************************************************************************************/
time_t app_bcdtimetotimet(uint8 *time);

/**********************************************************************************
 * �������ƣ�app_findbytes
 * �������ܣ���ѯĳ���ݴ����Ƿ���ָ�������ݴ�
 * �������: buf uint8  Դ���ݴ�������ָ���ַ
 * ���������len uint8  Դ���ݴ�����������
 * ���������needle uint8 Ŀ�����ݴ�������ָ���ַ
 * �������:  pos��Ӧ���ַ�������ʼλ��
 * ��  ��   ֵ��1�ҵ���Ӧ���ַ���??,0:δ�ҵ�ָ�����ݴ�
 **********************************************************************************/
uint8 app_findbytes(uint8 *buf, uint32 len, uint8 *needle, uint16 *pos );

//bcd ���뵽asc���ת��
//len Ϊָ��ת����ĳ���
int8 app_bcdtoa(char *asc, char *bcd, int len, int fmt);

//asc ii �� BCD �����ת��
int app_atobcd(char *bcd, char *asc, int len, int fmt);

/*******************************************************
 * ��������:��ȡ�绰������ֻ�����
 * ��������:app_getphone(uint8 *phone,uint8 *inbuf,uint8 len)
 * �������:pstr ĩ����ĺ�������
 * �������:len ĩ����ĺ������ݳ���
 * �������:pdtr �����ĺ���
 * ��   ��  ֵ:1 �ɹ�,0:ʧ��
 *******************************************************/
int8 app_getphone(uint8 *pdtr,uint8 *pstr,uint8 len);

/*******************************************************
 * ��������:�ж������Ƿ���Ч
 * ��������:app_isDate
 * �������:�꣬�£���
 * ��   ��  ֵ:1��Ч,0��Ч
 *******************************************************/
int8 app_isdate(int16 y, int16 m, int16 d);

/*******************************************************
 * ��������:����time_t�����������ʱ��
 * ��������:app_dateadd
 * �������:<u8Interval>[uint8] ��λ
 * �������:<s32Nt>[int32] �����
 * �������:<tTime>[time_t] ����ʱ��
 * ��   ��  ֵ:time_t
 *******************************************************/
time_t app_dateadd(uint8 u8Interval, int32 s32Nt, time_t tTime);

/*******************************************************
 * �������ܣ��ɴ�ӡ�ַ���ת��Ϊ�ֽ�����(�硰C8329B�� --> {0xC8,0x32,0x98})
 * �������ƣ�StringToBytes(uint8 * pDst,const uint8 * pSrc,int32 nSrcLength)
 * ���������pDst uint8 Ŀ���ַ���ָ��
 * ���������pSrc uint8 Դ�ַ���ָ��
 * ���������nSrcLength int32 Դ�ַ�������
 * �� �� ֵ��>0 Ŀ���ַ�������,-1 ת������
 ******************************************************/
int32 StringToBytes(uint8 * pDst,const uint8 * pSrc,int32 nSrcLength);

/*******************************************************
 * �������ܣ��ֽ�����ת��Ϊ�ɴ�ӡ�ַ���(�� {0xC8,0x32,0x98} --> "C8329B")
 * �������ƣ�int32 BytesToString(uint8 * pDst , const uint8 * pSrc, int32 nSrcLength)
 * ���������pDst uint8 Ŀ���ַ���ָ��
 * ���������pSrc uint8 Դ�ַ���ָ��
 * ���������nSrcLength int32 Դ�ַ�������
 * �� �� ֵ��Ŀ���ַ�������,-1 ת������
 ******************************************************/
int32 BytesToString(uint8 * pDst,const uint8 * pSrc,int32 nSrcLength);

/*******************************************************
 * �������ܣ������ߵ����ַ���ת��Ϊ����˳����ַ���
 * �������ƣ�int32 SerializeNumber(uint8 * pDst,const uint8 * pSrc,const int8 nSMSFlag,int32 nSrcLength)
 * ���������pDst uint8 Ŀ���ַ���ָ��
 * ���������pSrc uint8 Դ�ַ���ָ��
 * ���������nSMSFlag int8 ����Ϣ����ת����־(683195595682F1->8613599565281)
 * ���������nSrcLength int32 Դ�ַ�������
 * ��  ��  ֵ��Ŀ���ַ�������
 ******************************************************/
int32 SerializeNumber(uint8 * pDst,const uint8 * pSrc,const int8 nSMSFlag,int32 nSrcLength);

/*******************************************************
 * �������ܣ�����˳����ַ���ת��Ϊ�����ߵ����ַ���(����Ϊ��������'F'�ճ�ż��)
 * �������ƣ�int32 InvertNumbers(uint8 * pDst , const uint8 * pSrc, const int8 nSMSFlag, int32 nSrcLength)
 * ���������pDst uint8 Ŀ���ַ���ָ��
 * ���������pSrc uint8 Դ�ַ���ָ��
 * ���������nSMSFlag int8 ����Ϣ����ת����־(8613599565281->683195595682F1)
 * ���������nSrcLength int32 Դ�ַ�������
 * �� �� ֵ��Ŀ���ַ�������
 ******************************************************/
int32 InvertNumbers(uint8 * pDst , const uint8 * pSrc, const int8 nSMSFlag, int32 nSrcLength);

/*******************************************************
 * ��������:������
 * ��������:PowerCal
 * �������:<s32Origianl>[int] ���˷���
 * �������:<s32Power>[int] ���������
 * ��   ��  ֵ:��������
 *******************************************************/
int32 PowerCal(int s32Origianl, int s32Power);

/*******************************************************
 * ��������:��ȡ�ֽ�����Чλ��
 * ��������:ValidBitCount
 * �������:<u8Byte>[uint8] ��������ֽ�
 * �������:��
 * ��   ��  ֵ:��Чλ����
 *******************************************************/
uint8 ValidBitCount(uint8 u8Byte);

/*******************************************************
 * ��������:BCD ��ת���� HEX ��
 * ��������:BCD2HEX
 * �������:<pData>[uint8 *] ��ת��������
 * �������:<u16Len>[uint16] ��ת�������ݳ���
 * ��   ��  ֵ:��Чλ����
 *******************************************************/
uint8 * BCD2HEX(uint8 * pData, const uint16 u16Len);

/*******************************************************
 * ��������:HEX ��ת���� BCD ��
 * ��������:HEX2BCD
 * �������:<pData>[uint8 *] ��ת��������
 * �������:<u16Len>[uint16] ��ת�������ݳ���
 * ��   ��  ֵ:��Чλ����
 *******************************************************/
uint8 * HEX2BCD(uint8 *pData, const uint16 u16Len);

 /*******************************************************
 * ��������:ʱ���ʽת��
 * ��������:TimeFormatStr
 * �������:<TimeFlag>[uint8 *] ���ʱ���ʽBCD
 * �������:<FormatTime>[time_t]  ��ת����ʱ��
 * �������:<ReverSe>[uint8]  �Ƿ����ߵ�
 * ��   ��  ֵ:��
 *******************************************************/
int8 TimeFormatStr(uint8 *TimeFlag, time_t FormatTime, uint8 Reverse);

/*******************************************************
* ��������:��RTC��ȡʱ�佫תΪBCD��ʽ
* ��������:RtctimeToBcd
* �������:<pTime>[uint8 *] ���ʱ���ʽBCD
* �������:<ReverSe>[uint8]  �Ƿ����ߵ�ssmmhhddMMyy
* ��   ��  ֵ:��
*******************************************************/
int8 RtctimeToBcd(uint8 Reverse, uint8 *pTime);

/*******************************************************
* ��������:��ȡ��ǰϵͳʱ��time_t��ʽ
* ��������:GetTime
* ��   ��  ֵ:��ǰϵͳʱ��time_t
*******************************************************/
time_t GetTime(void);

/*******************************************************
 * ��������:��ȡ��Ϣ���Ӧ����Ϣ�����Ա�������
 * ��������:app_getdtprono
 * �������:<u16Dt>[uint16] ��Ϣ���־
 * ��   ��  ֵ:��Ϣ�����Ա�������
 *******************************************************/
uint16 app_getdtprono(uint16 u16Dt);

/*******************************************************
 * ��������:�����ܶ�����תΪ��
 * ��������:app_densitytosec
 * �������:<u8Density>[uint8] �����ܶ�
 * ��   ��  ֵ:�����ܶȶ�Ӧ������
 *******************************************************/
uint16 app_densitytosec(uint8 u8Density);

/*******************************************************
 * ��������:�����ܶ�����תΪ����
 * ��������:app_densitytomin
 * �������:<u8Density>[uint8] �����ܶ�
 * ��   ��  ֵ:�����ܶȶ�Ӧ������
 *******************************************************/
uint16 app_densitytomin(uint8 u8Density);

/*******************************************************
 * ��������:�����ܶ�ת��
 * ��������:app_dinsitytomd
 * ��   ��  ֵ:Э�鶳���ܶ�
 *******************************************************/
uint8 app_dinsitytomd(uint8 u8Density);

//��һ���ڴ滺���в���ָ���ַ�����λ�ã���ͷ��ʼ���ң������ִ�Сд��
//���ص�һ���ҵ���λ�á�
//str1 - �ڴ滺���ͷָ��
//nLen1 - �ڴ滺�峤��
//str2 - Ҫ����ƥ����ַ���
uint8 * memstr( uint8 * str1, uint32 nLen1,  uint8 * str2);

/*******************************************************
 * ��������:ʱ���ʽ����(����λʱ����������0)
 * ��������:app_datetrim
 * �������:<interUnit>[uint8] ��λ(1��, 2��, 3ʱ, 4��, 5��)
 * �������:<date1>[time_t]  ��������ʱ��
 * ��   ��  ֵ:�������ʱ��
 *******************************************************/
time_t app_datetrim(uint8 interUnit,const time_t date);

/*******************************************************
 * ��������:��������ʱ�䵥λ��ֵ
 * ��������:app_bcd5datediff
 * �������:<interUnit>[uint8] ��λ(1��, 2��, 3ʱ, 4��, 5��)
 * �������:<date1>[time_t]  ��ʼʱ��
 * �������:<date2>[time_t]  ����ʱ��
 * ��   ��  ֵ:��λ��ֵ
 *******************************************************/
int32 app_datediff(uint8 interUnit, const time_t date1,const time_t date2);

/*******************************************************
 * ��������:��������ʱ�䵥λ��ֵ
 * ��������:app_bcd5datediff
 * �������:<interUnit>[uint8] ��λ(2��, 3ʱ, 4��, 5��)
 * �������:<bcdTime1>[uint8 *]  ��ʼʱ��
 * �������:<bcdTime2>[uint8 *]  ����ʱ��
 * ��   ��  ֵ:��λ��ֵ
 *******************************************************/
int32 app_bcd5datediff(uint8 interUnit, const uint8 *bcdTime1, const uint8 *bcdTime2);

/*******************************************************
 * ��������:ʱ����ǰ��Ծ
 * ��������:app_upboundtime
 * �������:<tEndTime>[time_t] ����ʱ��
 * �������:<tStdTime>[time_t] ��ʼʱ��
 * �������:<interUnit>[uint8] ��λ(2��, 3ʱ, 4��, 5��)
 * �������:<interVal>[uint32] ����
 * ��   ��  ֵ:��Ծ���ʱ��
 *******************************************************/
time_t app_upboundtime(time_t tEndTime, time_t tStdTime, uint8 interUnit, uint32 interVal);

/*******************************************************
 * ��������:ʱ�������Ծ
 * ��������:app_downboundtime
 * �������:<tEndTime>[time_t] ����ʱ��
 * �������:<tStdTime>[time_t] ��ʼʱ��
 * �������:<interUnit>[uint8] ��λ(2��, 3ʱ, 4��, 5��)
 * �������:<interVal>[uint32] ����
 * ��   ��  ֵ:��Ծ���ʱ��
 *******************************************************/
time_t app_downboundtime(time_t tEndTime, time_t tStdTime, uint8 interUnit, int32 interVal);

/*******************************************************
 * ��������:�������㲢�������ж�
 * ��������:app_countelectricity
 * �������:<fpStart>[fp64] ����
 * �������:<fpEnd>[fp64]   ֹ��
 * �������:<u8IntegerNum>[uint8]  ����ʾֵ����λ����
 * �������:<pEnergy>[fp64 *]  ������
 * ��   ��  ֵ:-2 ʾ���쳣, -3 ʾ���½�, 0 ����
 *******************************************************/
int8 app_countelectricity(fp64 fpStart, fp64 fpEnd, uint8 u8IntegerNum, fp64 *pEnergy);

/*******************************************************
 * ��������:�󸡵�������ֵ
 * ��������:app_fabs
 * �������:<fpVal>[fp64] ������
 * ��   ��  ֵ:����������ֵ
 *******************************************************/
fp64 app_fabs(fp64 fpVal);

/*******************************************************
 * ��������:��ȡ�ֽ���ָ��λ��ǰ��Чλ��
 * ��������:ValidBitCount
 * �������:<u8Byte>[uint8] ��������ֽ�
 * �������:<u8End>[uint8] ָ��λ
 * �������:��
 * ��   ��  ֵ:��Чλ����
 *******************************************************/
uint8 BefValidBitCount(uint8 u8Byte, uint8 u8End);


//��������ת��Ϊ10����
//�������:
// buf_src   ���뻺����
// src_digit ת�����Ľ���
// ����ֵ:
//      ת�����10������
long int app_otdec(unsigned char *buf_src, int src_digit);

/*******************************************************
 * ��������:ʱ���ʽתΪTime_t����
 * ��������:app_mktime
 * �������:<pStTm>[struct tm *] ʱ�Ӹ�ʽ
 * ��   ��  ֵ:ת�����ʱ��
 *******************************************************/
time_t app_mktime(struct tm *pStTm);

/*******************************************************
 * ��������:ʱ���ʽתΪTime_tתΪtm
 * ��������:app_mktime
 * �������:<pStTm>[struct tm *] ʱ�Ӹ�ʽ
 * ��   ��  ֵ:ת�����ʱ��
 *******************************************************/
int8 app_localtime(struct tm *ptm, time_t tTime);

/*******************************************************
 * ��������:���ú������ʼ��
 * ��������:app_init
 * ��   ��  ֵ:��
 *******************************************************/
void app_init(void *param);

#endif /*_APP_UTIL_H_*/
