/*****************************************************************
**  名称：YS-XFS5152语音合成配套程序
**  说明：用户在使用程序时，只需要编写好 串口发送函数PrintCom()；即可，
         直接调用本文件进行播放即可，在编写程序时，注意判断语音合成模
				 块的返回值，根据返回值进行下一次播放，如不接收返回值则无法进
				 行下一次播放。
/*******************************************************************/

#include "XFS5152V2.h"
#include "uart.h"

/**************芯片设置命令*********************/
code uchar XFS_StopCom[]= {0xFD,0X00,0X01,0X02}; //停止合成
code uchar XFS_SuspendCom[]= {0XFD,0X00,0X01,0X03}; //暂停合成
code uchar XFS_RecoverCom[]= {0XFD,0X00,0X01,0X04}; //恢复合成
code uchar XFS_ChackCom[]= {0XFD,0X00,0X01,0X21}; //状态查询
code uchar XFS_PowerDownCom[]= {0XFD,0X00,0X01,0X88}; //进入POWER DOWN 状态命令

/***********************************************/

//声音参数设定
code uchar YSpeed[11][6] = {"[s0]","[s1]","[s2]","[s3]","[s4]","[s5]","[s6]","[s7]","[s8]","[s9]","[s10]"};//语速调整参数
code uchar YTone[11][6] = {"[t0]","[t1]","[t2]","[t3]","[t4]","[t5]","[t6]","[t7]","[t8]","[t9]","[t10]"};//语调调整参数
code uchar YVolume[11][6] = {"[v0]","[v1]","[v2]","[v3]","[v4]","[v5]","[v6]","[v7]","[v8]","[v9]","[v10]"};//音量调整参数
code uchar YPeople[6][6] = {"[m3]","[m51]","[m52]","[m53]","[m54]","[m55]"};//发音人

static uchar YS = 5;//语速指针 0-10
static uchar YT = 5;//音调指针 0-10
static uchar YV = 8;//音量指针 0-10
static uchar YP = 1;//发音人指针 0-5

/***********************************************************
* 名    称：  Strlen
* 功    能：  计算字符串长度
* 入口参数：  *DAT:文本指针变量
* 出口参数：		
* 说    明：
**********************************************************/
unsigned short Strlen(uchar *DAT)
{
    unsigned short length;
    for( length = 0; *DAT++ ; length++ );
    return length;
}

/***********************************************************
* 名    称：  Strcpy
* 功    能：  串拷贝函数
* 入口参数：  *info  *data:文本指针变量
* 出口参数：		void
* 说    明：
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
* 名    称：  YS-XFS5152 文本合成函数
* 功    能：  发送合成文本到XFS5152芯片进行合成播放
* 入口参数：  *data:文本指针变量
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率
						为115200bps(这里不做限制)。 
						Frame_Info[500]长度设置为500  所以最多一次性发送的
						信息字节为450以内。
* 调用方法：例： SYN_FrameInfo
**********************************************************/
void XFS_FrameInfo(uchar *dat)
{
    /****************需要发送的文本**********************************/
    unsigned  char Frame_Info[500]= {1,1,1,1,1}; //定义的文本长度 前五个一定要设为非0，后面串连接时计算串长要用
    unsigned  short length;
    unsigned  short pos;
    unsigned  short temp;

    //连接声音参数的字符
    temp = 5;
    Strcpy(&Frame_Info[temp],YSpeed[YS]);	//在信息串中加入语速
    temp = Strlen(Frame_Info);
    Strcpy(&Frame_Info[temp],YTone[YT]);	//在信息串中加入语调
    temp = Strlen(Frame_Info);
    Strcpy(&Frame_Info[temp],YVolume[YV]);	//在信息串中加入音量
    temp = Strlen(Frame_Info);
    Strcpy(&Frame_Info[temp],YPeople[YP]);	//在信息串中加入发音人
    temp = Strlen(Frame_Info);

    length = Strlen(dat); 			//需要发送文本的长度
    length = length+temp-3;								//计算数据区的总长度
    /*****************帧固定配置信息**************************************/
    Frame_Info[0] = 0xFD ; 			//构造帧头FD
    Frame_Info[1] = length>>8 ; 	//构造数据区长度的高字节
    Frame_Info[2] = length%256; 			//构造数据区长度的低字节
    Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
    Frame_Info[4] = GBK;       //文本编码格式：GBK

    /*******************发送帧信息***************************************/
    Strcpy(&Frame_Info[temp],dat);
    for( pos = 0 ; pos<length+3 ; pos++ )
    {
        XFS_PutChar(Frame_Info[pos]);//用串口发送 可在配置文件config中配置XFS_PutChar宏来指定端口发送
    }

}

/***********************************************************
* 名    称：  YSOrder 声音设置命令执行函数
* 功    能：  设置语速语调音量以及人声
* 入口参数：  order为命令参数  范围0x00-0x07
* 出口参数：
* 说    明：
* 调用方法：例： 本程序中在主函数中检测语音设置命令到来时执行
**********************************************************/
void YSOrder(unsigned char order)//语音设置命令
{
    unsigned  char temp[20];
    unsigned  char num[3];
    unsigned  short length;
    switch( order )
    {
    case 0x01:
        YS = (YS+1)%11;//语速加快
        break;
    case 0x02:
        YS = (YS+11-1)%11;//语速减慢
        break;
    case 0x03:
        YT = (YT+1)%11;//语调提升
        break;
    case 0x04:
        YT = (YT+11-1)%11;//语调降低
        break;
    case 0x05:
        YV = (YV+1)%11;//音量加
        break;
    case 0x06:
        YV = (YV+11-1)%11;//音量减
        break;
    case 0x07:
        YP = (YP+1)%6;//换一种声音
        XFS_FrameInfo("我现在的声音是这样的");
        break;
    case 0x08://恢复默认值
        YS = 5;//语速指针 0-10
        YT = 5;//音调指针 0-10
        YV = 5;//音量指针 0-10
        YP = 1;//发音人指针 0-5
        XFS_FrameInfo("声音已恢复默认设置");
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
        Strcpy(temp,"语速已调到");
        length = Strlen(temp);
        Strcpy(&temp[length],num);
        length = Strlen(temp);
        Strcpy(&temp[length],"级");
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
        Strcpy(temp,"语调已调到");
        length = Strlen(temp);
        Strcpy(&temp[length],num);
        length = Strlen(temp);
        Strcpy(&temp[length],"级");
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
        Strcpy(temp,"音量已调到");
        length = Strlen(temp);
        Strcpy(&temp[length],num);
        length = Strlen(temp);
        Strcpy(&temp[length],"级");
        XFS_FrameInfo(temp);
    }
}

