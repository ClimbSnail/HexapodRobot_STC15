#ifndef __CONFIG_H
#define __CONFIG_H
//���̵������ļ�

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define ushort unsigned short


#define FOSC 22118400L          //ϵͳƵ��

//uart.c��ʹ��
#define BAUD1 115200L             //����1������

//XFS5152V2.c��ʹ��
#define XFS_PutChar Uart1Send	//����XFS_PutChar����ָ��������Ϣ���ĸ����ڷ��� UART1_send UART2_send UART3_send UART4_send UART5_send

#define eachFrequencyTime 50		//���������λʱ��(Ԥ��50ms �ɸ���)

//pwm.c�ļ���ʹ��
#define CH0_TH    TH0       //�����0ͨ����PWMʹ����һ����ʱ������ ͬʱ��pwmͨ�������źź�����ӽ���Ӧ��ʱ���жϺ���
#define CH0_TL    TL0
#define CH1_TH    TH1       //�����1ͨ����PWMʹ����һ����ʱ������ ͬʱ��pwmͨ�������źź�����ӽ���Ӧ��ʱ���жϺ���
#define CH1_TL    TL1
#define CH2_TH    T3H       //�����2ͨ����PWMʹ����һ����ʱ������ ͬʱ��pwmͨ�������źź�����ӽ���Ӧ��ʱ���жϺ���
#define CH2_TL    T3L

//LED�ƽӿ�
sbit LED = P5^5;

//����������
sbit BEEP = P4^0;

//ADC��ѹ�ɼ��ӿ�
sbit ADC = P1^2;

//���������Ŷ���
sbit DATA = P4^1;

//IIC�ӿ�
sbit I2C_SCL = P3^5;
sbit I2C_SDA = P3^4;

#endif

