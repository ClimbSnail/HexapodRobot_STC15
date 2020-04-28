#include "pwm.h"


//调换舵机口顺序
code uchar changeOrder[21] = { 0,1,2,3,4,5,6,7,20, 8,9,10,11,12,13,14,15,19, 18,17,16};

//21路PWM 定时值的高8位与低8位
static uchar timingValueH[24]= {0xF5,0xF5,0xF5,0xF5,0xF5,0xF5,0xF5,0xF5, 0xF5,0xF5,0xF5,0xF5,0xF5,0xF5,0xF5,0xF5, 0xF5,0xF5,0xF5,0xF5,0xF5, 0xC6,0xC6,0xA6};
static uchar timingValueL[24]= {0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33, 0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33, 0x33,0x33,0x33,0x33,0x33, 0x66,0x66,0x00};

//三组定时器使用的pwm计数值下标
static uchar PwmCH0_CountFlag = 0;
static uchar PwmCH1_CountFlag = 0;
static uchar PwmCH2_CountFlag = 0;

//低电平的剩余时间
static uint PwmCH0_TimingLeftValue = 0;
static uint PwmCH1_TimingLeftValue = 0;
static uint PwmCH2_TimingLeftValue = 0;


//引脚定义:
//P44->H1 P43->H2	P42->H3
//P20->L1 		P00->R1
//P21->L2 		P01->R2
//P22->L3 		P02->R3
//P23->L4 		P03->R4
//P24->L5 		P04->R5
//P25->L6 		P05->R6
//P26->L7 		P06->R7
//P27->L8 		P07->R8
//P45->L9 		P46->R9
//P4^4->H1 P4^3->H2  P4^2->H3

//pwm通道0 同时生成8路pwm信号 timingValueH timingValueL中前8个数据决定高电平时间
void PwmCH0_Control(void)
{
    if( PwmCH0_CountFlag == 8 )  //到达剩余低电平时间
    {
        P0 = 0x00;
        CH0_TH = timingValueH[21];
        CH0_TL = timingValueL[21];
    }
    else	 //定时pwmL1~8
    {
        if( PwmCH0_CountFlag == 0 )
            P0 = 0x01;
        else
            P0 <<= 1;
        CH0_TH = timingValueH[ PwmCH0_CountFlag ]; //定时时间赋值
        CH0_TL = timingValueL[ PwmCH0_CountFlag ];
    }
    PwmCH0_CountFlag = (PwmCH0_CountFlag+1)%9;	//下标推进
}

//pwm通道1 同时生成8路pwm信号 timingValueH timingValueL中第9-16个数据决定高电平时间
void PwmCH1_Control(void)
{
    if( PwmCH1_CountFlag == 8 )  //到达剩余低电平时间
    {
        P2 = 0x00;
        CH1_TH = timingValueH[22];
        CH1_TL = timingValueL[22];
    }
    else	 //定时pwmR1~8
    {
        if( PwmCH1_CountFlag == 0 )
            P2 = 0x01;
        else
            P2 <<= 1;
        CH1_TH = timingValueH[ PwmCH1_CountFlag+8 ]; //定时时间赋值
        CH1_TL = timingValueL[ PwmCH1_CountFlag+8 ];
    }
    PwmCH1_CountFlag = (PwmCH1_CountFlag+1)%9;	//下标推进
}

//pwm通道2 同时生成5路pwm信号 timingValueH timingValueL中第17-21个数据决定高电平时间
void PwmCH2_Control(void)
{
    if( PwmCH2_CountFlag == 5 )  //到达剩余低电平时间
    {
        P4 &= 0x83;
        CH2_TH = timingValueH[23];
        CH2_TL = timingValueL[23];
    }
    else	 //定时pwmH0~4
    {
        if( PwmCH1_CountFlag == 0 )
            P4 = P4&0x83|0x04;
        else
            P4 = P4&0x83|(0x04<<PwmCH2_CountFlag);
        CH2_TH = timingValueH[ PwmCH2_CountFlag+16 ]; //定时时间赋值
        CH2_TL = timingValueL[ PwmCH2_CountFlag+16 ];
    }
    PwmCH2_CountFlag = (PwmCH2_CountFlag+1)%6;	//下标推进
}

//通过指定高电平时间来改变timingValueH timingValueL中的值，达到改变对应pwm信号的高电平时间
void PwmChange(uchar pwmNumber,uint pwmDuty)	//pwmNumber范围0~20 pwmDuty(1~2500us)
{
    pwmDuty = 65536-(uint)pwmDuty*1.8432;//因为用的是22.118400MHz晶振  定时器12分频 1us有1.8432个节拍
//先改变定时时间的值
//使用位操作,运算速度更快
		pwmNumber = changeOrder[pwmNumber];	//调换顺序
    timingValueH[pwmNumber]	=  pwmDuty>>8;	//赋值此路pwm
    timingValueL[pwmNumber]	=  (pwmDuty<<8)>>8;	//赋值此路pwm
}

//更新剩余低电平时间
void UpDataTimingLeft()
{
    uchar count;
		
    //计算剩余低电平的剩余时间
    PwmCH0_TimingLeftValue = 28672;//20ms 需要大概36864个计数值 28672 = 65536-36864
    for( count = 0 ; count<8 ; count++ )
        PwmCH0_TimingLeftValue += ( 65536-(timingValueH[count]<<8|timingValueL[count]) );	//注意'|'符号不能用加代替 向下兼容，数据会变成8位
    timingValueH[21] = PwmCH0_TimingLeftValue>>8;	//更新剩余低电平时间
    timingValueL[21] = PwmCH0_TimingLeftValue;	//等价于 timingValueL[21] = (PwmCH0_TimingLeftValue<<8)>>8;

    PwmCH1_TimingLeftValue = 28672;//20ms 需要大概36864个计数值 28672 = 65536-36864
    for( count = 8 ; count<16 ; count++ )
        PwmCH1_TimingLeftValue += ( 65536-(timingValueH[count]<<8|timingValueL[count]) );	//注意'|'符号不能用加代替 向下兼容，数据会变成8位
    timingValueH[22] = PwmCH1_TimingLeftValue>>8;	//更新剩余低电平时间
    timingValueL[22] = PwmCH1_TimingLeftValue;	//等价于 timingValueL[22] = (PwmCH1_TimingLeftValue<<8)>>8;

    PwmCH2_TimingLeftValue = 28672;//20ms 需要大概36864个计数值 28672 = 65536-36864
    for( count = 16 ; count<21 ; count++ )
        PwmCH2_TimingLeftValue += ( 65536-(timingValueH[count]<<8|timingValueL[count]) );	//注意'|'符号不能用加代替 向下兼容，数据会变成8位
    timingValueH[23] = PwmCH2_TimingLeftValue>>8;	//更新剩余低电平时间
    timingValueL[23] = PwmCH2_TimingLeftValue;	//等价于 timingValueL[23] = (PwmCH2_TimingLeftValue<<8)>>8;
		
}
