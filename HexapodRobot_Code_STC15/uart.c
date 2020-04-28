#include "uart.h"
#include "action.h"

uchar message1; //����1����Ϣ
uchar message2; //����2����Ϣ
uchar receiveFlag1 = 0;//���ձ�־

uchar busy1;	//����1æ��־
uchar busy2;	//����2æ��־

//����ָ�����õ�������
/**********************************************************************************/
//�Զ���ĵ�һ��USART1_RX_BUF������ ���ڻ��涯��ָ������� 
uchar  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART1_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
uchar USART1_RX_STA = 0;         		//����״̬���	
uchar usart1ReceiveSuccess = 0;//һ֡���ݽ�����ɱ�־λ ʹ�ú�Ҫ��0

//�Զ���ĵڶ���USART1_RX_BUF2������ ���ڲ�����������������  	
uchar  USART1_RX_BUF2[USART1_REC_LEN2]; //���ջ���,���USART1_REC_LEN2���ֽ�.ĩ�ֽ�Ϊ���з� 
uchar USART1_RX_STA2 = 0;         		//����״̬���	
uchar usart1ReceiveSuccess2 = 0;//һ֡���ݽ�����ɱ�־λ ʹ�ú�Ҫ��0 

uchar firstdata = 0;//��һ�ؽ��յ���ֵ
/**********************************************************************************/


/************************************************************************
�������ƣ�	AnalysisCom(void)    �������ݻ���
���������� ��������  ���ڻ�����USART1_RX_BUF��USART1_RX_BUF2��
��ڲ����� none
�� �� ֵ�� none
����˵���� ����֡��0x0A 0x0D��β��\r\n��
**************************************************************************/
void AnalysisCom(void)
{
    if( USART1_RX_STA == 0 )
    {
        firstdata = message1;
        USART1_RX_BUF[USART1_RX_STA++] = message1;
    }
    else if( USART1_RX_STA2 == 0 )
    {
        firstdata = message1;
        USART1_RX_BUF2[USART1_RX_STA2++] = message1;
    }
    else
    {
        if( firstdata == 'Y' )
        {
            if((USART1_RX_STA2&0x80)==0)//����δ���
            {
                if(USART1_RX_STA2&0x4000)//���յ���0x0d
                {
                    if(message1!=0x0a)
                        USART1_RX_STA2=0;//���մ���,���¿�ʼ
                    else
                    {
                        USART1_RX_STA2|=0x8000;	//���������
                        USART1_RX_STA2 = 0;//�Լ���ӵ� �ﵽѭ��������Ϣ������
                        usart1ReceiveSuccess2 = 1;//����һһ֡��ɱ�־λ
                    }
                }
                else //��û�յ�0X0D
                {
                    if(message1==0x0d)
                    {
                        USART1_RX_BUF2[USART1_RX_STA2] = 0x00;
                        USART1_RX_STA2|=0x4000;
                    }
                    else
                    {
                        USART1_RX_BUF2[USART1_RX_STA2&0X3FFF] = message1 ;
                        USART1_RX_STA2++;
                        if(USART1_RX_STA2>(USART1_REC_LEN2-1))
                            USART1_RX_STA2=0;//�������ݴ���,���¿�ʼ����
                    }
                }
            }
        }
        else
        {
            if((USART1_RX_STA&0x8000)==0)//����δ���
            {
                if(USART1_RX_STA&0x4000)//���յ���0x0d
                {
                    if(message1!=0x0a)
                        USART1_RX_STA=0;//���մ���,���¿�ʼ
                    else
                    {
                        USART1_RX_STA|=0x8000;	//���������
                        USART1_RX_STA = 0;//�Լ���ӵ� �ﵽѭ��������Ϣ������
                        usart1ReceiveSuccess = 1;//����һһ֡��ɱ�־λ
                    }
                }
                else //��û�յ�0X0D
                {
                    if(message1==0x0d)USART1_RX_STA|=0x4000;
                    else
                    {
                        USART1_RX_BUF[USART1_RX_STA&0X3FFF] = message1 ;
                        USART1_RX_STA++;
                        if(USART1_RX_STA>(USART1_REC_LEN-1))
                            USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����
                    }
                }
            }
        }
    }

}

/************************************************************************
�������ƣ�	ReceiveDataConvertToPwmValue()  ��������ָ��
���������� ��������ָ�� ������USART1_RX_BUF������תΪ��·pwmֵ(500-2500)
��ڲ����� none
�� �� ֵ�� none
����˵����
**************************************************************************/
void ReceiveDataConvertToPwmValue()
{
    short len = 1;//��ʶ��������ʱ��bufferָ��
    short pwm[21];//��ŵ�ǰ�������pwmֵ
    short actionNum = 0;//��Ŷ�����ĸ���ֵ
    short executionTime;//��ŵ�ǰ������Ҫִ�е�ʱ��
    char flag;
    char i;

    //������֡ͷ������(������ĸ���)
    while( USART1_RX_BUF[len]==' ' ) len++;

    while( USART1_RX_BUF[len]<58 && USART1_RX_BUF[len]>47 )
    {
        actionNum = actionNum*10+USART1_RX_BUF[len++]-48;
    }

    usart1ReceiveSuccess = 0;//��־λ����
    //����actionNum������
    while( actionNum-- && !usart1ReceiveSuccess )//��־λ���±���һ�Ļ�˵�����µ�ָ�� ��ֹͣ��ǰ����
    {
        executionTime = 0;
        flag = 0;
        for( i = 0 ; i<21 ; i++)
        {
            pwm[i] = 0;
        }
        //�������������ֵ
        while( flag<21 )
        {
            while( USART1_RX_BUF[len++]!=' ' );
            while( USART1_RX_BUF[len]<58 && USART1_RX_BUF[len]>47 )
            {
                pwm[flag] = pwm[flag]*10+USART1_RX_BUF[len++]-48;
            }
            if(  USART1_RX_BUF[len-1]<58 && USART1_RX_BUF[len-1]>47  )
                flag++;
        }
        //�������鶯����Ҫִ�е�ʱ��
        while( USART1_RX_BUF[len]==' ' ) len++;
        while( USART1_RX_BUF[len]<58 && USART1_RX_BUF[len]>47 )
        {
            executionTime = executionTime*10+USART1_RX_BUF[len++]-48;
        }

        CountAddPwm(pwm,executionTime/eachFrequencyTime);//����pwm�Լ����ٱ���5,��ʱ��6����ʱ���Զ�������Ч
//        delay_ms(executionTime);//�ȴ���������
    }
}

/*----------------------------
UART1(����1) ��ʼ������
-----------------------------*/
void Uart1Init(void)
{
    P3 = 0x03;	//P30 P31������� ���ڽ�������

//    ACC = P_SW1;
//    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
//    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)

////  ACC = P_SW1;
////  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=1 S1_S1=0
////  ACC |= S1_S0;               //(P3.6/RxD_2, P3.7/TxD_2)
////  P_SW1 = ACC;
////
////  ACC = P_SW1;
////  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=1
////  ACC |= S1_S1;               //(P1.6/RxD_3, P1.7/TxD_3)
////  P_SW1 = ACC;

//#if (PARITYBIT == NONE_PARITY)
//    SCON = 0x50;                //8λ�ɱ䲨����
//#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
//    SCON = 0xda;                //9λ�ɱ䲨����,У��λ��ʼΪ1
//#elif (PARITYBIT == SPACE_PARITY)
//    SCON = 0xd2;                //9λ�ɱ䲨����,У��λ��ʼΪ0
//#endif


//    AUXR |= 0x40;		//��ʱ��1ʱ��Ϊfosc,��1T
//    AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
//    TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
//
//    TL1 = (65536 - (FOSC/4/BAUD1));   //���ò�������װֵ
//    TH1 = (65536 - (FOSC/4/BAUD1))>>8;
//    ES = 1;      //ʹ�ܴ����ж�
//    EA = 1;			//�����ж�
//    ET1 = 0;		//��ֹ��ʱ��1�ж�
//    TR1 = 1;		//������ʱ��1


//	���Լ�Ϊ���´���
    SCON = 0x50;		//8λ����,�ɱ䲨����
    AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
    AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
    T2L = (65536 - (FOSC/4/BAUD1));		//�趨��ʱ��ֵ
    T2H = (65536 - (FOSC/4/BAUD1))>>8;		//�趨��ʱ��ֵ
    AUXR |= 0x10;		//������ʱ��2
}

/*----------------------------
UART2(����2) ��ʼ������
-----------------------------*/
void Uart2Init(void)
{
    S2CON = 0x50;		//8λ����,�ɱ䲨����
    AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
    T2L = (65536 - (FOSC/4/BAUD1));		//�趨��ʱ��ֵ
    T2H = (65536 - (FOSC/4/BAUD1))>>8;		//�趨��ʱ��ֵ
    AUXR |= 0x10;		//������ʱ��2
}

/*----------------------------
UART1(����1) �жϷ������
-----------------------------*/
void Uart1Interrupt() interrupt 4 using 1
{
    if (RI)
    {
        RI = 0;                 //���RIλ
        receiveFlag1 = 1;			//���ձ�־λ��1
        message1 = SBUF;					//ȡ����Ϣ
				AnalysisCom();
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
        busy1 = 0;               //��æ��־
    }
}


/*----------------------------
UART2(����2) �жϷ�����
-----------------------------*/
void Uart2Interrupt() interrupt 8 using 1
{
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;         //���S2RIλ
        message2 = S2BUF;     		//ȡ����Ϣ
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //���S2TIλ
        busy2 = 0;               //��æ��־
    }
}

/*----------------------------
���ʹ���1����(��У��� �����)
----------------------------*/
void Uart1SendData(uchar dat)
{
    while (busy1);               //�ȴ�ǰ������ݷ������
    ACC = dat;                  //��ȡУ��λP (PSW.0)
    if (P)                      //����P������У��λ
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;                //����У��λΪ0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;                //����У��λΪ1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;                //����У��λΪ1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;                //����У��λΪ0
#endif
    }
    busy1 = 1;
    SBUF = ACC;                 //д���ݵ�UART���ݼĴ���
}

/*----------------------------
���ʹ���2����(��У��� �����)
----------------------------*/
//�����²���
void Uart2SendData(uchar dat)
{

}

/*----------------------------
����1�����ַ���
----------------------------*/
void Uart1SendString(uchar *str)
{
    while (*str)                  //����ַ���������־
    {
        Uart1Send(*str++);         //���͵�ǰ�ַ�
    }
}


/*----------------------------
����2�����ַ���
----------------------------*/
void Uart2SendString(uchar *str)
{
    while (*str)                  //����ַ���������־
    {
        Uart2Send(*str++);         //���͵�ǰ�ַ�
    }
}

/*----------------------------
����1�����ַ�
----------------------------*/
void Uart1Send(uchar dat)
{
    SBUF = dat;
    while(!TI);		  //�ȴ������������
    TI=0;			  //���������ɱ�־λ
}

/*----------------------------
����2�����ַ�
----------------------------*/
void Uart2Send(uchar dat)
{
    while(!busy2);		 //�ȴ�ǰ�淢�����ݵ����
    S2BUF = dat;			//��������
    busy2 = 0;			  //���æ��־λ
}

