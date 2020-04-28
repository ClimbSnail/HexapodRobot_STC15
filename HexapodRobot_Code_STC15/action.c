#include "action.h"
#include "delay.h"
#include "pwm.h"

//Ŀ��Ҫ����Ķ���
int targetPwm[21] = {1500,1500,1500,1500,1500,1500,1500,1500,1500,   1500,1500,1500,1500,1500,1500,1500,1500,1500,  1500,1500,1500 };

//��¼��ǰ�Ķ���
int nowPwm[21] = {1500,1500,1500,1500,1500,1500,1500,1500,1500,   1500,1500,1500,1500,1500,1500,1500,1500,1500,  1500,1500,1500 };

//ÿ�ζ�ʱ�ж�ʱҪ���ӵĶ����
int addPwm[21] = {0};

//����İ�װ���
/********************************************************************************************
��Ϊ�������ֵǰ(��errorValueֵȫΪ0)��ͨ�����ڷ���Debugָ��(�����н���)���鿴��ǰ�����װ��������
λ״̬�Ĳ���ֵ�ļ��㣺��Ӧλ�ϵ�ֵ=1500-ʹ����ﵽ��λ��ֵ��
������õ���ֵ��������errorValue�����С�
********************************************************************************************/
int errorValue[21] = {0};


void UpData(void);//pwm���ݸ���
void CountAddPwm(int tempPwm[21],int frequency);//����pwm�Լ����ٱ���,��ʱ��6����ʱ���Զ�������Ч

/************************************************************************
�������ƣ�	CountAddPwm(uint tempPwm[21],uint frequency)
���������� ����pwmÿ�����ݸ��µ��� frequencyΪ���ٱ���
��ڲ����� tempPwmΪ����Ŀ��pwmֵ
�� �� ֵ�� none
����˵���� �Ѿ��ڶ�ʱ��4��ʹ�ø���
**************************************************************************/
void CountAddPwm(int tempPwm[21],int frequency)
{
    uchar i;
		uchar changedI;
    for( i = 0 ; i<21 ; i++ )
    {
        if( tempPwm[i]>0 )
        {
						changedI = changeOrder[i];	//����˳�������
            targetPwm[changedI] = tempPwm[i]-errorValue[i];
            addPwm[changedI] = (targetPwm[changedI]-nowPwm[changedI])/frequency;
//            targetPwm[i] = tempPwm[i];
//            addPwm[i] = (targetPwm[i]-nowPwm[i])/frequency;
        }
    }
}

/************************************************************************
�������ƣ�	UpData(void)
���������� pwm���ݸ���
��ڲ����� none
�� �� ֵ�� none
����˵���� �Ѿ��ڶ�ʱ��4��ʹ�ø���
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
    UpDataTimingLeft();	//����ʣ��͵�ƽʱ��
}

/************************************************************************
�������ƣ�	DoAction(const uint *actionArr,uint length)
���������� ִ�ж�������
��ڲ����� ָ���͵�actionArr length
�� �� ֵ�� none
����˵���� �ú���������Ķ�������ָ��actionArr���Լ�actionArr�ĳ���length(һ��
						Ҫ��22�ı��� ÿ���25λΪִ�е�ʱ����)����Ȥ�Ŀɸ�����Ҫ�����޸ġ�
**************************************************************************/
void DoAction(const int *actionArr,int length) //ִ�ж������麯��
{
    uint i = 0;
		 for( i = 0 ; i<length ; i+=22 )	//����������;�жϹ���
    {
      CountAddPwm(&actionArr[i],actionArr[i+21]/eachFrequencyTime);//50msΪ��ʱ������pwm������
			Delay50ms(actionArr[i+21]/eachFrequencyTime);////��ʱactionArr[i+22]/50��50ms ʱ���ӻ� ������ٿ��Ƶ�һ����(number���Ϊ255)
    }
}

