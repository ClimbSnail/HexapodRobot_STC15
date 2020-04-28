/***************************************************************
本六足机器人程序使用的是STC15W4K48S4单片机(与本单片机结构相同的
均可以使用)默认22.1184MHz的晶振。程序稳定产生21路PWM(均为模拟PWM)
产生的动作应与舵机的初始安装状态有关
***************************************************************/

//P0对应PWM0~7 P2对应PWM9~16 P1^0和P1^1分别对应PWM8 PWM17
//使用串口1进行连接串口蓝牙设备
//使用P1.6与P1.7进行AD采集

#include "stc15w.h"
#include "config.h"
#include "timer.h"
#include "uart.h"
#include "action.h"
#include "delay.h"
#include "MPU6050.h"
#include "oled.h"
#include "pwm.h"
#include "XFS5152V2.h"
#include "ActionArr.h"

/************************************************************************
函数名称：	IOInit(void) 初始化IO口
功能描述： 设置各个IO口输出模式为:传统模式 准双向口
入口参数： none
返 回 值： none
其他说明： 不初始化会输出不了PWM，P30 P31如果清零 串口将不能用
**************************************************************************/
void IOInit(void)
{
    //不设置会输出不了PWM
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    P6M0 = 0x00;
    P6M1 = 0x00;
    P7M0 = 0x00;
    P7M1 = 0x00;

    //以下最好清零
    P0 = 0x00;
    P1 = 0x00;
    P2 = 0x00;
    P3 = 0x03;	//P30 P31如果清零 串口将不能用
    P4 = 0x00;
    P5 = 0x00;
}


/************************************************************************
函数名称：	ExecuteOrder(unsigned char order)
功能描述： 计算每次pwm更新需要多大的增量(用于减速控制)
入口参数： order命令代号
返 回 值： none
其他说明： 执行动画指令 order范围0x00~0x40 总共能容纳65条指令(0x0D因为接受帧
					处理的问题,暂时不能用)
**************************************************************************/
void ExecuteOrder(unsigned char order)
{
    //在此补充指令执行代码
}

/************************************************************************
函数名称：	CheckAndDealY( void )
功能描述： 检查并处理语音指令
入口参数： none
返 回 值： none
其他说明： 开头‘YS’为语音设置指令 开头‘YT’为语音转发指令
**************************************************************************/
char CheckAndDealY( void )
{
    if( usart1ReceiveSuccess2 )//如果语音缓冲区表明接收到完成指令帧
    {
        if( USART1_RX_BUF2[0] == 'Y' )//语音命令帧头标志
            if( USART1_RX_BUF2[1] == 'S' )
                YSOrder( USART1_RX_BUF2[2] );//语音设置命令
            else if( USART1_RX_BUF2[1] == 'T' ) //语音转发
                XFS_FrameInfo( &USART1_RX_BUF2[2] );

        usart1ReceiveSuccess2 = 0;//标志位清零
        return 1;
    }
    else
        return 0;
}


/************************************************************************
函数名称：	CheckAndDealActionDebug( void )
功能描述： 检查并处理动作及调试指令
入口参数： none
返 回 值： none
其他说明： 开头‘D’为Debug模式 其余为动作指令
**************************************************************************/
char CheckAndDealActionDebug( void )
{
    if( usart1ReceiveSuccess )//是否已接收完整帧
    {
        if( USART1_RX_BUF[0] == 'D' )//数组调试模式帧头标志
            ReceiveDataConvertToPwmValue();//数组调试模式 一次调试执行1个动作

        while( USART1_RX_BUF[0] && USART1_RX_BUF[0] < 0x41 )//添加循环的目的是只要发送一次指令就可以不停执行一个动作 直到下一条指令
            ExecuteOrder(USART1_RX_BUF[0]);//执行动作指令 需要接收到停止命令的时候，当前动作才可以被打断

        usart1ReceiveSuccess = 0;//标志位清零
        return 1;
    }
    else
        return 0;
}



void main()
{
		uchar x = 9;
    IOInit();//初始化IO口 必须
		
		BEEP = 1;
    Uart1Init();//串口1初始化(默认115200) 波特率的值和系统晶振频率可以在config.h中设置
    Uart2Init();//串口2初始化(默认115200) 波特率的值和系统晶振频率可以在config.h中设置
//		while(1){
//    Uart1SendString("串口1成功发送!\r\n");//串口1发送数据
//		Delay200ms();
//		}

    Timer0Init_Mode12T();//定时器0初始化	控制pwm生成通道0
    Timer1Init_Mode12T();//定时器1初始化	控制pwm生成通道1
    Timer3Init_Mode12T();//定时器3初始化	控制pwm生成通道2
    Timer4Init_Mode12T();//定时器4初始化 控制舵机转速

    LED = 1;
		while(1);
		//以下代码测试用
		while(1)
		{
			PwmChange(x,1000);//改变R2的pwm为2000
			UpDataTimingLeft();//更新剩余低电平时间
			Delay1000ms();
			PwmChange(x,2000);//改变R2的pwm为2000
			UpDataTimingLeft();//更新剩余低电平时间
			Delay1000ms();
		}
		
    DoAction( forWordArr, 22 );//执行前进动作数组  66为forWordArr数组的长度
    while( 1 )
    {
        CheckAndDealActionDebug( );//检查并处理动作及调试指令
        CheckAndDealY( );//检查并处理语音指令
    }
}

