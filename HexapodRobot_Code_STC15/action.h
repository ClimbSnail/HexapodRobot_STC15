#ifndef __ACTION_H
#define __ACTION_H

#include "stc15w.h"
#include "config.h"

extern void CountAddPwm(int tempPwm[21],int frequency);//传入pwm以及减速倍率,定时器6开启时将自动缓慢生效
extern void DoAction(const int *actionArr,int length); //执行动作数组函数 length为22的倍数
extern void UpData(void);//pwm数据更新


/************************************************************************
以下为动作数组  22个为一个单位(一个动作)
**************************************************************************/

#endif

