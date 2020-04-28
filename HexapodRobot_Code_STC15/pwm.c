#include "pwm.h"


//���������˳��
code uchar changeOrder[21] = { 0,1,2,3,4,5,6,7,20, 8,9,10,11,12,13,14,15,19, 18,17,16};

//21·PWM ��ʱֵ�ĸ�8λ���8λ
static uchar timingValueH[24]= {0xF5,0xF5,0xF5,0xF5,0xF5,0xF5,0xF5,0xF5, 0xF5,0xF5,0xF5,0xF5,0xF5,0xF5,0xF5,0xF5, 0xF5,0xF5,0xF5,0xF5,0xF5, 0xC6,0xC6,0xA6};
static uchar timingValueL[24]= {0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33, 0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33, 0x33,0x33,0x33,0x33,0x33, 0x66,0x66,0x00};

//���鶨ʱ��ʹ�õ�pwm����ֵ�±�
static uchar PwmCH0_CountFlag = 0;
static uchar PwmCH1_CountFlag = 0;
static uchar PwmCH2_CountFlag = 0;

//�͵�ƽ��ʣ��ʱ��
static uint PwmCH0_TimingLeftValue = 0;
static uint PwmCH1_TimingLeftValue = 0;
static uint PwmCH2_TimingLeftValue = 0;


//���Ŷ���:
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

//pwmͨ��0 ͬʱ����8·pwm�ź� timingValueH timingValueL��ǰ8�����ݾ����ߵ�ƽʱ��
void PwmCH0_Control(void)
{
    if( PwmCH0_CountFlag == 8 )  //����ʣ��͵�ƽʱ��
    {
        P0 = 0x00;
        CH0_TH = timingValueH[21];
        CH0_TL = timingValueL[21];
    }
    else	 //��ʱpwmL1~8
    {
        if( PwmCH0_CountFlag == 0 )
            P0 = 0x01;
        else
            P0 <<= 1;
        CH0_TH = timingValueH[ PwmCH0_CountFlag ]; //��ʱʱ�丳ֵ
        CH0_TL = timingValueL[ PwmCH0_CountFlag ];
    }
    PwmCH0_CountFlag = (PwmCH0_CountFlag+1)%9;	//�±��ƽ�
}

//pwmͨ��1 ͬʱ����8·pwm�ź� timingValueH timingValueL�е�9-16�����ݾ����ߵ�ƽʱ��
void PwmCH1_Control(void)
{
    if( PwmCH1_CountFlag == 8 )  //����ʣ��͵�ƽʱ��
    {
        P2 = 0x00;
        CH1_TH = timingValueH[22];
        CH1_TL = timingValueL[22];
    }
    else	 //��ʱpwmR1~8
    {
        if( PwmCH1_CountFlag == 0 )
            P2 = 0x01;
        else
            P2 <<= 1;
        CH1_TH = timingValueH[ PwmCH1_CountFlag+8 ]; //��ʱʱ�丳ֵ
        CH1_TL = timingValueL[ PwmCH1_CountFlag+8 ];
    }
    PwmCH1_CountFlag = (PwmCH1_CountFlag+1)%9;	//�±��ƽ�
}

//pwmͨ��2 ͬʱ����5·pwm�ź� timingValueH timingValueL�е�17-21�����ݾ����ߵ�ƽʱ��
void PwmCH2_Control(void)
{
    if( PwmCH2_CountFlag == 5 )  //����ʣ��͵�ƽʱ��
    {
        P4 &= 0x83;
        CH2_TH = timingValueH[23];
        CH2_TL = timingValueL[23];
    }
    else	 //��ʱpwmH0~4
    {
        if( PwmCH1_CountFlag == 0 )
            P4 = P4&0x83|0x04;
        else
            P4 = P4&0x83|(0x04<<PwmCH2_CountFlag);
        CH2_TH = timingValueH[ PwmCH2_CountFlag+16 ]; //��ʱʱ�丳ֵ
        CH2_TL = timingValueL[ PwmCH2_CountFlag+16 ];
    }
    PwmCH2_CountFlag = (PwmCH2_CountFlag+1)%6;	//�±��ƽ�
}

//ͨ��ָ���ߵ�ƽʱ�����ı�timingValueH timingValueL�е�ֵ���ﵽ�ı��Ӧpwm�źŵĸߵ�ƽʱ��
void PwmChange(uchar pwmNumber,uint pwmDuty)	//pwmNumber��Χ0~20 pwmDuty(1~2500us)
{
    pwmDuty = 65536-(uint)pwmDuty*1.8432;//��Ϊ�õ���22.118400MHz����  ��ʱ��12��Ƶ 1us��1.8432������
//�ȸı䶨ʱʱ���ֵ
//ʹ��λ����,�����ٶȸ���
		pwmNumber = changeOrder[pwmNumber];	//����˳��
    timingValueH[pwmNumber]	=  pwmDuty>>8;	//��ֵ��·pwm
    timingValueL[pwmNumber]	=  (pwmDuty<<8)>>8;	//��ֵ��·pwm
}

//����ʣ��͵�ƽʱ��
void UpDataTimingLeft()
{
    uchar count;
		
    //����ʣ��͵�ƽ��ʣ��ʱ��
    PwmCH0_TimingLeftValue = 28672;//20ms ��Ҫ���36864������ֵ 28672 = 65536-36864
    for( count = 0 ; count<8 ; count++ )
        PwmCH0_TimingLeftValue += ( 65536-(timingValueH[count]<<8|timingValueL[count]) );	//ע��'|'���Ų����üӴ��� ���¼��ݣ����ݻ���8λ
    timingValueH[21] = PwmCH0_TimingLeftValue>>8;	//����ʣ��͵�ƽʱ��
    timingValueL[21] = PwmCH0_TimingLeftValue;	//�ȼ��� timingValueL[21] = (PwmCH0_TimingLeftValue<<8)>>8;

    PwmCH1_TimingLeftValue = 28672;//20ms ��Ҫ���36864������ֵ 28672 = 65536-36864
    for( count = 8 ; count<16 ; count++ )
        PwmCH1_TimingLeftValue += ( 65536-(timingValueH[count]<<8|timingValueL[count]) );	//ע��'|'���Ų����üӴ��� ���¼��ݣ����ݻ���8λ
    timingValueH[22] = PwmCH1_TimingLeftValue>>8;	//����ʣ��͵�ƽʱ��
    timingValueL[22] = PwmCH1_TimingLeftValue;	//�ȼ��� timingValueL[22] = (PwmCH1_TimingLeftValue<<8)>>8;

    PwmCH2_TimingLeftValue = 28672;//20ms ��Ҫ���36864������ֵ 28672 = 65536-36864
    for( count = 16 ; count<21 ; count++ )
        PwmCH2_TimingLeftValue += ( 65536-(timingValueH[count]<<8|timingValueL[count]) );	//ע��'|'���Ų����üӴ��� ���¼��ݣ����ݻ���8λ
    timingValueH[23] = PwmCH2_TimingLeftValue>>8;	//����ʣ��͵�ƽʱ��
    timingValueL[23] = PwmCH2_TimingLeftValue;	//�ȼ��� timingValueL[23] = (PwmCH2_TimingLeftValue<<8)>>8;
		
}
