/*****************************************************************
**  ���ƣ�YS-XFS5152�����ϳ����׳���
**  ˵�����û���ʹ�ó���ʱ��ֻ��Ҫ��д�� ���ڷ��ͺ���PrintCom()�����ɣ�
         ֱ�ӵ��ñ��ļ����в��ż��ɣ��ڱ�д����ʱ��ע���ж������ϳ�ģ
				 ��ķ���ֵ�����ݷ���ֵ������һ�β��ţ��粻���շ���ֵ���޷���
				 ����һ�β��š�
/*******************************************************************/

#include "XFS5152V2.h"
#include "uart.h"

/**************оƬ��������*********************/
code uchar XFS_StopCom[]= {0xFD,0X00,0X01,0X02}; //ֹͣ�ϳ�
code uchar XFS_SuspendCom[]= {0XFD,0X00,0X01,0X03}; //��ͣ�ϳ�
code uchar XFS_RecoverCom[]= {0XFD,0X00,0X01,0X04}; //�ָ��ϳ�
code uchar XFS_ChackCom[]= {0XFD,0X00,0X01,0X21}; //״̬��ѯ
code uchar XFS_PowerDownCom[]= {0XFD,0X00,0X01,0X88}; //����POWER DOWN ״̬����

/***********************************************/

//���������趨
code uchar YSpeed[11][6] = {"[s0]","[s1]","[s2]","[s3]","[s4]","[s5]","[s6]","[s7]","[s8]","[s9]","[s10]"};//���ٵ�������
code uchar YTone[11][6] = {"[t0]","[t1]","[t2]","[t3]","[t4]","[t5]","[t6]","[t7]","[t8]","[t9]","[t10]"};//�����������
code uchar YVolume[11][6] = {"[v0]","[v1]","[v2]","[v3]","[v4]","[v5]","[v6]","[v7]","[v8]","[v9]","[v10]"};//������������
code uchar YPeople[6][6] = {"[m3]","[m51]","[m52]","[m53]","[m54]","[m55]"};//������

static uchar YS = 5;//����ָ�� 0-10
static uchar YT = 5;//����ָ�� 0-10
static uchar YV = 8;//����ָ�� 0-10
static uchar YP = 1;//������ָ�� 0-5

/***********************************************************
* ��    �ƣ�  Strlen
* ��    �ܣ�  �����ַ�������
* ��ڲ�����  *DAT:�ı�ָ�����
* ���ڲ�����		
* ˵    ����
**********************************************************/
unsigned short Strlen(uchar *DAT)
{
    unsigned short length;
    for( length = 0; *DAT++ ; length++ );
    return length;
}

/***********************************************************
* ��    �ƣ�  Strcpy
* ��    �ܣ�  ����������
* ��ڲ�����  *info  *data:�ı�ָ�����
* ���ڲ�����		void
* ˵    ����
**********************************************************/
void Strcpy(uchar *info,uchar *dat)
{
    while( *dat )
    {
        *info++ = *dat++;
    }
    *info = 0;
}


/***********************************************************
* ��    �ƣ�  YS-XFS5152 �ı��ϳɺ���
* ��    �ܣ�  ���ͺϳ��ı���XFS5152оƬ���кϳɲ���
* ��ڲ�����  *data:�ı�ָ�����
* ���ڲ�����
* ˵    ���� ������ֻ�����ı��ϳɣ��߱���������ѡ��Ĭ�ϲ�����
						Ϊ115200bps(���ﲻ������)�� 
						Frame_Info[500]��������Ϊ500  �������һ���Է��͵�
						��Ϣ�ֽ�Ϊ450���ڡ�
* ���÷��������� SYN_FrameInfo
**********************************************************/
void XFS_FrameInfo(uchar *dat)
{
    /****************��Ҫ���͵��ı�**********************************/
    unsigned  char Frame_Info[500]= {1,1,1,1,1}; //������ı����� ǰ���һ��Ҫ��Ϊ��0�����洮����ʱ���㴮��Ҫ��
    unsigned  short length;
    unsigned  short pos;
    unsigned  short temp;

    //���������������ַ�
    temp = 5;
    Strcpy(&Frame_Info[temp],YSpeed[YS]);	//����Ϣ���м�������
    temp = Strlen(Frame_Info);
    Strcpy(&Frame_Info[temp],YTone[YT]);	//����Ϣ���м������
    temp = Strlen(Frame_Info);
    Strcpy(&Frame_Info[temp],YVolume[YV]);	//����Ϣ���м�������
    temp = Strlen(Frame_Info);
    Strcpy(&Frame_Info[temp],YPeople[YP]);	//����Ϣ���м��뷢����
    temp = Strlen(Frame_Info);

    length = Strlen(dat); 			//��Ҫ�����ı��ĳ���
    length = length+temp-3;								//�������������ܳ���
    /*****************֡�̶�������Ϣ**************************************/
    Frame_Info[0] = 0xFD ; 			//����֡ͷFD
    Frame_Info[1] = length>>8 ; 	//�������������ȵĸ��ֽ�
    Frame_Info[2] = length%256; 			//�������������ȵĵ��ֽ�
    Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������
    Frame_Info[4] = GBK;       //�ı������ʽ��GBK

    /*******************����֡��Ϣ***************************************/
    Strcpy(&Frame_Info[temp],dat);
    for( pos = 0 ; pos<length+3 ; pos++ )
    {
        XFS_PutChar(Frame_Info[pos]);//�ô��ڷ��� ���������ļ�config������XFS_PutChar����ָ���˿ڷ���
    }

}

/***********************************************************
* ��    �ƣ�  YSOrder ������������ִ�к���
* ��    �ܣ�  ����������������Լ�����
* ��ڲ�����  orderΪ�������  ��Χ0x00-0x07
* ���ڲ�����
* ˵    ����
* ���÷��������� �����������������м���������������ʱִ��
**********************************************************/
void YSOrder(unsigned char order)//������������
{
    unsigned  char temp[20];
    unsigned  char num[3];
    unsigned  short length;
    switch( order )
    {
    case 0x01:
        YS = (YS+1)%11;//���ټӿ�
        break;
    case 0x02:
        YS = (YS+11-1)%11;//���ټ���
        break;
    case 0x03:
        YT = (YT+1)%11;//�������
        break;
    case 0x04:
        YT = (YT+11-1)%11;//�������
        break;
    case 0x05:
        YV = (YV+1)%11;//������
        break;
    case 0x06:
        YV = (YV+11-1)%11;//������
        break;
    case 0x07:
        YP = (YP+1)%6;//��һ������
        XFS_FrameInfo("�����ڵ�������������");
        break;
    case 0x08://�ָ�Ĭ��ֵ
        YS = 5;//����ָ�� 0-10
        YT = 5;//����ָ�� 0-10
        YV = 5;//����ָ�� 0-10
        YP = 1;//������ָ�� 0-5
        XFS_FrameInfo("�����ѻָ�Ĭ������");
        break;
    default:
        break;
    }
    if( order == 0x01 || order == 0x02 )
    {
        if( YS == 10 )
        {
            num[0] = 49;
            num[1] = 48;
            num[2] = 0;
        }
        else
        {
            num[0] = YS+48;
            num[1] = 0;
        }
        Strcpy(temp,"�����ѵ���");
        length = Strlen(temp);
        Strcpy(&temp[length],num);
        length = Strlen(temp);
        Strcpy(&temp[length],"��");
        XFS_FrameInfo(temp);
    }
    else if( order == 0x03 || order == 0x04 )
    {
        if( YT == 10 )
        {
            num[0] = 49;
            num[1] = 48;
            num[2] = 0;
        }
        else
        {
            num[0] = YT+48;
            num[1] = 0;
        }
        Strcpy(temp,"����ѵ���");
        length = Strlen(temp);
        Strcpy(&temp[length],num);
        length = Strlen(temp);
        Strcpy(&temp[length],"��");
        XFS_FrameInfo(temp);
    }
    else if( order == 0x05 || order == 0x06 )
    {

        if( YV == 10 )
        {
            num[0] = 49;
            num[1] = 48;
            num[2] = 0;
        }
        else
        {
            num[0] = YV+48;
            num[1] = 0;
        }
        Strcpy(temp,"�����ѵ���");
        length = Strlen(temp);
        Strcpy(&temp[length],num);
        length = Strlen(temp);
        Strcpy(&temp[length],"��");
        XFS_FrameInfo(temp);
    }
}

