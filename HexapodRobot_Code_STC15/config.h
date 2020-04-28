#ifndef __CONFIG_H
#define __CONFIG_H
//工程的配置文件

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define ushort unsigned short


#define FOSC 22118400L          //系统频率

//uart.c中使用
#define BAUD1 115200L             //串口1波特率

//XFS5152V2.c中使用
#define XFS_PutChar Uart1Send	//配置XFS_PutChar宏来指定语音信息由哪个串口发送 UART1_send UART2_send UART3_send UART4_send UART5_send

#define eachFrequencyTime 50		//舵机减缓单位时间(预设50ms 可更改)

//pwm.c文件中使用
#define CH0_TH    TH0       //定义第0通道的PWM使用哪一个定时器控制 同时将pwm通道生成信号函数添加进对应定时器中断函数
#define CH0_TL    TL0
#define CH1_TH    TH1       //定义第1通道的PWM使用哪一个定时器控制 同时将pwm通道生成信号函数添加进对应定时器中断函数
#define CH1_TL    TL1
#define CH2_TH    T3H       //定义第2通道的PWM使用哪一个定时器控制 同时将pwm通道生成信号函数添加进对应定时器中断函数
#define CH2_TL    T3L

//LED灯接口
sbit LED = P5^5;

//蜂鸣器引脚
sbit BEEP = P4^0;

//ADC电压采集接口
sbit ADC = P1^2;

//超声波引脚定义
sbit DATA = P4^1;

//IIC接口
sbit I2C_SCL = P3^5;
sbit I2C_SDA = P3^4;

#endif

