#include "uart.h"
#include "action.h"

uchar message1; //串口1的消息
uchar message2; //串口2的消息
uchar receiveFlag1 = 0;//接收标志

uchar busy1;	//串口1忙标志
uchar busy2;	//串口2忙标志

//解析指令所用到的数据
/**********************************************************************************/
//自定义的第一组USART1_RX_BUF缓冲区 用于缓存动作指令的数据 
uchar  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART1_REC_LEN个字节.末字节为换行符 
uchar USART1_RX_STA = 0;         		//接收状态标记	
uchar usart1ReceiveSuccess = 0;//一帧数据接收完成标志位 使用后要置0

//自定义的第二组USART1_RX_BUF2缓冲区 用于操作播放语音的数据  	
uchar  USART1_RX_BUF2[USART1_REC_LEN2]; //接收缓冲,最大USART1_REC_LEN2个字节.末字节为换行符 
uchar USART1_RX_STA2 = 0;         		//接收状态标记	
uchar usart1ReceiveSuccess2 = 0;//一帧数据接收完成标志位 使用后要置0 

uchar firstdata = 0;//第一回接收到的值
/**********************************************************************************/


/************************************************************************
函数名称：	AnalysisCom(void)    串口数据缓冲
功能描述： 接收数据  存在缓冲区USART1_RX_BUF或USART1_RX_BUF2中
入口参数： none
返 回 值： none
其他说明： 数据帧以0x0A 0x0D结尾（\r\n）
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
            if((USART1_RX_STA2&0x80)==0)//接收未完成
            {
                if(USART1_RX_STA2&0x4000)//接收到了0x0d
                {
                    if(message1!=0x0a)
                        USART1_RX_STA2=0;//接收错误,重新开始
                    else
                    {
                        USART1_RX_STA2|=0x8000;	//接收完成了
                        USART1_RX_STA2 = 0;//自己添加的 达到循环覆盖信息的作用
                        usart1ReceiveSuccess2 = 1;//接收一一帧完成标志位
                    }
                }
                else //还没收到0X0D
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
                            USART1_RX_STA2=0;//接收数据错误,重新开始接收
                    }
                }
            }
        }
        else
        {
            if((USART1_RX_STA&0x8000)==0)//接收未完成
            {
                if(USART1_RX_STA&0x4000)//接收到了0x0d
                {
                    if(message1!=0x0a)
                        USART1_RX_STA=0;//接收错误,重新开始
                    else
                    {
                        USART1_RX_STA|=0x8000;	//接收完成了
                        USART1_RX_STA = 0;//自己添加的 达到循环覆盖信息的作用
                        usart1ReceiveSuccess = 1;//接收一一帧完成标志位
                    }
                }
                else //还没收到0X0D
                {
                    if(message1==0x0d)USART1_RX_STA|=0x4000;
                    else
                    {
                        USART1_RX_BUF[USART1_RX_STA&0X3FFF] = message1 ;
                        USART1_RX_STA++;
                        if(USART1_RX_STA>(USART1_REC_LEN-1))
                            USART1_RX_STA=0;//接收数据错误,重新开始接收
                    }
                }
            }
        }
    }

}

/************************************************************************
函数名称：	ReceiveDataConvertToPwmValue()  解析动作指令
功能描述： 解析动作指令 将冲区USART1_RX_BUF中数据转为各路pwm值(500-2500)
入口参数： none
返 回 值： none
其他说明：
**************************************************************************/
void ReceiveDataConvertToPwmValue()
{
    short len = 1;//标识处理数据时的buffer指针
    short pwm[21];//存放当前动作组的pwm值
    short actionNum = 0;//存放动作组的个数值
    short executionTime;//存放当前动作组要执行的时间
    char flag;
    char i;

    //解析出帧头的数据(动作组的个数)
    while( USART1_RX_BUF[len]==' ' ) len++;

    while( USART1_RX_BUF[len]<58 && USART1_RX_BUF[len]>47 )
    {
        actionNum = actionNum*10+USART1_RX_BUF[len++]-48;
    }

    usart1ReceiveSuccess = 0;//标志位清零
    //解析actionNum次数据
    while( actionNum-- && !usart1ReceiveSuccess )//标志位重新被置一的话说明有新的指令 需停止当前动作
    {
        executionTime = 0;
        flag = 0;
        for( i = 0 ; i<21 ; i++)
        {
            pwm[i] = 0;
        }
        //解析动作数组的值
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
        //解析此组动作组要执行的时间
        while( USART1_RX_BUF[len]==' ' ) len++;
        while( USART1_RX_BUF[len]<58 && USART1_RX_BUF[len]>47 )
        {
            executionTime = executionTime*10+USART1_RX_BUF[len++]-48;
        }

        CountAddPwm(pwm,executionTime/eachFrequencyTime);//传入pwm以及减速倍率5,定时器6开启时将自动缓慢生效
//        delay_ms(executionTime);//等待动作结束
    }
}

/*----------------------------
UART1(串口1) 初始化函数
-----------------------------*/
void Uart1Init(void)
{
    P3 = 0x03;	//P30 P31如果清零 串口将不能用

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
//    SCON = 0x50;                //8位可变波特率
//#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
//    SCON = 0xda;                //9位可变波特率,校验位初始为1
//#elif (PARITYBIT == SPACE_PARITY)
//    SCON = 0xd2;                //9位可变波特率,校验位初始为0
//#endif


//    AUXR |= 0x40;		//定时器1时钟为fosc,即1T
//    AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
//    TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
//
//    TL1 = (65536 - (FOSC/4/BAUD1));   //设置波特率重装值
//    TH1 = (65536 - (FOSC/4/BAUD1))>>8;
//    ES = 1;      //使能串口中断
//    EA = 1;			//开总中断
//    ET1 = 0;		//禁止定时器1中断
//    TR1 = 1;		//启动定时器1


//	可以简化为如下代码
    SCON = 0x50;		//8位数据,可变波特率
    AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
    AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
    T2L = (65536 - (FOSC/4/BAUD1));		//设定定时初值
    T2H = (65536 - (FOSC/4/BAUD1))>>8;		//设定定时初值
    AUXR |= 0x10;		//启动定时器2
}

/*----------------------------
UART2(串口2) 初始化函数
-----------------------------*/
void Uart2Init(void)
{
    S2CON = 0x50;		//8位数据,可变波特率
    AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
    T2L = (65536 - (FOSC/4/BAUD1));		//设定定时初值
    T2H = (65536 - (FOSC/4/BAUD1))>>8;		//设定定时初值
    AUXR |= 0x10;		//启动定时器2
}

/*----------------------------
UART1(串口1) 中断服务程序
-----------------------------*/
void Uart1Interrupt() interrupt 4 using 1
{
    if (RI)
    {
        RI = 0;                 //清除RI位
        receiveFlag1 = 1;			//接收标志位置1
        message1 = SBUF;					//取出信息
				AnalysisCom();
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy1 = 0;               //清忙标志
    }
}


/*----------------------------
UART2(串口2) 中断服务函数
-----------------------------*/
void Uart2Interrupt() interrupt 8 using 1
{
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;         //清除S2RI位
        message2 = S2BUF;     		//取出信息
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //清除S2TI位
        busy2 = 0;               //清忙标志
    }
}

/*----------------------------
发送串口1数据(有校验的 差错检测)
----------------------------*/
void Uart1SendData(uchar dat)
{
    while (busy1);               //等待前面的数据发送完成
    ACC = dat;                  //获取校验位P (PSW.0)
    if (P)                      //根据P来设置校验位
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;                //设置校验位为0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;                //设置校验位为1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;                //设置校验位为1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;                //设置校验位为0
#endif
    }
    busy1 = 1;
    SBUF = ACC;                 //写数据到UART数据寄存器
}

/*----------------------------
发送串口2数据(有校验的 差错检测)
----------------------------*/
//在以下补充
void Uart2SendData(uchar dat)
{

}

/*----------------------------
串口1发送字符串
----------------------------*/
void Uart1SendString(uchar *str)
{
    while (*str)                  //检测字符串结束标志
    {
        Uart1Send(*str++);         //发送当前字符
    }
}


/*----------------------------
串口2发送字符串
----------------------------*/
void Uart2SendString(uchar *str)
{
    while (*str)                  //检测字符串结束标志
    {
        Uart2Send(*str++);         //发送当前字符
    }
}

/*----------------------------
串口1发送字符
----------------------------*/
void Uart1Send(uchar dat)
{
    SBUF = dat;
    while(!TI);		  //等待发送数据完成
    TI=0;			  //清除发送完成标志位
}

/*----------------------------
串口2发送字符
----------------------------*/
void Uart2Send(uchar dat)
{
    while(!busy2);		 //等待前面发送数据的完成
    S2BUF = dat;			//发送数据
    busy2 = 0;			  //清除忙标志位
}

