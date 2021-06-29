#include "timer.h"
#include "pwm.h"
#include "action.h"

uchar timer4Flag = 0;

//定时器0初始化
void Timer0Init_Mode1T(void)
{
    EA = 1;       //开启总中断
    ET0 = 1;      // 定时器0可用
    TMOD |= 0x01; //设置定时器0为工作方式1(16位计数器)
    //先初始赋值
    TH0 = 0xFA;
    TL0 = 0x24;
    TR0 = 1; //定时器0开始计数
}

void Timer0Init_Mode12T(void)
{
    AUXR &= 0x7F; //定时器时钟12T模式
    TMOD &= 0xF0; //设置定时器模式
    TMOD |= 0x01; //设置定时器0为工作方式1(16位计数器)
    TL0 = 0x00;   //设置定时初值
    TH0 = 0x70;   //设置定时初值
    TF0 = 0;      //清除TF0标志
    TR0 = 1;      //定时器0开始计时
    ET0 = 1;      //使能定时器0中断
    EA = 1;
}

//定时器1初始化
void Timer1Init_Mode1T(void)
{
    EA = 1;       //开启总中断
    ET1 = 1;      // 定时器1可用
    TMOD |= 0x10; //设置定时器0为工作方式1(16位计数器)
    //先初始赋值
    TH1 = 0xFA;
    TL1 = 0x24;
    TR1 = 1; //定时器1开始计数
}

void Timer1Init_Mode12T(void)
{
    AUXR &= 0xBF; //定时器时钟12T模式
    TMOD &= 0x0F; //设置定时器模式
    TL1 = 0x00;   //设置定时初值
    TH1 = 0x70;   //设置定时初值
    TF1 = 0;      //清除TF1标志
    TR1 = 1;      //定时器1开始计时
    EA = 1;       //开启总中断
    ET1 = 1;      // 定时器0可用
}

//定时器2初始化
void Timer2Init_Mode1T(void)
{
    EA = 1;       //开启总中断
    IE2 |= 0x04;  // 定时器2可用
    AUXR |= 0x04; //定时器时钟1T模式
    T2L = 0x9A;   //设置定时初值
    T2H = 0xA9;   //设置定时初值
    AUXR |= 0x10; //定时器2开始计时
}

void Timer2Init_Mode12T(void)
{
    EA = 1;       //开启总中断
    IE2 |= 0x04;  // 定时器2可用
    AUXR &= 0xFB; //定时器时钟12T模式
    T2L = 0xCD;   //设置定时初值
    T2H = 0xF8;   //设置定时初值
    AUXR |= 0x10; //定时器2开始计时
}

//定时器3初始化
void Timer3Init_Mode1T(void)
{
    EA = 1;        //开启总中断
    IE2 |= 0x20;   // 定时器3可用
    T4T3M |= 0x02; //定时器时钟1T模式
    T3L = 0x9A;    //设置定时初值
    T3H = 0xA9;    //设置定时初值
    T4T3M |= 0x08; //定时器3开始计时
}

void Timer3Init_Mode12T(void)
{
    EA = 1;        //开启总中断
    IE2 |= 0x20;   // 定时器3可用
    T4T3M &= 0xFD; //定时器时钟12T模式
    T3L = 0xCD;    //设置定时初值
    T3H = 0xF8;    //设置定时初值
    T4T3M |= 0x08; //定时器3开始计时
}

//定时器4初始化
void Timer4Init_Mode1T(void)
{
    EA = 1;        //开启总中断
    IE2 |= 0x40;   // 定时器4可用
    T4T3M |= 0x20; //定时器时钟1T模式
    T4L = 0x9A;    //设置定时初值
    T4H = 0xA9;    //设置定时初值
    T4T3M |= 0x80; //定时器4开始计时
}

void Timer4Init_Mode12T(void)
{
    EA = 1;        //开启总中断
    IE2 |= 0x40;   // 定时器4可用
    T4T3M &= 0xDF; //定时器时钟12T模式
    T4L = 0x00;    //设置定时初值 25ms
    T4H = 0x4C;    //设置定时初值	25ms
    T4T3M |= 0x80; //定时器4开始计时
}

//定时器中断0
void timer0(void) interrupt 1
{
    PwmCH0_Control();
    LED = !LED;
}

//定时器中断1
void timer1(void) interrupt 3
{
    PwmCH1_Control();
}

//定时器中断2
void timer2(void) interrupt 12
{
}

//定时器中断3
void timer3(void) interrupt 19
{
    PwmCH2_Control();
}

//定时器中断4
void timer4(void) interrupt 20
{
    timer4Flag = !timer4Flag;
    //confgi.h中配置的是50ms  所以要进两次中断才能达50ms
    T4L = 0x00; //设置定时初值 25ms
    T4H = 0x4C; //设置定时初值	25ms
    if (timer4Flag)
    {
        UpData();
    }
    else
    {
    }
}
