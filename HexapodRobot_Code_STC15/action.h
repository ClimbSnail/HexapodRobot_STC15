#ifndef __ACTION_H
#define __ACTION_H

#include "stc15w.h"
#include "config.h"

extern void CountAddPwm(int tempPwm[21],int frequency);//����pwm�Լ����ٱ���,��ʱ��6����ʱ���Զ�������Ч
extern void DoAction(const int *actionArr,int length); //ִ�ж������麯�� lengthΪ22�ı���
extern void UpData(void);//pwm���ݸ���


/************************************************************************
����Ϊ��������  22��Ϊһ����λ(һ������)
**************************************************************************/

#endif

