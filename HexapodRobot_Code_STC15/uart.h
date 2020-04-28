#ifndef __UART_H
#define __UART_H

#include "stc15w.h"
#include "config.h"


//串口1的部分定义
/******************************************/
#define NONE_PARITY     0       //无校验
#define ODD_PARITY      1       //奇校验
#define EVEN_PARITY     2       //偶校验
#define MARK_PARITY     3       //标记校验
#define SPACE_PARITY    4       //空白校验

#define PARITYBIT NONE_PARITY   //定义校验位

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7
/******************************************/


//串口2的部分定义
/****************************************/
#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1
#define S2_S0 0x01              //P_SW2.0
/*****************************************/

typedef unsigned char BYTE;
typedef unsigned int WORD;

extern uchar receiveFlag1;  //接收标志
extern uchar message;	 //消息
extern void Uart1Init(void);//串口1初始化 波特率的值和系统晶振频率可以在config.h中设置
extern void Uart2Init(void);//串口2初始化 波特率的值和系统晶振频率可以在config.h中设置

extern void Uart1SendString(uchar *str); //串口1发送字符串
extern void Uart2SendString(uchar *str); //串口2发送字符串

extern void Uart1SendData(uchar dat);		  //串口1发送字符
extern void Uart2SendData(uchar dat);		  //串口2发送字符

extern void Uart1Send(uchar dat);		  //串口1发送字符
extern void Uart2Send(uchar dat);		  //串口2发送字符



//解析指令所用到的数据
/**********************************************************************************/
extern void AnalysisCom(void); 		//串口数据缓冲
extern void ReceiveDataConvertToPwmValue();//解析动作指令


#define USART1_REC_LEN  			256  	//定义最大接收字节数 256
extern uchar  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART1_REC_LEN个字节.末字节为换行符 
extern uchar USART1_RX_STA;         		//接收状态标记	
extern uchar usart1ReceiveSuccess;//一帧数据接收完成标志位 使用后要置0


//自定义的第二组USART1_RX_BUF缓冲区 用于操作播放语音的数据
#define USART1_REC_LEN2  			10 	//定义最大接收字节数 10	  	
extern uchar  USART1_RX_BUF2[USART1_REC_LEN2]; //接收缓冲,最大USART1_REC_LEN2个字节.末字节为换行符 
extern uchar USART1_RX_STA2;         		//接收状态标记	
extern uchar usart1ReceiveSuccess2;//一帧数据接收完成标志位 使用后要置0 

extern uchar firstdata;//第一回接收到的值
/**********************************************************************************/


#endif

