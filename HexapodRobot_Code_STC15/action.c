#include "action.h"
#include "delay.h"
#include "pwm.h"

//目标要到达的舵量
int targetPwm[21] = {1500,1500,1500,1500,1500,1500,1500,1500,1500,   1500,1500,1500,1500,1500,1500,1500,1500,1500,  1500,1500,1500 };

//记录当前的舵量
int nowPwm[21] = {1500,1500,1500,1500,1500,1500,1500,1500,1500,   1500,1500,1500,1500,1500,1500,1500,1500,1500,  1500,1500,1500 };

//每次定时中断时要增加的舵机量
int addPwm[21] = {0};

//舵机的安装误差
/********************************************************************************************
在为调整误差值前(即errorValue值全为0)，通过串口发送Debug指令(下面有介绍)来查看当前舵机安装与理想中
位状态的差。误差值的计算：对应位上的值=1500-使舵机达到中位的值。
最后计算得到的值挨个填入errorValue数组中。
********************************************************************************************/
int errorValue[21] = {0};


void UpData(void);//pwm数据更新
void CountAddPwm(int tempPwm[21],int frequency);//传入pwm以及减速倍率,定时器6开启时将自动缓慢生效

/************************************************************************
函数名称：	CountAddPwm(uint tempPwm[21],uint frequency)
功能描述： 计算pwm每次数据更新的量 frequency为减速倍率
入口参数： tempPwm为本次目标pwm值
返 回 值： none
其他说明： 已经在定时器4中使用更新
**************************************************************************/
void CountAddPwm(int tempPwm[21],int frequency)
{
    uchar i;
		uchar changedI;
    for( i = 0 ; i<21 ; i++ )
    {
        if( tempPwm[i]>0 )
        {
						changedI = changeOrder[i];	//调换顺序的作用
            targetPwm[changedI] = tempPwm[i]-errorValue[i];
            addPwm[changedI] = (targetPwm[changedI]-nowPwm[changedI])/frequency;
//            targetPwm[i] = tempPwm[i];
//            addPwm[i] = (targetPwm[i]-nowPwm[i])/frequency;
        }
    }
}

/************************************************************************
函数名称：	UpData(void)
功能描述： pwm数据更新
入口参数： none
返 回 值： none
其他说明： 已经在定时器4中使用更新
**************************************************************************/
void UpData(void)
{
    uchar pwmNumber;
    for( pwmNumber = 0 ; pwmNumber<21 ; pwmNumber++ )
    {
        if( addPwm[pwmNumber]>0 )
        {
            if( nowPwm[pwmNumber]<targetPwm[pwmNumber]-addPwm[pwmNumber] )
                nowPwm[pwmNumber] += addPwm[pwmNumber];
            else
            {
                nowPwm[pwmNumber] = targetPwm[pwmNumber];
                addPwm[pwmNumber] = 0;
            }

            PwmChange( pwmNumber, nowPwm[pwmNumber] );
        }
        if( addPwm[pwmNumber]<0 )
        {
            if( nowPwm[pwmNumber]>targetPwm[pwmNumber]-addPwm[pwmNumber] )
                nowPwm[pwmNumber] += addPwm[pwmNumber];
            else
            {
                nowPwm[pwmNumber] = targetPwm[pwmNumber];
                addPwm[pwmNumber] = 0;
            }
            PwmChange( pwmNumber, nowPwm[pwmNumber] );
        }
    }
//		for( pwmNumber = 0 ; pwmNumber<21 ; pwmNumber++ )
//            PwmChange( pwmNumber, nowPwm[pwmNumber] );
    UpDataTimingLeft();	//更新剩余低电平时间
}

/************************************************************************
函数名称：	DoAction(const uint *actionArr,uint length)
功能描述： 执行动作数组
入口参数： 指针型的actionArr length
返 回 值： none
其他说明： 该函数将传入的动作数组指针actionArr，以及actionArr的长度length(一定
						要是22的倍数 每组第25位为执行的时间间隔)有兴趣的可根据需要自行修改。
**************************************************************************/
void DoAction(const int *actionArr,int length) //执行动作数组函数
{
    uint i = 0;
		 for( i = 0 ; i<length ; i+=22 )	//不带动作中途中断功能
    {
      CountAddPwm(&actionArr[i],actionArr[i+21]/eachFrequencyTime);//50ms为定时器更新pwm的周期
			Delay50ms(actionArr[i+21]/eachFrequencyTime);////定时actionArr[i+22]/50个50ms 时间延缓 舵机减速控制的一部分(number最大为255)
    }
}

