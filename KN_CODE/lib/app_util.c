/****************************************Copyright (c)****************************************************
 **                         							ZhangZhou KENENG electronics Co.,LTD.
 **
 **
 **
 **--------------File Info---------------------------------------------------------------------------------
 ** File name:			app_util.c
 ** Last modified Date:  2009-05-15
 ** Last Version:		1.0
 ** Descriptions:		提供给APP层进行调用的一些公共函数接口
 **
 **--------------------------------------------------------------------------------------------------------
 ** Created by:		heshusheng
 ** Created date:		2009-05-15
 ** Version:				1.0
 ** Descriptions:		The original version
 **
 **--------------------------------------------------------------------------------------------------------
 2009-05-27	by heshusheng		  add function app_checksum.
 2009-06-03	by heshusheng		  add function app_hex_reverse.
 2009-07-07	by heshusheng		  add function app_crc32.
 2009-07-08    by heshusheng		  add function app_findbytes.
 2008-09-05    by heshusheng		  add function app_crc16.
 **
 **  2010-01-20  by lindw  add app_getphone.
 **  2010-01-20  by lindw  add StringToBytes.
 **  2010-01-20  by lindw  add SerializeNumber.
 **  2010-01-20  by lindw  add BytesToString.
 **  2010-01-20  by lindw  add BytesToString.
 *********************************************************************************************************/
#include <math.h>
#include "config.h"
#include "app_util.h"
#include ".\kn_code\driver\dog\dog.h"
#include "debug.h"

#define MAX_VAL_NUM_ITEM 0xFFFFFFFF  

/* ========================================================================
 * Table of CRC-32's of all single-byte values (made by make_crc_table)
 */
static const uint32 crc_table[256] =
{0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L, 0x706af48fL,
		0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL,
		0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
		0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL, 0x1adad47dL,
		0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L, 0x646ba8c0L,
		0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L,
		0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
		0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL, 0x35b5a8faL,
		0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L, 0x45df5c75L,
		0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL, 0xc8d75180L,
		0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
		0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L, 0x2f6f7c87L,
		0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L, 0x01db7106L,
		0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L,
		0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
		0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L, 0x6b6b51f4L,
		0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL, 0x1b01a57bL,
		0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL,
		0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
		0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L, 0x4adfa541L,
		0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL, 0x346ed9fcL,
		0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L, 0xaa0a4c5fL,
		0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
		0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL, 0x5edef90eL,
		0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L, 0x2eb40d81L,
		0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL,
		0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
		0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L, 0xe40ecf0bL,
		0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L, 0x8708a3d2L,
		0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL, 0x196c3671L,
		0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
		0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L, 0xd6d6a3e8L,
		0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L, 0xa6bc5767L,
		0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L,
		0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
		0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L, 0xcc0c7795L,
		0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL, 0xb2bd0b28L,
		0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL,
		0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
		0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L, 0x95bf4a82L,
		0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL, 0xe5d5be0dL,
		0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L,
		0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
		0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL, 0x8f659effL,
		0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L, 0xd70dd2eeL,
		0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L, 0x4969474dL,
		0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
		0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L, 0xbdbdf21cL,
		0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L, 0xcdd70693L,
		0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L,
		0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL };

/* Table of CRC constants - implements x^16+x^12+x^5+1 */
static const uint16 crc16_tab[] =
{ 0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108,
		0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 0x1231, 0x0210,
		0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 0x9339, 0x8318, 0xb37b,
		0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 0x2462, 0x3443, 0x0420, 0x1401,
		0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee,
		0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6,
		0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d,
		0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
		0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 0x5af5,
		0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12, 0xdbfd, 0xcbdc,
		0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a, 0x6ca6, 0x7c87, 0x4ce4,
		0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 0xedae, 0xfd8f, 0xcdec, 0xddcd,
		0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13,
		0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a,
		0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e,
		0xe16f, 0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
		0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x02b1,
		0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256, 0xb5ea, 0xa5cb,
		0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 0x34e2, 0x24c3, 0x14a0,
		0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xa7db, 0xb7fa, 0x8799, 0x97b8,
		0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657,
		0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9,
		0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882,
		0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
		0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 0xfd2e,
		0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07,
		0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1, 0xef1f, 0xff3e, 0xcf5d,
		0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55, 0x5e74,
		0x2e93, 0x3eb2, 0x0ed1, 0x1ef0, };

/* ========================================================================= */
#define DO1(buf) crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);

#define  UPCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c))


#define UTIL_LOCK(nblock)  do {                                        \
                                uint8   ucErr;                          \
                                OSSemPend(nblock, 0, &ucErr);           \
                                if (ucErr != OS_NO_ERR) {               \
                                    return  (0);             \
                                }                                       \
                            } while (0)

#define UTIL_UNLOCK(nblock) do {                                       \
                                OSSemPost(nblock);                      \
                             } while(0)

static OS_EVENT *_util_sem1;   // 数据访问信号量
static OS_EVENT *_util_sem2;   // 数据访问信号量
static OS_EVENT *_util_sem3;   // 数据访问信号量
static OS_EVENT *_util_sem4;   // 数据访问信号量
static OS_EVENT *_util_sem5;   // 数据访问信号量

/*********************************************************************************************************
 * 名    称：app_crc32
 * 功    能：CRC32位校验
 * 入口参数:crc:校验前的值
 buf:数据缓存
 len:有效数据长度

 * 出口参数:计算后的CRC的值
 **********************************************************************************************************/
uint32 app_crc32(uint32 crc, const char *buf, uint32 len)
{
	crc = crc ^ 0xffffffffL;
	while (len >= 8)
	{
		DO8(buf);
		len -= 8;
	}
	if (len)
		do
		{
			DO1(buf);
		} while (--len);
	return crc ^ 0xffffffffL;
}

/*********************************************************************************************************
 * 名    称：app_crc16
 * 功    能：CRC16位校验
 * 入口参数:
 buf:数据缓存
 len:有效数据长度

 * 出口参数:计算后的CRC的值
 **********************************************************************************************************/
uint16 app_crc16(uint8 *buf, int len)
{
	int i;
	uint16 cksum;

	cksum = 0;
	for (i = 0; i < len; i++)
	{
		cksum = crc16_tab[((cksum >> 8) ^ *buf++) & 0xFF] ^ (cksum << 8);
	}
	return cksum;
}

/*********************************************************************************************************
 * 名    称：app_crc16_r
 * 功    能：高位 CRC16位校验
 * 入口参数:
 buf:数据缓存
 len:有效数据长度

 * 出口参数:计算后的CRC的值
 **********************************************************************************************************/
uint16 app_crc16_r(uint8 *buf, int len)
{
	uint16 crc, temp = 0;
	int i;
	uint8 low = 0;
	uint8 high = 0;

	crc = 0xffff;
	while (len--)
	{
		low = (uint8) (crc & 0xff);
		high = (uint8) ((crc >> 8) & 0xff);
		crc = (*buf ^ high) << 8 | low;

		for (i = 0; i < 8; i++)
		{
			temp = crc;
			crc = (crc >> 1);
			if (temp & 0x01)
			{
				crc ^= 0xa001;
			}
		}
		buf++;
	}
	return crc;
}

/*********************************************************************************************************
 * 名    称：app_itoa
 * 功    能：n进制编码到asc编码的转换
 * 入口参数: v:待转换的n进制数
 s:转换后的asc码
 r:v数据是r进制的；
 h:预留的数据位数，如不够则前面补asc字符'0'

 * 出口参数:指向返回数据的下一个地址
 **********************************************************************************************************/
char *app_itoa(uint32 v, char *s, uint16 r, uint8 h)
{
	char buf[33], c;
	int n, x;

	//n = 0;
	x = 0;
	n = sizeof(buf) - 1;
	buf[n] = '\0';
	do
	{
		c = (char) (v % r);

		//if (n >= 32)
		if (n <= 0)
		{
			DEBUG("OVERFLOW\n\r");
			break;
		}

		//buf[n++] = (char) ( (c > 9) ? c + 'A' - 10 : c + '0');
		buf[--n] = (char) ((c > 9) ? c + 'A' - 10 : c + '0');
		x++;
		//} while ( ((v /= r) > 0) || (h > n));
	} while (((v /= r) > 0) || (h > x));
	//buf[n] = '\0';
	//n = 0;

	while ((*s++ = buf[n++]) != 0)
		;

	return (s - 1);
} /* app_itoa */

/*********************************************************************************************************
 * 名    称：app_atoi
 * 功    能：asc 到n进制编码的转换
 * 入口参数: val_arg_ptr:指向返回存储的数据域
 s:指向待处理的字符串，(遇空格区分数据)
 r:返回数据是r进制的
 flag:返回的单个数据的长度。
 * 出口参数:解析得到的数据个数
 **********************************************************************************************************/
int8 app_atoi(void *val_arg_ptr, /*  value returned  */
char *s, /*  points to string to eval  */
unsigned int r, /*  radix */
uint8 flag /*value length returned*/
)
{
	uint8 *u8_ptr;
	uint32 *u32_ptr;
	void *pstart;
	char c;
	uint32 val;

	if (flag == 0)
		u8_ptr = (uint8 *) val_arg_ptr;
	else
		u32_ptr = (uint32 *) val_arg_ptr;

	pstart = val_arg_ptr;
	val = 0;

	while ((c = *s++) != '\0')
	{
		if (c != ' ')
		{
			c = (char) UPCASE (c);
			if (c >= '0' && c <= '9')
			{
				c -= '0';
			}
			else if (c >= 'A')
			{
				c -= 'A' - 10;
			}
			else
			{
				break;
			}

			val = (uint32) (val * r + c);

		}
		else //test.
		{
			if (flag == 0)
			{
				*(u8_ptr)++ = val;
			}
			else
			{
				*(u32_ptr)++ = val;
			}
			val = 0;
		}

	}

	if (flag == 0)
	{
		*(u8_ptr)++ = (uint8) val;
	}
	else
	{
		*(u32_ptr)++ = (uint32) val;
	}

	if (flag == 0)
	{
		return ((uint8 *) u8_ptr - (uint8 *) pstart);
	}
	else
	{
		return ((uint8 *) u32_ptr - (uint8 *) pstart);
	}
} /*  dsatutil_atoi */

//asc ii 到 BCD 编码的转换
int app_atobcd(char *bcd, char *asc, int len, int fmt)
{
	int i, odd;
	char c;

	odd = len & 0x01;

	if (odd && !fmt)
		*bcd++ = (*asc++) & '\x0F';

	len >>= 1;

	for (i = 0; i < len; i++)
	{
		c = (*asc++) << 4;
		c |= (*asc++) & '\x0F';

		*bcd++ = c;
	}

	if (odd && fmt)
		*bcd = (*asc) << 4;

	return (i + odd);
}

//bcd 编码到asc码的转换
//len 为指定转换后的长度
int8 app_bcdtoa(char *asc, char *bcd, int len, int fmt)
{

	char *ptemp;
	int i, rlen;
	unsigned char c;

	ptemp = asc;

	rlen = len;
	len >>= 1;

	//先转换
	for (i = 0; i < len; i++)
	{
		c = *bcd++;
		*ptemp++ = (c >> 4) + '0';
		*ptemp++ = (c & '\x0F') + '0';
	}

	if (fmt == 1)
	{
		ptemp = asc;
		for (i = 0; i < rlen; i++)
		{
			if (*ptemp != '0')
			{
				memcpy(asc, ptemp, (rlen - i));
				break;
			}
			ptemp++;
		}
	}

	return (rlen - i);

}

/*********************************************************************************************************
 * 名    称：app_checksum
 * 功    能：计算校验和
 * 入口参数：src:信息域起始指针
 len:信息域长度
 * 出口参数:返回校验和数据
 **********************************************************************************************************/
uint8 app_checksum(int8 *src, uint32 len)
{
	uint8 sum;

	for (sum = 0; len != 0; len--)
	{
		sum += *(src++);
	}

	return sum;
}

/*********************************************************************************************************
 * 名    称：app_hex_reverse
 * 功    能：将hex信息进行反序处理
 * 入口参数：pdata:信息起始指针
 len:信息域长度
 * 出口参数:0-成功  1-失败
 **********************************************************************************************************/
uint8 app_hex_reverse(uint8 *pdata, uint16 len)
{
	int i, t, value;

	UTIL_LOCK(_util_sem1);
	value = (len / 2);
	len--;

	for (i = 0; i < value; i++)
	{
		t = pdata[i];
		pdata[i] = pdata[len - i];
		pdata[len - i] = t;
	}

	UTIL_UNLOCK(_util_sem1);
	return 0;
}

/*********************************************************************************************************
 * 名    称：app_timeconv
 * 功    能：将存储的时间格式转化为计算的时间大小
 * 入口参数：time:指向时间存储指针

 * 出口参数:返回计算的偏移时间
 **********************************************************************************************************/
time_t app_timeconv(uint8 *time)
{
	time_t t_time;
	struct tm temp;

	UTIL_LOCK(_util_sem1);
	memset(&temp, 0x00, sizeof(struct tm));
	temp.tm_year = (100 + time[0]);
	temp.tm_mon = (time[1] - 1);
	temp.tm_mday = time[2];
	temp.tm_hour = time[3];
	temp.tm_min = time[4];
	temp.tm_sec = time[5];
	t_time = app_mktime(&temp);
	UTIL_UNLOCK(_util_sem1);

	return t_time;
}

/*********************************************************************************************************
 * 名    称：app_timeconv
 * 功    能：将存储的时间格式转化为计算的时间大小
 * 入口参数：time:指向时间存储指针BCD(ssmmHHddMMyy)

 * 出口参数:返回计算的偏移时间
 **********************************************************************************************************/
time_t app_bcdtimetotimet(uint8 *time)
{
	uint8 _u8Tmp[6]; // 临时变量
	time_t t_time;
	struct tm temp;

	UTIL_LOCK(_util_sem1);
	memcpy(_u8Tmp, time, 6);
	BCD2HEX(_u8Tmp, 6);
	memset(&temp, 0x00, sizeof(struct tm));
	temp.tm_year = (100 + _u8Tmp[5]);
	temp.tm_mon = (_u8Tmp[4] - 1);
	temp.tm_mday = _u8Tmp[3];
	temp.tm_hour = _u8Tmp[2];
	temp.tm_min = _u8Tmp[1];
	temp.tm_sec = _u8Tmp[0];
	t_time = app_mktime(&temp);
	UTIL_UNLOCK(_util_sem1);

	return t_time;
}

/*******************************************************
 * 函数功能:时间格式转为Time_t类型
 * 函数名称:app_mktime
 * 输入参数:<pStTm>[struct tm *] 时钟格式
 * 返   回  值:转换后的时间
 *******************************************************/
time_t app_mktime(struct tm *pStTm)
{
	time_t t_time;

	UTIL_LOCK(_util_sem2);
	t_time = mktime(pStTm);
	UTIL_UNLOCK(_util_sem2);

	return t_time;
}

/*******************************************************
 * 函数功能:时间格式转为Time_t转为tm
 * 函数名称:app_mktime
 * 输入参数:<pStTm>[struct tm *] 时钟格式
 * 返   回  值:转换后的时间
 *******************************************************/
int8 app_localtime(struct tm *ptm, time_t tTime)
{
	struct tm _stTm;

	UTIL_LOCK(_util_sem4);
	_stTm = *localtime(&tTime);
	memcpy(ptm, &_stTm, sizeof(struct tm));
	UTIL_UNLOCK(_util_sem4);

	return 0;
}

/*********************************************************************************************************
 * 名    称：app_timereconv
 * 功    能：将计算时间的大小转换为标准时间格式
 * 入口参数：src:计算时间的大小

 * 出口参数:time:返回标准时间格式
 **********************************************************************************************************/
int8 app_timereconv(uint8 *time, time_t sr)
{
	struct tm _stTm;

	UTIL_LOCK(_util_sem1);
	app_localtime(&_stTm, sr);
	time[0] = _stTm.tm_year - 100;
	time[1] = _stTm.tm_mon + 1;
	time[2] = _stTm.tm_mday;
	time[3] = _stTm.tm_hour;
	time[4] = _stTm.tm_min;
	time[5] = 0x00;
	UTIL_UNLOCK(_util_sem1);

	return 0;
}

/**********************************************************************************
 * 函数名称：app_findbytes
 * 函数功能：查询某数据串中是否有指定的数据串
 * 输入参数: buf uint8  源数据串缓冲区指针地址
 * 输入参数：len uint8  源数据串缓冲区长度
 * 输入参数：needle uint8 目标数据串缓冲区指针地址
 * 输出参数:  pos对应的字符串的起始位置
 * 返  回   值：1找到对应的字符串,0:未找到指定数据串
 **********************************************************************************/
uint8 app_findbytes(uint8 *buf, uint32 len, uint8 *needle, uint16 *pos)
{
	uint32 i;
	uint32 needleMatchedPos = 0;

	if (needle[0] == '\0')
	{
		return 1;
	}

	for (i = 0; i < len; i++)
	{
		if (needle[needleMatchedPos] == buf[i])
		{
			//记住起始位置
			if (needleMatchedPos == 0)
			{
				*pos = i;
			}

			needleMatchedPos++;

			if (needle[needleMatchedPos] == '\0')
			{
				UTIL_UNLOCK(_util_sem1);
				return 1;
			}
		}
		else
		{
			needleMatchedPos = 0;
		}
	}

	return 0;
}


/*******************************************************
 * 函数功能:获取电话号码或手机号码
 * 函数名称:app_getphone(uint8 *phone,uint8 *inbuf,uint8 len)
 * 输入参数:pstr 末解码的号码数据
 * 输入参数:len 末解码的号码数据长度
 * 输出参数:pdtr 解码后的号码
 * 返   回  值:1 成功,0:失败
 *******************************************************/
int8 app_getphone(uint8 *pdtr, uint8 *pstr, uint8 len)
{
	int32 i = 0;
	int32 j = 0;

	for (i = 0; i < len; i++)
	{
		switch (*pstr & 0xF0)
		{
			case 0xB0:
			{
				pdtr[i * 2] = ',';
				break;
			}
			case 0xA0:
			{
				pdtr[i * 2] = '#';
				break;
			}
			default:
			{
				j = (*pstr & 0xF0) >> 4;

				if (j >= 0 && j <= 9)
				{
					pdtr[i * 2] = j + 0x30;
				}

				break;
			}
		}

		switch (*pstr & 0x0F)
		{
			case 0x0B:
			{
				pdtr[i * 2 + 1] = ',';
				break;
			}
			case 0x0A:
			{
				pdtr[i * 2 + 1] = '#';
				break;
			}
			default:
			{
				j = (*pstr & 0x0F);

				if (j >= 0 && j <= 9)
				{
					pdtr[i * 2 + 1] = j + 0x30;
				}

				break;
			}
		}

		pstr++;
	}

	return 1;
}

/*******************************************************
 * 函数功能:闰年判断
 * 函数名称:app_isleap
 * 输入参数:年
 * 返   回  值:1是， 0不是
 *******************************************************/
int8 app_isleap(int16 y)
{
	return (((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) ? 1 : 0);
}

/*******************************************************
 * 函数功能:判断日期是否有效
 * 函数名称:app_isDate
 * 输入参数:年，月，日
 * 返   回  值:1有效,0无效
 *******************************************************/
int8 app_isdate(int16 y, int16 m, int16 d)
{
	return (!((y < 1 || m < 1 || m > 12 || d < 1 || d > 31) || ((m == 4 || m
			== 6 || m == 9 || m == 11) && d > 30) || (m == 2 && app_isleap(y)
			&& d > 29) || (m == 2 && !app_isleap(y) && d > 28)) ? 1 : 0);
}

/*******************************************************
 * 函数功能:计算time_t类型相对日期时间(单位为月、年时月份取每月最后一天)
 * 函数名称:app_dateadd
 * 输入参数:<u8Interval>[uint8] 单位
 * 输入参数:<s32Nt>[int32] 相对量
 * 输入参数:<tTime>[time_t] 日期时间
 * 返   回  值:time_t
 *******************************************************/
time_t app_dateadd(uint8 u8Interval, int32 s32Nt, time_t tTime)
{
	time_t t_time;
	struct tm pt;

	UTIL_LOCK(_util_sem1);
	app_localtime(&pt, tTime);

	switch (u8Interval)
	{
		case DYEAR://年
		{
			pt.tm_year += s32Nt;

			//这两个while是移动到当月最大的1天
			while (!app_isdate(pt.tm_year + 1900, pt.tm_mon + 1, pt.tm_mday)
					&& pt.tm_mday > 27)
			{
				pt.tm_mday--;
			}

			while (app_isdate(pt.tm_year + 1900, pt.tm_mon + 1, pt.tm_mday)
					&& pt.tm_mday < 32)
			{
				pt.tm_mday++;
			}

			pt.tm_mday--;

			break;
		}

		case DMONTH://月
		{
			pt.tm_mon += s32Nt;

			while (pt.tm_mon < 0)
			{
				pt.tm_year--;
				pt.tm_mon += 12;
			}

			while (pt.tm_mon >= 12)
			{
				pt.tm_year++;
				pt.tm_mon -= 12;
			}

			//这两个while是移动到当月最大的1天
			while (!app_isdate(pt.tm_year + 1900, pt.tm_mon + 1, pt.tm_mday)
					&& pt.tm_mday > 27)
			{
				pt.tm_mday--;
			}

			while (app_isdate(pt.tm_year + 1900, pt.tm_mon + 1, pt.tm_mday)
					&& pt.tm_mday < 32)
			{
				pt.tm_mday++;
			}

			pt.tm_mday--;

			break;
		}

		case DWEEK://周
		{
			pt.tm_wday += s32Nt;
			break;
		}

		case DDAY://日
		{
			pt.tm_mday += s32Nt;
			break;
		}

		case DHOUR://时
		{
			pt.tm_hour += s32Nt;
			break;
		}

		case DMINUTE://分
		{
			pt.tm_min += s32Nt;
			break;
		}

		default: //秒
		{
			pt.tm_sec += s32Nt;
			break;
		}
	}

	t_time = app_mktime(&pt);
	UTIL_UNLOCK(_util_sem1);

	return t_time;
}


/*******************************************************
 * 函数功能:计算time_t类型相对日期时间
 * 函数名称:app_dateadd
 * 输入参数:<u8Interval>[uint8] 单位
 * 输入参数:<s32Nt>[int32] 相对量
 * 输入参数:<tTime>[time_t] 日期时间
 * 返   回  值:time_t
 *******************************************************/
time_t app_dateadd_two(uint8 u8Interval, int32 s32Nt, time_t tTime)
{
	struct tm pt;
	time_t t_time;

	UTIL_LOCK(_util_sem1);
	app_localtime(&pt, tTime);

	switch (u8Interval)
	{
		case DYEAR://年
		{
			pt.tm_year += s32Nt;
			break;
		}

		case DMONTH://月
		{
			pt.tm_mon += s32Nt;
			break;
		}

		case DWEEK://周
		{
			pt.tm_wday += s32Nt;
			break;
		}

		case DDAY://日
		{
			pt.tm_mday += s32Nt;
			break;
		}

		case DHOUR://时
		{
			pt.tm_hour += s32Nt;
			break;
		}

		case DMINUTE://分
		{
			pt.tm_min += s32Nt;
			break;
		}

		default: //秒
		{
			pt.tm_sec += s32Nt;
			break;
		}
	}

	pt.tm_wday = 0;
	pt.tm_yday = 0;
	pt.tm_isdst = -1;
	t_time = mktime(&pt);
	UTIL_UNLOCK(_util_sem1);

	return t_time;
}


//其他进制转化为10进制
//输入参数:
// buf_src   输入缓冲区
// src_digit 转化数的进制
// 返回值:
//      转换后的10进制数
long int app_otdec(unsigned char *buf_src, int src_digit)
{
	int i = 0, j = 0, lc = 0;
	int num[32]; //存储由字符串转化的数字
	long int decimal = 0;

	memset(num, 0, sizeof(num));

	//字符转化为数字
	for (lc = 0; lc < strlen((char *) buf_src); lc++)
	{
		if (buf_src[lc] >= 'A' && buf_src[lc] <= 'F')//A~F的字母转化
		{
			num[lc] = buf_src[lc] - 'A' + 10;
		}
		else if (buf_src[lc] >= 'a' && buf_src[lc] <= 'f')
		{
			num[lc] = buf_src[lc] - 'a' + 10;
		}
		else
		{
			//数字的转化
			num[lc] = buf_src[lc] - '0';
		}
	}

	for (i = lc - 1; i >= 0; i--)
	{
		//      decimal += num[j]*pow(src_digit,i);
		decimal = decimal * src_digit + num[j];
		j++;
	}

	return decimal;
}

/*******************************************************
 * 函数功能：可打印字符串转换为字节数据(如“C8329B” --> {0xC8,0x32,0x98})
 * 函数名称：StringToBytes(uint8 * pDst,const uint8 * pSrc,int32 nSrcLength)
 * 输出参数：pDst uint8 目标字符串指针
 * 输入参数：pSrc uint8 源字符串指针
 * 输入参数：nSrcLength int32 源字符串长度
 * 返 回 值：>0 目标字符串长度,-1 转换错误
 ******************************************************/
int32 StringToBytes(uint8 * pDst, const uint8 * pSrc, int32 nSrcLength)
{
	int32 lp = 0; /* 循环标志 */

	for (lp = 0; lp < nSrcLength; lp += 2)
	{
		/* 输出高4位 */
		if (*pSrc >= '0' && *pSrc <= '9')
		{
			*pDst = (*pSrc - '0') << 4;
		}
		else if (*pSrc >= 'a' && *pSrc <= 'f')
		{
			*pDst = (*pSrc - 'a' + 10) << 4;
		}
		else
		{
			*pDst = (*pSrc - 'A' + 10) << 4;
		}

		pSrc++;

		/* 输出低4位 */
		if (*pSrc >= '0' && *pSrc <= '9')
		{
			*pDst |= *pSrc - '0';
		}
		else if (*pSrc >= 'a' && *pSrc <= 'f')
		{
			*pDst |= *pSrc - 'a' + 10;
		}
		else
		{
			*pDst |= *pSrc - 'A' + 10;
		}

		pSrc++;
		pDst++;
	}

	/* 返回目标数据长度 */
	return nSrcLength / 2;
}

/*******************************************************
 * 函数功能：字节数据转换为可打印字符串(如 {0xC8,0x32,0x98} --> "C8329B")
 * 函数名称：int32 BytesToString(uint8 * pDst , const uint8 * pSrc, int32 nSrcLength)
 * 输出参数：pDst uint8 目标字符串指针
 * 输入参数：pSrc uint8 源字符串指针
 * 输入参数：nSrcLength int32 源字符串长度
 * 返 回 值：目标字符串长度,-1 转换错误
 ******************************************************/
int32 BytesToString(uint8 * pDst, const uint8 * pSrc, int32 nSrcLength)
{
	int32 lp = 0; /* 循环标志 */
	const uint8 tabhex[] = "0123456789ABCDEF"; /* 0x0-0xF的字符查找表 */

	for (lp = 0; lp < nSrcLength; lp++)
	{
		/* 输出低4位 */
		*pDst++ = tabhex[*pSrc >> 4];

		/* 输出高4位 */
		*pDst++ = tabhex[*pSrc & 0x0F];

		pSrc++;
	}

	/* 输出字符串加个结束符 */
	*pDst = '\0';

	/* 返回目标数据长度 */
	return nSrcLength * 2;
}

/*******************************************************
 * 函数功能：两两颠倒的字符串转换为正常顺序的字符串
 * 函数名称：int32 SerializeNumber(uint8 * pDst,const uint8 * pSrc,const int8 nSMSFlag,int32 nSrcLength)
 * 输出参数：pDst uint8 目标字符串指针
 * 输入参数：pSrc uint8 源字符串指针
 * 输入参数：nSMSFlag int8 短消息号码转换标志(683195595682F1->8613599565281)
 * 输入参数：nSrcLength int32 源字符串长度
 * 返  回  值：目标字符串长度
 ******************************************************/
int32 SerializeNumber(uint8 * pDst, const uint8 * pSrc, const int8 nSMSFlag,
		int32 nSrcLength)
{
	int32 i = 0;
	int32 nDstLength = 0; /* 目标字符串长度 */
	uint8 ch; /* 用于保存一个字符 */

	/*  复制串长度 */
	nDstLength = nSrcLength;

	/* 两两颠倒 */
	for (i = 0; i < nSrcLength; i += 2)
	{
		ch = *pSrc++; /* 保存先出现的字符 */
		*pDst++ = *pSrc++; /* 复制后出现的字符 */
		*pDst++ = ch; /* 复制先出现的字符 */
	}

	/* 判断最后字符是'F' */
	if (nSMSFlag == 1 && *(pDst - 1) == 'F')
	{
		pDst--;
		nDstLength--; /* 目标串长度减1 */
	}

	/* 输出字符串加个结束符 */
	*pDst = '\0';

	/* 返回目标字符串长度 */
	return nDstLength;
}

/*******************************************************
 * 函数功能：正常顺序的字符串转换为两两颠倒的字符串(长度为奇数，补'F'凑成偶数)
 * 函数名称：int32 InvertNumbers(uint8 * pDst , const uint8 * pSrc, const int8 nSMSFlag, int32 nSrcLength)
 * 输出参数：pDst uint8 目标字符串指针
 * 输入参数：pSrc uint8 源字符串指针
 * 输入参数：nSMSFlag int8 短消息号码转换标志(8613599565281->683195595682F1)
 * 输入参数：nSrcLength int32 源字符串长度
 * 返 回 值：目标字符串长度
 ******************************************************/
int32 InvertNumbers(uint8 * pDst, const uint8 * pSrc, const int8 nSMSFlag,
		int32 nSrcLength)
{
	int32 i = 0;
	int32 nDstLength = 0; /* 目标字符串长度 */
	unsigned char ch; /* 用于保存一个字符 */

	/*  复制串长度 */
	nDstLength = nSrcLength;

	/* 两两颠倒 */
	for (i = 0; i < nSrcLength; i += 2)
	{
		ch = *pSrc++; /* 保存先出现的字符 */
		*pDst++ = *pSrc++; /* 复制后出现的字符 */
		*pDst++ = ch; /* 复制先出现的字符 */
	}

	/* 判断源串长度是否为奇数 */
	if (nSMSFlag == 1 && (nSrcLength & 1))
	{
		*(pDst - 2) = 'F'; /* 补'F' */
		nDstLength++; /* 目标串长度加1 */
	}

	/* 输出字符串加个结束符 */
	*pDst = '\0';

	/* 返回目标字符串长度 */
	return nDstLength;
}

/*******************************************************
 * 函数功能:幂运算
 * 函数名称:PowerCal
 * 输入参数:<s32Origianl>[int] 被乘方数
 * 输出参数:<s32Power>[int] 幂运算次数( >= 0)
 * 返   回  值:幂运算结果
 *******************************************************/
int32 PowerCal(int s32Origianl, int s32Power)
{
	int32 i, _s32RtnVal = 0;

	if (s32Power)
	{
		_s32RtnVal = s32Origianl;

		for (i = 1; i < s32Power; i++)
		{
			_s32RtnVal *= s32Origianl;
		}
	}
	else
	{
		_s32RtnVal = 1;
	}

	return _s32RtnVal;
}

/*******************************************************
 * 函数功能:获取字节中有效位数
 * 函数名称:ValidBitCount
 * 输入参数:<u8Byte>[uint8] 被计算的字节
 * 输出参数:无
 * 返   回  值:有效位个数
 *******************************************************/
uint8 ValidBitCount(uint8 u8Byte)
{
	uint8 _u8lb = 0;
	uint8 _u8Count = 0;

	for (_u8lb = 0; _u8lb < 8; _u8lb++)
	{
		if ((u8Byte & (1 << _u8lb)) > 0)
		{
			_u8Count++;
		}
	}

	return _u8Count;
}


/*******************************************************
 * 函数功能:BCD 码转换成 HEX 码
 * 函数名称:BCD2HEX
 * 输入参数:<pData>[uint8 *] 被转换的数据
 * 输入参数:<u16Len>[uint16] 被转换的数据长度
 * 返   回  值:有效位个数
 *******************************************************/
uint8 * BCD2HEX(uint8 * pData, const uint16 u16Len)
{
	uint16 _u16lc;

	//  检查全 FF
	for (_u16lc = 0; _u16lc < u16Len; _u16lc++)
	{
		if (pData[_u16lc] != 0xFF)
		{
			break;
		}
	}

	//  全 FF 不做转换
	if (_u16lc == u16Len)
	{
		return pData;
	}

	//  转换，不检查格式是否错误
	for (_u16lc = 0; _u16lc < u16Len; _u16lc++)
	{
		pData[_u16lc] = ((((0xFF & (pData[_u16lc])) >> 4) * 10) + ((0xFF
				& (pData[_u16lc])) & 0x0F));
	}

	return pData;
}

/*******************************************************
 * 函数功能:HEX 码转换成 BCD 码
 * 函数名称:HEX2BCD
 * 输入参数:<pData>[uint8 *] 被转换的数据
 * 输入参数:<u16Len>[uint16] 被转换的数据长度
 * 返   回  值:有效位个数
 *******************************************************/
uint8 *HEX2BCD(uint8 *pData, const uint16 u16Len)
{
	uint16 _u16lc;

	//  检查全 FF
	for (_u16lc = 0; _u16lc < u16Len; _u16lc++)
	{
		if (pData[_u16lc] != 0xFF)
		{
			break;
		}
	}

	//  全 FF 不做转换
	if (_u16lc == u16Len)
	{
		return pData;
	}

	//  转换，不检查格式是否错误
	for (_u16lc = 0; _u16lc < u16Len; _u16lc++)
	{
		pData[_u16lc] = (pData[_u16lc] / 10) * 16 + (pData[_u16lc] % 10);
	}

	return pData;
}

/*******************************************************
 * 函数功能:时间转字符串(yymmddHHmmss)
 * 函数名称:TimeToStr
 * 输出参数:<pData>[uint8 *]  输出时间格式字符串
 * 输入参数:<tTime>[time_t]   要格式化的时间
 * 返   回  值:无
 *******************************************************/
void TimeToStr(uint8 * pdata, time_t tTime)
{
	struct tm t;
	uint8 buf[20];

	memset(buf, 0, sizeof(buf));
	app_localtime(&t, tTime);
	strftime((char*) buf, 20, "%Y%m%d%H%M%S", &t);
	memcpy(pdata, &buf[2], 13);
}

/*******************************************************
 * 函数功能:时间格式转换
 * 函数名称:TimeFormatStr
 * 输出参数:<TimeFlag>[uint8 *] 输出时间格式BCD
 * 输入参数:<FormatTime>[time_t]  被转换的时间
 * 输入参数:<ReverSe>[uint8]  是否次序颠倒
 * 返   回  值:无
 *******************************************************/
int8 TimeFormatStr(uint8 *TimeFlag, time_t FormatTime, uint8 Reverse)
{
	uint8 buf[13];

	UTIL_LOCK(_util_sem3);
	memset(buf, 0, sizeof(buf));
	TimeToStr(buf, FormatTime);
	StringToBytes(TimeFlag, buf, 12);

	// 判断是否颠倒
	if (Reverse == 1)
	{
		app_hex_reverse(TimeFlag, 6);
	} 

	UTIL_UNLOCK(_util_sem3);

	return 0;
}


#if 0

/*******************************************************
 * 函数功能:从RTC获取时间将转为BCD格式
 * 函数名称:RtctimeToBcd
 * 输出参数:<pTime>[uint8 *] 输出时间格式BCD
 * 输入参数:<ReverSe>[uint8]  是否次序颠倒ssmmhhddMMyy
 * 返   回  值:无
 *******************************************************/
int8 RtctimeToBcd(uint8 Reverse, uint8 *pTime)
{
	rtc_data_type _curtime; // 当前时间

	rtc_read_time(&_curtime);// 从RTC获取时间
	pTime[5] = Bin2BCD(_curtime.seconds); // 秒
	pTime[4] = Bin2BCD(_curtime.minutes); //分
	pTime[3] = Bin2BCD(_curtime.hours); //时
	pTime[2] = Bin2BCD(_curtime.days); //日
	pTime[1] = Bin2BCD(_curtime.months); //月
	pTime[0] = Bin2BCD(_curtime.years); //年

	// 判断是否颠倒
	if (Reverse == 1)
	{
		app_hex_reverse(pTime, 6);
	}

	return 0;
}

/*******************************************************
 * 函数功能:获取当前系统时钟time_t格式
 * 函数名称:GetTime
 * 返   回  值:当前系统时间time_t
 *******************************************************/
time_t GetTime(void)
{
	time_t _tCurTime = 0;
	uint8 _curtime[6]; // 当前 系统时间

	UTIL_LOCK(_util_sem5);
	RtctimeToBcd(0, &_curtime[0]); // 从RTC获取时间将转为BCD格式
	BCD2HEX(_curtime, 6); // BCD转HEX码
	_tCurTime = app_timeconv(_curtime);
	UTIL_UNLOCK(_util_sem5);

	return _tCurTime;
}
#endif

/*******************************************************
 * 函数功能:冻结密度类型转为秒
 * 函数名称:app_densitytosec
 * 输入参数:<u8Density>[uint8] 冻结密度
 * 返   回  值:冻结密度对应的秒数
 *******************************************************/
uint16 app_densitytosec(uint8 u8Density)
{
	switch (u8Density)
	{
		case 0x01: // 15min
		{
			return 15 * 60;
		}
		case 0x02: // 30min
		{
			return 30 * 60;
		}
		case 0x03: // 60min
		{
			return 60 * 60;
		}
		case 0xFE: // 5min
		{
			return 5 * 60;
		}
		case 0xFF: // 1min
		{
			return 1 * 60;
		}
		default: // 不冻结
		{
			return 0;
		}
	}
}

/*******************************************************
 * 函数功能:冻结密度类型转为分钟
 * 函数名称:app_densitytomin
 * 输入参数:<u8Density>[uint8] 冻结密度
 * 返   回  值:冻结密度对应的秒数
 *******************************************************/
uint16 app_densitytomin(uint8 u8Density)
{
	switch (u8Density)
	{
		case 0x01: // 15min
		{
			return 15;
		}
		case 0x02: // 30min
		{
			return 30;
		}
		case 0x03: // 60min
		{
			return 60;
		}
		case 0xFE: // 5min
		{
			return 5;
		}
		case 0xFF: // 1min
		{
			return 1;
		}
		default: // 不冻结
		{
			return 15;
		}
	}
}

/*******************************************************
 * 函数功能:冻结密度转换
 * 函数名称:app_dinsitytomd
 * 返   回  值:协议冻结密度
 *******************************************************/
uint8 app_dinsitytomd(uint8 u8Density)
{
	switch (u8Density)
	{
		case 15:
		{
			return 1;
		}
		case 30:
		{
			return 2;
		}
		case 60:
		{
			return 3;
		}
		case 5:
		{
			return 254;
		}
		case 1:
		{
			return 255;
		}
		default:
		{
			return 1;
		}
	}
}

/*******************************************************
 * 函数功能:获取信息类对应的信息类属性表索引号
 * 函数名称:app_getdtprono
 * 输入参数:<u16Dt>[uint16] 信息类标志
 * 返   回  值:信息类属性表索引号
 *******************************************************/
uint16 app_getdtprono(uint16 u16Dt)
{
	uint8 _u8Dt1 = 0; // 信息类低字节
	uint8 _u8lj = 0; // 循环计数器

	// 信息类低字节
	_u8Dt1 = (u16Dt >> 8);

	// 计算_u8Dt1编号
	for (_u8lj = 0; _u8lj < 8; _u8lj++)
	{
		if ((1 << _u8lj) == _u8Dt1)
		{
			_u8Dt1 = _u8lj + 1;
			break;
		}
	}

	// 信息类属性表索引号
	return ((u16Dt & 0xFF) * 8 + _u8Dt1) - 1;
}


//在一段内存缓冲中查找指定字符串的位置，从头开始查找，不区分大小写。
//返回第一个找到的位置。
//str1 - 内存缓冲的头指针
//nLen1 - 内存缓冲长度
//str2 - 要查找匹配的字符串
uint8 * memstr( uint8 * str1, uint32 nLen1,  uint8 * str2)
{
	uint32 ls1 = nLen1;
	uint8 *cp = (uint8 *) str1;
	uint8 *s1, *s2;

	if ((NULL == str1) || (NULL == str2) || (nLen1 <= 0))
	{
		return NULL;
	}

	if (!*str2)
	{
		return ((uint8 *) str1);
	}

	while (ls1 > 0)
	{
		s1 = cp;
		s2 = (uint8 *) str2;

		while (*s1 && *s2 && !(*s1 - *s2))
		{
			s1++, s2++;
		}

		if (!*s2)
		{
			return (cp);
		}

		cp++;
		ls1--;
	}

	return (NULL);
}


/*******************************************************
 * 函数功能:时间格式修整(将单位时间后的余数置0)
 * 函数名称:app_datetrim
 * 输入参数:<interUnit>[uint8] 单位(1秒, 2分, 3时, 4日, 5月)
 * 输入参数:<date1>[time_t]  待修整的时间
 * 返   回  值:修整后的时间
 *******************************************************/
time_t app_datetrim(uint8 interUnit, const time_t date)
{
	struct tm t;
	uint16 maxs[] =
	{ 1, 60, 3600 };
	uint16 max = 0;

	if (date > 0)
	{
		if ((interUnit > 0) && (interUnit < 4))//秒、分、时
		{
			max = maxs[interUnit - 1];
			return (date / (max * max));
		}
		else
		{
			app_localtime(&t, date);
			t.tm_sec = 0;
			t.tm_min = 0;
			t.tm_hour = 0;
			if (interUnit == 5)
				t.tm_mday = 1;
			if (interUnit >= 6)
				t.tm_mon = 0;
			t.tm_wday = 0;
			t.tm_yday = 0;
			t.tm_isdst = -1;
			return app_mktime(&t);
		}
	}

	return 0;
}


/*******************************************************
 * 函数功能:计算两个时间单位差值
 * 函数名称:app_bcd5datediff
 * 输入参数:<interUnit>[uint8] 单位(1秒, 2分, 3时, 4日, 5月)
 * 输入参数:<date1>[time_t]  起始时间
 * 输入参数:<date2>[time_t]  结束时间
 * 返   回  值:单位差值
 *******************************************************/
int32 app_datediff(uint8 interUnit, const time_t date1, const time_t date2)
{
	time_t _date1 = date1; //app_datetrim(interUnit, date1); // 时间修整
	time_t _date2 = date2; //app_datetrim(interUnit, date2); // 时间修整

	switch (interUnit)
	{
		case 1: // 秒
		{
			return (int32) difftime(_date2, _date1);
		}
		case 2: // 分
		{
			return (int32) (difftime(_date2, _date1) / 60);
		}
		case 3: // 时
		{
			return (int32) (difftime(_date2, _date1) / 3600);
		}
		case 4: // 日
		{
			return (int32) (difftime(_date2, _date1) / 86400);
		}
		case 5: // 月
		{
			struct tm t1;
			struct tm t2;

			app_localtime(&t1, _date1);
			app_localtime(&t2, _date2);

			return (t2.tm_year * 12 + t2.tm_mon) - (t1.tm_year * 12 + t1.tm_mon);
		}
	}

	return 0;
}


/*******************************************************
 * 函数功能:计算两个时间单位差值
 * 函数名称:app_bcd5datediff
 * 输入参数:<interUnit>[uint8] 单位(2分, 3时, 4日, 5月)
 * 输入参数:<bcdTime1>[uint8 *]  起始时间
 * 输入参数:<bcdTime2>[uint8 *]  结束时间
 * 返   回  值:单位差值
 *******************************************************/
int32 app_bcd5datediff(uint8 interUnit, const uint8 *bcdTime1,
		const uint8 *bcdTime2)
{
	uint8 _u8Time1[6];
	uint8 _u8Time2[6];
	time_t _time1;
	time_t _time2;

	memset(_u8Time1, 0x0, 6);
	memset(_u8Time2, 0x0, 6);
	memcpy(&_u8Time1[1], bcdTime1, 5);
	memcpy(&_u8Time2[1], bcdTime2, 5);

	switch (interUnit)
	{
		case 3: // 时(将ssmm置0)
		{
			memset(&_u8Time1[0], 0x0, 2);
			memset(&_u8Time2[0], 0x0, 2);
			break;
		}
		case 4: // 日 (将ssmmhh置0)
		case 5: // 月(将ssmmhh置0)
		{
			memset(&_u8Time1[0], 0x0, 3);
			memset(&_u8Time2[0], 0x0, 3);
			break;
		}
	}

	BCD2HEX(_u8Time1, 6); // BCD转HEX码
	app_hex_reverse(_u8Time1, 6);
	_time1 = app_timeconv(_u8Time1);

	BCD2HEX(_u8Time2, 6); // BCD转HEX码
	app_hex_reverse(_u8Time2, 6);
	_time2 = app_timeconv(_u8Time2);

	return app_datediff(interUnit, _time1, _time2);
}


/*******************************************************
 * 函数功能:时间向前跳跃
 * 函数名称:app_upboundtime
 * 输入参数:<tEndTime>[time_t] 结束时间
 * 输入参数:<tStdTime>[time_t] 起始时间
 * 输入参数:<interUnit>[uint8] 单位(2分, 3时, 4日, 5月)
 * 输入参数:<interVal>[uint32] 周期
 * 返   回  值:跳跃后的时间
 * 备          注:获取一个时间，此时间满足条件(interUnit 1=秒，2=分，3=时，4=日，5=月，6=年)
 *         1、它在interUnit以下的各部分与tStdTime相同
 *         2、它在interUnit部分与tStdTime相差的间隔是interVal的倍数
 *         3、它是不小于date的符合1、2条件的最小时间
 *         例如,当tEndTime=2010-02-04 06:58:23,tStdTime=2010-02-01 01:12:24，interUnit=2,interVal=3时
 *         返回2010-02-04 07:00:24
 *******************************************************/
time_t app_upboundtime(time_t tEndTime, time_t tStdTime, uint8 interUnit, uint32 interVal)
{
	struct tm _stdTm;
	struct tm _newTm;
	time_t _tNewTime = 0;
	time_t _tEndTime = 0;
	int32 _s32Diff = 0;

	if (interVal == 0 || tEndTime <= 0)
	{
		return 0;
	}

	app_localtime(&_stdTm, tStdTime);
	app_localtime(&_newTm, tEndTime);
	_newTm.tm_sec = _stdTm.tm_sec;
	_tEndTime = app_mktime(&_newTm);
	_s32Diff = app_datediff(interUnit, tStdTime, _tEndTime);

	if ((_s32Diff % (long) interVal) == 0)
	{
		_tNewTime = tEndTime;
	}
	else
	{
		if (_s32Diff > 0)
		{
			_tNewTime = app_dateadd_two(interUnit, interVal - (_s32Diff % interVal), _tEndTime);
		}
		else
		{
			_s32Diff = -1 * _s32Diff;
			_tNewTime = app_dateadd_two(interUnit, (_s32Diff % interVal), _tEndTime);
		}
	}

	app_localtime(&_newTm, _tNewTime);

	if (interUnit >= 2)
	{
		_newTm.tm_sec = _stdTm.tm_sec;
	}

	if (interUnit >= 3)
	{
		_newTm.tm_min = _stdTm.tm_min;
	}

	if (interUnit >= 4)
	{
		_newTm.tm_hour = _stdTm.tm_hour;
	}

	if (interUnit >= 5)
	{
		_newTm.tm_mday = _stdTm.tm_mday;
	}

	if (interUnit >= 6)
	{
		_newTm.tm_mon = _stdTm.tm_mon;
	}

	_newTm.tm_wday = 0;
	_newTm.tm_yday = 0;
	_newTm.tm_isdst = -1;
	_tNewTime = app_mktime(&_newTm);

	if (_tNewTime < tEndTime)
	{
		_tNewTime = app_dateadd_two(interUnit, interVal, _tNewTime);
	}

	return _tNewTime;
}

/*******************************************************
 * 函数功能:时间向后跳跃
 * 函数名称:app_downboundtime
 * 输入参数:<tEndTime>[time_t] 结束时间
 * 输入参数:<tStdTime>[time_t] 起始时间
 * 输入参数:<interUnit>[uint8] 单位(2分, 3时, 4日, 5月)
 * 输入参数:<interVal>[uint32] 周期
 * 返   回  值:跳跃后的时间
 * 备          注:获取一个时间，此时间满足条件(interUnit 1=秒，2=分，3=时，4=日，5=月，6=年)
 *         1、它在interUnit以下的各部分与tStdTime相同
 *         2、它在interUnit部分与tStdTime相差的间隔是interVal的倍数
 *         3、它是不大于tEndTime的符合1、2条件的最大时间
 *         例如,当tEndTime=2010-02-04 06:58:23,tStdTime=2010-02-01 01:12:24，interUnit=2,interVal=3时
 *         //返回2010-02-04 06:57:24
 *******************************************************/
time_t app_downboundtime(time_t tEndTime, time_t tStdTime, uint8 interUnit, int32 interVal)
{
	time_t tNewTime = app_upboundtime(tEndTime, tStdTime, interUnit, interVal);

	if (tNewTime == tEndTime)
	{
		return tNewTime;
	}
	else
	{
		return app_dateadd_two(interUnit, -1 * interVal, tNewTime);
	}
}

/*******************************************************
 * 函数功能:电量计算并做过零判断
 * 函数名称:app_countelectricity
 * 输入参数:<fpStart>[fp64] 起码
 * 输入参数:<fpEnd>[fp64]   止码
 * 输入参数:<u8IntegerNum>[uint8]  电能示值整数位个数
 * 输出参数:<pEnergy>[fp64 *]  电能量
 * 返   回  值:-2 示度异常, -3 示度下降(越过0.5), -4 示度下降(不越过0.5), 0 正常
 *******************************************************/
int8 app_countelectricity(fp64 fpStart, fp64 fpEnd, uint8 u8IntegerNum, fp64 *pEnergy)
{
	fp64 _fpValue = 0;
	fp64 _fpLimitMax = 0;
	fp64 _fpLimitMin = 0;

	_fpLimitMax = pow((double) 10, (int) u8IntegerNum);
	_fpLimitMin = pow((double) 10, (int) (u8IntegerNum - 2)) * 99;

	// 示度异常
	if ((fpEnd >= _fpLimitMax) || (fpStart >= _fpLimitMax))
	{
		*pEnergy = 0;
		return -2;
	}

	_fpValue = fpEnd - fpStart;
	*pEnergy = _fpValue;

	if (_fpValue < 0)
	{
		// 示度回零
		if ((fpEnd <= (_fpLimitMax - _fpLimitMin)) && (fpStart >= _fpLimitMin))
		{
			*pEnergy = fpEnd + _fpLimitMax - fpStart;
		}

		// 示度下降
		else
		{
			*pEnergy = fpStart - fpEnd;

			if (*pEnergy >= 0.1)
			{
				return -3; // 下降超过0.1
			}
			else
			{
				return -4; // 下降不超过0.1
			}
		}
	}

	return 0;
}

/*******************************************************
 * 函数功能:求浮点数绝对值
 * 函数名称:app_fabs
 * 输入参数:<fpVal>[fp64] 浮点数
 * 返   回  值:浮点数绝对值
 *******************************************************/
fp64 app_fabs(fp64 fpVal)
{
	*(((int *) &fpVal) + 1) &= 0x7FFFFFFF;

	return fpVal;
}

/*******************************************************
 * 函数功能:获取字节中指定位置前有效位数
 * 函数名称:ValidBitCount
 * 输入参数:<u8Byte>[uint8] 被计算的字节
 * 输入参数:<u8End>[uint8] 指定位
 * 输出参数:无
 * 返   回  值:有效位个数
 *******************************************************/
uint8 BefValidBitCount(uint8 u8Byte, uint8 u8End)
{
	uint8 _u8lb = 0;
	uint8 _u8Count = 0;

	for (_u8lb = 0; _u8lb < u8End; _u8lb++)
	{
		if ((u8Byte & (1 << _u8lb)) > 0)
		{
			_u8Count++;
		}
	}

	return _u8Count;
}

/*******************************************************
 * 函数功能:公用函数库初始化
 * 函数名称:app_init
 * 返   回  值:无
 *******************************************************/
void app_init(void *param)
{
	_util_sem1 = OSSemCreate(1);
	_util_sem2 = OSSemCreate(1);
	_util_sem3 = OSSemCreate(1);
	_util_sem4 = OSSemCreate(1);
	_util_sem5 = OSSemCreate(1);
}
