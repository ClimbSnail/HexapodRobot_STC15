#ifndef __PWM_H
#define __PWM_H

#include "stc15w.h"
#include "config.h"

extern void PwmChange(uchar pwmNumber, uint pwmDuty); //pwmNumber范围0~21 pwmDuty(1~2500)
extern void UpDataTimingLeft(void);                   //更新剩余低电平时间
extern void PwmCH0_Control(void);                     //pwm通道0 同时生成8路pwm信号 timingValueH timingValueL中前8个数据决定高电平时间
extern void PwmCH1_Control(void);                     //pwm通道1 同时生成8路pwm信号 timingValueH timingValueL中第9-16个数据决定高电平时间
extern void PwmCH2_Control(void);                     //pwm通道2 同时生成5路pwm信号 timingValueH timingValueL中第17-21个数据决定高电平时间

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

sbit L1 = P2 ^ 0;
sbit R1 = P0 ^ 0;
sbit L2 = P2 ^ 1;
sbit R2 = P0 ^ 1;
sbit L3 = P2 ^ 2;
sbit R3 = P0 ^ 2;
sbit L4 = P2 ^ 3;
sbit R4 = P0 ^ 3;
sbit L5 = P2 ^ 4;
sbit R5 = P0 ^ 4;
sbit L6 = P2 ^ 5;
sbit R6 = P0 ^ 5;
sbit L7 = P2 ^ 6;
sbit R7 = P0 ^ 6;
sbit L8 = P2 ^ 7;
sbit R8 = P0 ^ 7;
sbit L9 = P4 ^ 5;
sbit R9 = P4 ^ 6;
sbit H1 = P4 ^ 4;
sbit H2 = P4 ^ 3;
sbit H3 = P4 ^ 2;

//调换舵机口顺序
extern code uchar changeOrder[21];

#endif