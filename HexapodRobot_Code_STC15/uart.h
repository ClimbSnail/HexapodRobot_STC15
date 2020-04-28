#ifndef __UART_H
#define __UART_H

#include "stc15w.h"
#include "config.h"


//����1�Ĳ��ֶ���
/******************************************/
#define NONE_PARITY     0       //��У��
#define ODD_PARITY      1       //��У��
#define EVEN_PARITY     2       //żУ��
#define MARK_PARITY     3       //���У��
#define SPACE_PARITY    4       //�հ�У��

#define PARITYBIT NONE_PARITY   //����У��λ

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7
/******************************************/


//����2�Ĳ��ֶ���
/****************************************/
#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1
#define S2_S0 0x01              //P_SW2.0
/*****************************************/

typedef unsigned char BYTE;
typedef unsigned int WORD;

extern uchar receiveFlag1;  //���ձ�־
extern uchar message;	 //��Ϣ
extern void Uart1Init(void);//����1��ʼ�� �����ʵ�ֵ��ϵͳ����Ƶ�ʿ�����config.h������
extern void Uart2Init(void);//����2��ʼ�� �����ʵ�ֵ��ϵͳ����Ƶ�ʿ�����config.h������

extern void Uart1SendString(uchar *str); //����1�����ַ���
extern void Uart2SendString(uchar *str); //����2�����ַ���

extern void Uart1SendData(uchar dat);		  //����1�����ַ�
extern void Uart2SendData(uchar dat);		  //����2�����ַ�

extern void Uart1Send(uchar dat);		  //����1�����ַ�
extern void Uart2Send(uchar dat);		  //����2�����ַ�



//����ָ�����õ�������
/**********************************************************************************/
extern void AnalysisCom(void); 		//�������ݻ���
extern void ReceiveDataConvertToPwmValue();//��������ָ��


#define USART1_REC_LEN  			256  	//�����������ֽ��� 256
extern uchar  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART1_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uchar USART1_RX_STA;         		//����״̬���	
extern uchar usart1ReceiveSuccess;//һ֡���ݽ�����ɱ�־λ ʹ�ú�Ҫ��0


//�Զ���ĵڶ���USART1_RX_BUF������ ���ڲ�����������������
#define USART1_REC_LEN2  			10 	//�����������ֽ��� 10	  	
extern uchar  USART1_RX_BUF2[USART1_REC_LEN2]; //���ջ���,���USART1_REC_LEN2���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uchar USART1_RX_STA2;         		//����״̬���	
extern uchar usart1ReceiveSuccess2;//һ֡���ݽ�����ɱ�־λ ʹ�ú�Ҫ��0 

extern uchar firstdata;//��һ�ؽ��յ���ֵ
/**********************************************************************************/


#endif

