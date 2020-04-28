#include "timer.h"
#include "pwm.h"
#include "action.h"

uchar timer4Flag = 0;

//��ʱ��0��ʼ��
void Timer0Init_Mode1T(void)
{
    EA = 1;	  //�������ж�
    ET0 = 1;	   // ��ʱ��0����
    TMOD |= 0x01;//���ö�ʱ��0Ϊ������ʽ1(16λ������)
    //�ȳ�ʼ��ֵ
    TH0 = 0xFA;
    TL0 = 0x24;
    TR0 = 1;//��ʱ��0��ʼ����
}

void Timer0Init_Mode12T(void)
{
    AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
    TMOD &= 0xF0;		//���ö�ʱ��ģʽ
    TMOD |= 0x01;//���ö�ʱ��0Ϊ������ʽ1(16λ������)
    TL0 = 0x00;		//���ö�ʱ��ֵ
    TH0 = 0x70;		//���ö�ʱ��ֵ
    TF0 = 0;		//���TF0��־
    TR0 = 1;       //��ʱ��0��ʼ��ʱ
    ET0 = 1;      //ʹ�ܶ�ʱ��0�ж�
    EA = 1;
}


//��ʱ��1��ʼ��
void Timer1Init_Mode1T(void)
{
    EA = 1;	  //�������ж�
    ET1 = 1;	   // ��ʱ��1����
    TMOD |= 0x10;//���ö�ʱ��0Ϊ������ʽ1(16λ������)
    //�ȳ�ʼ��ֵ
    TH1 = 0xFA;
    TL1 = 0x24;
    TR1 = 1;//��ʱ��1��ʼ����
}

void Timer1Init_Mode12T(void)
{
    AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
    TMOD &= 0x0F;		//���ö�ʱ��ģʽ
    TL1 = 0x00;		//���ö�ʱ��ֵ
    TH1 = 0x70;		//���ö�ʱ��ֵ
    TF1 = 0;		//���TF1��־
    TR1 = 1;		//��ʱ��1��ʼ��ʱ
    EA = 1;	  //�������ж�
    ET1 = 1;	   // ��ʱ��0����
}

//��ʱ��2��ʼ��
void Timer2Init_Mode1T(void)
{
    EA = 1;	  //�������ж�
    IE2 |= 0x04;	   // ��ʱ��2����
    AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
    T2L = 0x9A;		//���ö�ʱ��ֵ
    T2H = 0xA9;		//���ö�ʱ��ֵ
    AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
}

void Timer2Init_Mode12T(void)
{
    EA = 1;	  //�������ж�
    IE2 |= 0x04;	   // ��ʱ��2����
    AUXR &= 0xFB;		//��ʱ��ʱ��12Tģʽ
    T2L = 0xCD;		//���ö�ʱ��ֵ
    T2H = 0xF8;		//���ö�ʱ��ֵ
    AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
}

//��ʱ��3��ʼ��
void Timer3Init_Mode1T(void)
{
    EA = 1;	  //�������ж�
    IE2 |= 0x20;	   // ��ʱ��3����
    T4T3M |= 0x02;		//��ʱ��ʱ��1Tģʽ
    T3L = 0x9A;		//���ö�ʱ��ֵ
    T3H = 0xA9;		//���ö�ʱ��ֵ
    T4T3M |= 0x08;		//��ʱ��3��ʼ��ʱ
}

void Timer3Init_Mode12T(void)
{
    EA = 1;	  //�������ж�
    IE2 |= 0x20;	   // ��ʱ��3����
    T4T3M &= 0xFD;		//��ʱ��ʱ��12Tģʽ
    T3L = 0xCD;		//���ö�ʱ��ֵ
    T3H = 0xF8;		//���ö�ʱ��ֵ
    T4T3M |= 0x08;		//��ʱ��3��ʼ��ʱ
}

//��ʱ��4��ʼ��
void Timer4Init_Mode1T(void)
{
    EA = 1;	  //�������ж�
    IE2 |= 0x40;	   // ��ʱ��4����
    T4T3M |= 0x20;		//��ʱ��ʱ��1Tģʽ
    T4L = 0x9A;		//���ö�ʱ��ֵ
    T4H = 0xA9;		//���ö�ʱ��ֵ
    T4T3M |= 0x80;		//��ʱ��4��ʼ��ʱ
}

void Timer4Init_Mode12T(void)
{
    EA = 1;	  //�������ж�
    IE2 |= 0x40;	   // ��ʱ��4����
	T4T3M &= 0xDF;		//��ʱ��ʱ��12Tģʽ
	T4L = 0x00;		//���ö�ʱ��ֵ 25ms
	T4H = 0x4C;		//���ö�ʱ��ֵ	25ms
	T4T3M |= 0x80;		//��ʱ��4��ʼ��ʱ
}


//��ʱ���ж�0
void timer0 (void) interrupt 1
{
    PwmCH0_Control();
    LED = !LED;
}

//��ʱ���ж�1
void timer1 (void) interrupt 3 
{
    PwmCH1_Control();
}

//��ʱ���ж�2
void timer2 (void) interrupt 12
{
}

//��ʱ���ж�3
void timer3 (void) interrupt 19
{
    PwmCH2_Control();
}

//��ʱ���ж�4
void timer4 (void) interrupt 20
{
	timer4Flag = !timer4Flag;
	//confgi.h�����õ���50ms  ����Ҫ�������жϲ��ܴ�50ms
	T4L = 0x00;		//���ö�ʱ��ֵ 25ms
	T4H = 0x4C;		//���ö�ʱ��ֵ	25ms
	if( timer4Flag )
	{
		UpData();
	}
	else
	{
	}
}


