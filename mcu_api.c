/****************************************Copyright (c)*************************
**                               Copyright (C) 2014-2020, Tuya Inc., All Rights Reserved
**
**                                 http://www.tuya.com
**
**--------------Revision record---------------------------------------------------
** version: v1.0
** date : may 3, 2017 
description: Initial version
**

**version::v2.0
** date: March 23, 2020
** description: 
1. Added module unbinding interface support, command code 0x09.
2.Add rf RF test interface support, command code 0x0e.
3.Add record-based data reporting interface support,command code 0xe0.
4. Added access to real-time time API support,command code 0xe1.
5. Added support for modifying sleep mode state bluetooth broadcast interval,command code 0xe2.
6. Added support for turning off system clock,command code 0xe4.
7. Increase low power consumption to enable support,commadn code 0xe5.
8. Add dynamic password authentication interface support,command code 0xe6.
9. Added support for disconnecting Bluetooth connection,command code 0xe7.
10. Added support for querying MCU version number,command code 0xe8.
11. Added support for MCU to actively send version Numbers,command code 0xe9.
12. Add OTA upgrade request support,command code 0xea.
13. Add OTA update file information support,command 0xeb.
14. Add OTA upgrade file migration request support,command code 0xec.
15. Add OTA upgrade data support,command code 0xed.
16. Add OTA upgrade end support,command code 0xee.
17. Added support for MCU to acquire module version information,commadn code 0xa0.
18. Added support for resuming factory Settings notifications,command code 0xa1.
19. Add MCU OTA demo code.
20. Optimized bt_uart_service.
说明：初始版本
1.增加了模块解绑接口支持，命令代码0x09。

2.增加射频测试接口支持，命令代码0x0e。

3.添加基于记录的数据报告接口支持，命令代码0xe0。

4.添加了对实时API支持的访问，命令代码0xe1。

5.增加了对修改睡眠模式状态蓝牙广播间隔的支持，命令代码0xe2。

6.增加了关闭系统时钟的支持，命令代码0xe4。

7.增加低功耗以启用支持，通信代码0xe5。

8.添加动态密码认证接口支持，命令代码0xe6。

9.增加了对断开蓝牙连接的支持，命令代码0xe7。

10.增加了对查询MCU版本号、命令代码0xe8的支持。

11.增加了对MCU主动发送版本号、命令代码0xe9的支持。

12.添加OTA升级请求支持，命令代码0xea。

13.添加OTA更新文件信息支持，命令0xeb。

14.添加OTA升级文件迁移请求支持，命令代码0xec。

15.添加OTA升级数据支持，命令代码0xed。

16.添加OTA升级端支持，命令代码0xee。

17.增加了对MCU获取模块版本信息的支持，commadn代码0xa0。

18.添加了对恢复出厂设置通知的支持，命令代码0xa1。

19.添加MCU OTA演示代码。

20.优化的bt_uart_服务。
**
**-----------------------------------------------------------------------------
******************************************************************************/


#define MCU_API_GLOBAL

#include "bluetooth.h"

/*****************************************************************************
Function name: hex_to_bcd
Function description: hex to bcd
Input parameters: 
	Value_H: high byte / Value_L: low byte
Return value:
	bcd_value:Data after conversion is completed.
	#定义MCU_API_全局
	#包括“bluetooth.h”蓝牙
函数名称：十六进制到十六进制bcd

功能描述：十六进制到bcd

输入参数：

值\u H:高字节/值\u L:低字节

返回值：

bcd_值：转换完成后的数据。
*****************************************************************************/
unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L)//无符号字符十六进制到bcd（无符号主高位值，无符号低位值）//

{
  unsigned char bcd_value;//无符号字符bcd_值；//
  
  if((Value_H >= '0') && (Value_H <= '9'))
    Value_H -= '0';
  else if((Value_H >= 'A') && (Value_H <= 'F'))//否则如果//
    Value_H = Value_H - 'A' + 10;
  else if((Value_H >= 'a') && (Value_H <= 'f'))
    Value_H = Value_H - 'a' + 10;
   
  bcd_value = Value_H & 0x0f;
  
  bcd_value <<= 4;
  if((Value_L >= '0') && (Value_L <= '9'))
    Value_L -= '0';
  else if((Value_L >= 'A') && (Value_L <= 'F'))
    Value_L = Value_L - 'a' + 10;
  else if((Value_L >= 'a') && (Value_L <= 'f'))
    Value_L = Value_L - 'a' + 10;
  
  bcd_value |= Value_L & 0x0f;

  return bcd_value;
}
/*****************************************************************************
Function name: my_strlen
Function description: find the length of the string
Input parameter:
	src: source address
Return parameter: 
	len: data length
功能名称：my_strlen

函数说明：查找字符串的长度

输入参数：

src:源地址

返回参数：

len：数据长度

*****************************************************************************/
unsigned long my_strlen(unsigned char *str)  
{
  unsigned long len = 0;
  if(str == NULL)
  { 
    return 0;
  }
  
  for(len = 0; *str ++ != '\0'; )
  {
    len ++;
  }
  
  return len;
}
/*****************************************************************************
Function name: my_memset
Function description: set the first count bytes of the memory area referred to by src to the character ch
Input parameter: src: source address
		   ch: setting character
           count:Operation data length
Return parameters :
	src:source address after data processing
	函数名称：my_memset

功能描述：将src引用的内存区域的第一个计数字节设置为字符ch

输入参数：src：源地址

设置字符

计数：操作数据长度

返回参数：

src：数据处理后的源地址
*****************************************************************************/
void *my_memset(void *src,unsigned char ch,unsigned short count)
{
  unsigned char *tmp = (unsigned char *)src;
  
  if(src == NULL)
  {
    return NULL;
  }
  
  while(count --)
  {
    *tmp ++ = ch;
  }
  
  return src;
}
/*****************************************************************************
Function name: my_memcpy
Function description: memory copy
Input parameter: dest: destination address
           src:Source address
           count:Number of data copies
 Return parameters :
 	src:Source address after data processing
函数名称：my_memcpy

功能描述：内存复制

输入参数：dest：目标地址

源地址

计数：数据拷贝数

返回参数：

src：数据处理后的源地址
*****************************************************************************/
void *my_memcpy(void *dest, const void *src, unsigned short count)  
{  
  unsigned char *pdest = (unsigned char *)dest;  
  const unsigned char *psrc  = (const unsigned char *)src;  
  unsigned short i;
  
  if(dest == NULL || src == NULL)
  { 
    return NULL;
  }
  
  if((pdest <= psrc) || (pdest > psrc + count))
  {  
    for(i = 0; i < count; i ++)
    {  
      pdest[i] = psrc[i];  
    }  
  }
  else
  {
    for(i = count; i > 0; i --)
    {  
      pdest[i - 1] = psrc[i - 1];  
    }  
  }  
  
  return dest;  
}
/*****************************************************************************
Function name: my_memcmp
Function description: memory comparison
Input parameter: buffer1: memory 1
           buffer2:memory 2
           	count:Comparative length
Return parameters :0:buffer1=buffer2; -1:buffer1<buffer2; 1:buffer1>buffer2
函数名称：my_memcmp

功能描述：内存比较

输入参数：buffer1：内存1

缓冲区2：内存2

计数：相对长度

返回参数：0:buffer1=buffer2-1:buffer1<buffer2；1:buffer1>buffer2
*****************************************************************************/
int my_memcmp(const void *buffer1,const void *buffer2,int count)
{
   if (!count)
      return(0);
   while ( --count && *(char *)buffer1 == *(char *)buffer2)
   {
      buffer1 = (char *)buffer1 + 1;
        buffer2 = (char *)buffer2 + 1;
   }
   return( *((unsigned char *)buffer1) - *((unsigned char *)buffer2) );
}
/*****************************************************************************
Function name: my_atoll
Function description: string to integer
Input parameter: P string
Return parameter: integer
功能名称：my_atoll

函数说明：字符串到整数

输入参数：P字符串

返回参数：整数
*****************************************************************************/
 
#include <ctype.h>
 long my_atoll(const char *p)
{
	  long n;
	int c, neg = 0;
	unsigned char   *up = (unsigned char *)p;

	if (!isdigit(c = *up)) {
		while (isspace(c))
			c = *++up;
		switch (c) {
		case '-':
			neg++;
			/* FALLTHROUGH */
		case '+':
			c = *++up;
		}
		if (!isdigit(c))
			return (0);
	}

	for (n = '0' - c; isdigit(c = *++up); ) {
		n *= 10; /* two steps to avoid unnecessary overflow */
		n += '0' - c; /* accum neg to avoid surprises at MAX */
	}

	return (neg ? n : -n);
}


/*****************************************************************************
Function name: int_to_byte
Function description: split the int type into four bytes
Input parameter: 
	number:Number to be processed
	value:4 bytes of data after processing
Return parameter: none
函数名称：int_to_字节

函数描述：将int类型拆分为四个字节

输入参数：

编号：要处理的编号

值：处理后的4字节数据

返回参数：无
****************************************************************************/
void int_to_byte(unsigned long number,unsigned char value[4])
{
  value[0] = number >> 24;
  value[1] = number >> 16;
  value[2] = number >> 8;
  value[3] = number & 0xff;
}
/*****************************************************************************
Function name: byte_to_int
Function description: merge 4 bytes into 1 32bit variable
Input parameters: value:4 byte array
Return parameter: 
	number:32bit variables after merging
	函数名称：byte_to_int

函数描述：将4个字节合并为1个32位变量

输入参数：值：4字节数组

返回参数：

编号：合并后的32位变量
****************************************************************************/
unsigned long byte_to_int(const unsigned char value[4])
{
  unsigned long nubmer = 0;

  nubmer = (unsigned long)value[0];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[1];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[2];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[3];
  
  return nubmer;
}
#ifndef BT_CONTROL_SELF_MODE
/*****************************************************************************
Function name: mcu_get_reset_bt_flag
Function description: MCU acquires reset bt success flag
Input parameters: none
Return parameter: RESET_BT_ERROR:failed/RESET_BT_SUCCESS:success
Instructions for use: 1:MCU call this function after calling mcu_reset_bt () actively to get the reset state
		   2: if it is in module self-processing mode, MCU does not need to call this function
功能名称：mcu_获取_重置_bt_标志

功能描述：MCU获取复位bt成功标志

输入参数：无

返回参数：重置错误：失败/重置成功：成功

使用说明：1：MCU在主动调用MCU_reset_bt（）后调用此函数，以获取复位状态

2：如果是模块自处理模式，MCU不需要调用此函数
*****************************************************************************/
unsigned char mcu_get_reset_bt_flag(void)
{
  return reset_bt_flag;
}
/*****************************************************************************
Function name: mcu_reset_bt
Function description: MCU actively resets bt working mode
Input parameters: none
Return parameter: none
Instructions for use: 1:MCU is called actively, and the mcu_get_reset_bt_flag () function is used to obtain whether the reset bt is successful or not.
		   2: if it is in module self-processing mode, MCU does not need to call this function
功能名称：mcu\U复位\U bt

功能描述：MCU主动复位bt工作模式

输入参数：无

返回参数：无

使用说明：1：主动调用MCU，使用MCU_get_reset_bt_flag（）函数获取重置bt是否成功。

2：如果是模块自处理模式，MCU不需要调用此函数
*****************************************************************************/
void mcu_reset_bt(void)
{
  reset_bt_flag = RESET_BT_ERROR;
  
  bt_uart_write_frame(BT_RESET_CMD, 0);
}
/*****************************************************************************
Function name: mcu_get_bt_work_state
Function description: MCU actively acquires the current bt working status
Input parameters: none
Return parameter: BT_WORK_SATE_E:
            BT_UN_BIND:Unbound state
            BT_UNCONNECT:Bound but not connected
            BT_CONNECTED:Bound and connected
Instructions for use: none
功能名称：mcu_获取_bt_工作_状态

功能描述：MCU主动获取当前bt工作状态

输入参数：无

返回参数：BT_工作状态：

BT_UN_绑定：未绑定状态

BT_未连接：已绑定但未连接

BT_CONNECTED：绑定和连接

使用说明：无
*****************************************************************************/
unsigned char mcu_get_bt_work_state(void)
{
  return bt_work_state;
}
#endif

/*****************************************************************************
Function name: mcu_dp_raw_update
Function description: raw dp data upload
Return parameter: dpid:id
           value:dp date
           len:length
Return parameter: none
功能名称：mcu\U dp\U raw\U更新

功能描述：原始dp数据上传

返回参数：dpid:id

值：dp日期

len:长度

返回参数：无
*****************************************************************************/
unsigned char mcu_dp_raw_update(unsigned char dpid,const unsigned char value[],unsigned short len)
{
  unsigned short length = 0;
  
  if(stop_update_flag == ENABLE)
    return SUCCESS;
  //
  length = set_bt_uart_byte(length,dpid);
  length = set_bt_uart_byte(length,DP_TYPE_RAW);
  //
  length = set_bt_uart_byte(length,len / 0x100);
  length = set_bt_uart_byte(length,len % 0x100);
  //
  length = set_bt_uart_buffer(length,(unsigned char *)value,len);
  
  bt_uart_write_frame(STATE_UPLOAD_CMD,length);
  
  return SUCCESS;
}
/*****************************************************************************
Function name: mcu_dp_bool_update
Function description: Bool dp data upload
Input parameter: dpid:id number
           value:
Return parameter: none
功能名称：mcu\U dp\U bool\U更新

功能描述：Bool dp数据上传

输入参数：dpid:id编号

价值：

返回参数：无
*****************************************************************************/
unsigned char mcu_dp_bool_update(unsigned char dpid,unsigned char value)
{
  unsigned short length = 0;
  
  if(stop_update_flag == ENABLE)
    return SUCCESS;
  
  length = set_bt_uart_byte(length,dpid);
  length = set_bt_uart_byte(length,DP_TYPE_BOOL);
  //
  length = set_bt_uart_byte(length,0);
  length = set_bt_uart_byte(length,1);
  //
  if(value == FALSE)
  {
    length = set_bt_uart_byte(length,FALSE);
  }
  else
  {
    length = set_bt_uart_byte(length,1);
  }
  
  bt_uart_write_frame(STATE_UPLOAD_CMD,length);
  
  return SUCCESS;
}
/*****************************************************************************
Function name: mcu_dp_value_update
Function description: value dp data upload
Input parameter: dpid:id number
           value:
Return parameter: none
功能名称：mcu\U dp\U值\U更新

功能描述：值dp数据上传

输入参数：dpid:id编号

价值：

返回参数：无
*****************************************************************************/
unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value)
{
  unsigned short length = 0;
  
  if(stop_update_flag == ENABLE)
    return SUCCESS;
  
  length = set_bt_uart_byte(length,dpid);
  length = set_bt_uart_byte(length,DP_TYPE_VALUE);
  //
  length = set_bt_uart_byte(length,0);
  length = set_bt_uart_byte(length,4);
  //
  length = set_bt_uart_byte(length,value >> 24);
  length = set_bt_uart_byte(length,value >> 16);
  length = set_bt_uart_byte(length,value >> 8);
  length = set_bt_uart_byte(length,value & 0xff);
  
  bt_uart_write_frame(STATE_UPLOAD_CMD,length);
  
  return SUCCESS;
}
/*****************************************************************************
Function name: mcu_dp_string_update
Function description: string dp data upload
Input parameter: dpid:id number
           value:current DP value pointer
           len:data length
Return parameter: none
功能名称：mcu\U dp\U字符串\U更新

功能描述：字符串dp数据上传

输入参数：dpid:id编号

值：当前DP值指针

len：数据长度

返回参数：无
*****************************************************************************/
unsigned char mcu_dp_string_update(unsigned char dpid,const unsigned char value[],unsigned short len)
{
  unsigned short length = 0;
  
  if(stop_update_flag == ENABLE)
    return SUCCESS;
  //
  length = set_bt_uart_byte(length,dpid);
  length = set_bt_uart_byte(length,DP_TYPE_STRING);
  //
  length = set_bt_uart_byte(length,len / 0x100);
  length = set_bt_uart_byte(length,len % 0x100);
  //
  length = set_bt_uart_buffer(length,(unsigned char *)value,len);
  
  bt_uart_write_frame(STATE_UPLOAD_CMD,length);
  
  return SUCCESS;
}
/*****************************************************************************
Function name: mcu_dp_enum_update
Function description: enum dp data upload
Input parameter: dpid:id number
           value:
Return parameter: none
功能名称：mcu\U dp\U枚举更新

功能描述：枚举dp数据上传

输入参数：dpid:id编号

价值：

返回参数：无
*****************************************************************************/
unsigned char mcu_dp_enum_update(unsigned char dpid,unsigned char value)
{
  unsigned short length = 0;
  
  if(stop_update_flag == ENABLE)
    return SUCCESS;
  
  length = set_bt_uart_byte(length,dpid);
  length = set_bt_uart_byte(length,DP_TYPE_ENUM);
  //
  length = set_bt_uart_byte(length,0);
  length = set_bt_uart_byte(length,1);
  //
  length = set_bt_uart_byte(length,value);
  
  bt_uart_write_frame(STATE_UPLOAD_CMD,length);
  
  return SUCCESS;
}
/*****************************************************************************
Function name: mcu_dp_fault_update
Function description: bitmap dp data upload
Input parameter: dpid:id number
           value:current dp value
Return parameter: none
功能名称：mcu\U dp\U故障\U更新

功能描述：位图dp数据上传

输入参数：dpid:id编号

值：当前dp值

返回参数：无
*****************************************************************************/
unsigned char mcu_dp_fault_update(unsigned char dpid,unsigned long value)
{
  unsigned short length = 0;
   
  if(stop_update_flag == ENABLE)
    return SUCCESS;
  
  length = set_bt_uart_byte(length,dpid);
  length = set_bt_uart_byte(length,DP_TYPE_BITMAP);
  //
  length = set_bt_uart_byte(length,0);
  
  if((value | 0xff) == 0xff)
  {
    length = set_bt_uart_byte(length,1);
    length = set_bt_uart_byte(length,value);
  }
  else if((value | 0xffff) == 0xffff)
  {
    length = set_bt_uart_byte(length,2);
    length = set_bt_uart_byte(length,value >> 8);
    length = set_bt_uart_byte(length,value & 0xff);
  }
  else
  {
    length = set_bt_uart_byte(length,4);
    length = set_bt_uart_byte(length,value >> 24);
    length = set_bt_uart_byte(length,value >> 16);
    length = set_bt_uart_byte(length,value >> 8);
    length = set_bt_uart_byte(length,value & 0xff);
  }    
  
  bt_uart_write_frame(STATE_UPLOAD_CMD,length);

  return SUCCESS;
}
/*****************************************************************************
Function name: mcu_get_dp_download_bool
Function description: mcu acquires the DP value issued by Bool
Input parameter: value:dp data buffer address
           length:dp data length
Return parameter: current DP value of bool
功能名称：mcu_get_dp_download_bool

功能描述：mcu获取Bool发出的DP值

输入参数：值：dp数据缓冲区地址

长度：dp数据长度

返回参数：bool的当前DP值
*****************************************************************************/
unsigned char mcu_get_dp_download_bool(const unsigned char value[],unsigned short len)
{
  return(value[0]);
}
/*****************************************************************************
Function name: mcu_get_dp_download_enum
Function description: mcu acquires the DP value issued by enum
Input parameter: value:dp data buffer address
           length:dp data length
Return parameter: current DP value of eunm
功能名称：mcu_get_dp_download_enum

功能描述：mcu获取enum发出的DP值

输入参数：值：dp数据缓冲区地址

长度：dp数据长度

返回参数：eunm的当前DP值
*****************************************************************************/

unsigned char mcu_get_dp_download_enum(const unsigned char value[],unsigned short len)
{
  return(value[0]);
}
/*****************************************************************************
Function name: mcu_get_dp_download_value
Function description: mcu acquires the DP value issued by value
Input parameter: value:dp data buffer address
           length:dp data length
Return parameter: current DP value
功能名称：mcu\u获取\u dp\u下载\u值

功能描述：mcu获取由value发出的DP值

输入参数：值：dp数据缓冲区地址

长度：dp数据长度

返回参数：当前DP值
*****************************************************************************/

unsigned long mcu_get_dp_download_value(const unsigned char value[],unsigned short len)
{
  return(byte_to_int(value));
}
/*****************************************************************************
Function name: uart_receive_input
Function description: receiving data processing
Input parameters: value: uart port receives byte data
Return parameter: none
Instructions for use: call this function in the MCU uart port receiving function and pass in the received data as parameters
功能名称：uart\u接收\u输入

功能描述：接收数据处理

输入参数：值：uart端口接收字节数据

返回参数：无

使用说明：在MCU uart端口接收函数中调用此函数，并将接收到的数据作为参数传入
*****************************************************************************/
void uart_receive_input(unsigned char value)
{
  if((queue_in > queue_out) && ((queue_in - queue_out) >= sizeof(bt_queue_buf)))
  {
	// the Data queue is full
  }
  else if((queue_in < queue_out) && ((queue_out  - queue_in) == 0))
  {
    //Data queue is full
  }
  else
  {
    //Queue is not full
    if(queue_in >= (unsigned char *)(bt_queue_buf + sizeof(bt_queue_buf)))
    {
      queue_in = (unsigned char *)(bt_queue_buf);
    }
    
    *queue_in ++ = value;
  }
}
/*
 *@brief Function for receive uart data.
 *@param
 *
 *@note
 **@接收uart数据的简要功能。

*@param

*

*@注
 * */

typedef enum {
    MCU_UART_REV_STATE_FOUND_NULL,
    MCU_UART_REV_STATE_FOUND_HEAD,
    MCU_UART_REV_STATE_FOUND_CMD,
    MCU_UART_REV_STATE_FOUND_LEN_H,
    MCU_UART_REV_STATE_FOUND_LEN_L,
    MCU_UART_REV_STATE_FOUND_DATA,
    MCU_UART_REV_STATE_UNKOWN,
} mcu_uart_rev_state_type_t;

#define UART_RX_BUFFER_MAX   (PROTOCOL_HEAD + BT_UART_RECV_BUF_LMT)
#define UART_RX_DATA_LEN_MAX (PROTOCOL_HEAD + BT_UART_RECV_BUF_LMT)
static volatile mcu_uart_rev_state_type_t current_uart_rev_state_type = MCU_UART_REV_STATE_FOUND_NULL;
static uint8_t bt_uart_rx_buf_temp[3] = {0};
static uint16_t uart_data_len =  0;
static volatile uint16_t UART_RX_Count = 0;


static uint8_t mcu_common_uart_data_unpack(uint8_t dataa)
{
    uint8_t ret = FALSE;

    bt_uart_rx_buf_temp[0] = bt_uart_rx_buf_temp[1];
    bt_uart_rx_buf_temp[1] = bt_uart_rx_buf_temp[2];
    bt_uart_rx_buf_temp[2] = dataa;

    if((bt_uart_rx_buf_temp[0]==0x55)&&(bt_uart_rx_buf_temp[1]==0xAA)&&(bt_uart_rx_buf_temp[2]==0x00))
    {
        my_memset(bt_uart_rx_buf,0,sizeof(bt_uart_rx_buf));
        my_memcpy(bt_uart_rx_buf,bt_uart_rx_buf_temp,3);
        my_memset(bt_uart_rx_buf_temp,0,3);
        UART_RX_Count = 3;
        current_uart_rev_state_type = MCU_UART_REV_STATE_FOUND_HEAD;
        uart_data_len = 0;
        return ret;
    }
    switch(current_uart_rev_state_type)
    {
    case MCU_UART_REV_STATE_FOUND_NULL:
        break;
    case MCU_UART_REV_STATE_FOUND_HEAD:
        bt_uart_rx_buf[UART_RX_Count++] = dataa;
        current_uart_rev_state_type = MCU_UART_REV_STATE_FOUND_CMD;
        break;
    case MCU_UART_REV_STATE_FOUND_CMD:
        bt_uart_rx_buf[UART_RX_Count++] = dataa;
        current_uart_rev_state_type = MCU_UART_REV_STATE_FOUND_LEN_H;
        break;
    case MCU_UART_REV_STATE_FOUND_LEN_H:
        bt_uart_rx_buf[UART_RX_Count++] = dataa;
        uart_data_len = (bt_uart_rx_buf[UART_RX_Count-2]<<8)|bt_uart_rx_buf[UART_RX_Count-1];
        if(uart_data_len>UART_RX_DATA_LEN_MAX)
        {
            my_memset(bt_uart_rx_buf_temp,0,3);
            my_memset(bt_uart_rx_buf,0,sizeof(bt_uart_rx_buf));
            UART_RX_Count = 0;
            current_uart_rev_state_type = MCU_UART_REV_STATE_FOUND_NULL;
            uart_data_len = 0;
        }
        else if(uart_data_len>0)
        {
            current_uart_rev_state_type = MCU_UART_REV_STATE_FOUND_LEN_L;
        }
        else
        {
            current_uart_rev_state_type = MCU_UART_REV_STATE_FOUND_DATA;
        }
        break;
    case MCU_UART_REV_STATE_FOUND_LEN_L:
        bt_uart_rx_buf[UART_RX_Count++] = dataa;   //DATA
        uart_data_len--;
        if(uart_data_len==0)
        {
            current_uart_rev_state_type = MCU_UART_REV_STATE_FOUND_DATA;
        }
        break;
    case MCU_UART_REV_STATE_FOUND_DATA:
        bt_uart_rx_buf[UART_RX_Count++] = dataa;  //sum data
        ret = TRUE;
        break;
    default:
        my_memset(bt_uart_rx_buf_temp,0,3);
        my_memset(bt_uart_rx_buf,0,sizeof(bt_uart_rx_buf));
        UART_RX_Count = 0;
        current_uart_rev_state_type = MCU_UART_REV_STATE_FOUND_NULL;
        uart_data_len = 0;
        break;
    };

    return ret;

}

/*****************************************************************************
Function name: bt_uart_service
Function description: bt serial port processing service
Input parameters: none
Return parameter: none
Instructions: call this function in the while loop of the MCU main function
功能名称：bt_uart_服务

功能描述：bt串口处理服务

输入参数：无

返回参数：无

说明：在MCU主函数的while循环中调用此函数
*****************************************************************************/
void bt_uart_service(void)
{
  static unsigned short rx_in = 0;
  unsigned short offset = 0;
  unsigned short rx_value_len = 0;             //Data frame length
  
  if((rx_in < sizeof(bt_uart_rx_buf)) && get_queue_total_data() > 0)
  {
    if(mcu_common_uart_data_unpack(Queue_Read_Byte()))
    {
		data_handle(0);
		rx_value_len = bt_uart_rx_buf[LENGTH_HIGH] * 0x100 + bt_uart_rx_buf[LENGTH_LOW] + PROTOCOL_HEAD;
		my_memset(bt_uart_rx_buf_temp,0,3);
        my_memset(bt_uart_rx_buf,0,sizeof(bt_uart_rx_buf));
        UART_RX_Count = 0;
        current_uart_rev_state_type = MCU_UART_REV_STATE_FOUND_NULL;
        uart_data_len = 0;
    }
  }

}
/*****************************************************************************
Function name: bt_protocol_init
Function description: protocol serial port initialization function
Input parameters: none
Return parameter: none
Instructions for use: this function must be called in the MCU initialization code
功能名称：bt_协议_init

功能描述：协议串口初始化功能

输入参数：无

返回参数：无

使用说明：此函数必须在MCU初始化代码中调用
*****************************************************************************/
void bt_protocol_init(void)
{
  queue_in = (unsigned char *)bt_queue_buf;
  queue_out = (unsigned char *)bt_queue_buf;
  //
#ifndef BT_CONTROL_SELF_MODE
  bt_work_state = BT_SATE_UNKNOW;
#endif
	//mcu_ota_init();

}

