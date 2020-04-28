#ifndef __ADC_H
#define __ADC_H

#include "stc15w.h"
#include "config.h"


//ADC引脚为P1^2		在config.h中已经定义了ADC
extern void ADC_init();//AD采集的初始化
extern uint ADValue;//用来存AD采集完的数值

#endif