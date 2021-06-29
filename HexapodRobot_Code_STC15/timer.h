#ifndef __TIMER_H
#define __TIMER_H

#include "stc15w.h"
#include "config.h"

extern void Timer0Init_Mode1T(void);  //定时器0初始化1T模式
extern void Timer0Init_Mode12T(void); //定时器0初始化12T模式
extern void Timer1Init_Mode1T(void);  //定时器1初始化1T模式
extern void Timer1Init_Mode12T(void); //定时器1初始化12T模式
extern void Timer2Init_Mode1T(void);  //定时器2初始化1T模式
extern void Timer2Init_Mode12T(void); //定时器2初始化12T模式
extern void Timer3Init_Mode1T(void);  //定时器3初始化1T模式
extern void Timer3Init_Mode12T(void); //定时器3初始化12T模式
extern void Timer4Init_Mode1T(void);  //定时器4初始化1T模式
extern void Timer4Init_Mode12T(void); //定时器4初始化12T模式

#endif
