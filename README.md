## 六足机器人控制器（多路舵机控制器）

此项目是本人在本科期间，基于STC15系列的MCU制作的六足机器人。

包含了硬件以及控制程序代码两个部分。

硬件部分附有详细的原理图及其对应的PCB文件。程序部分较为精简使之可靠得运行在低速MCU上。
可自行去 [STC官网](http://www.stcmcu.com) 下载 [STC15](http://www.stcmcudata.com/datasheet/stc/STC-AD-PDF/STC15.pdf) 系列的芯片手册

B站视频[链接](https://b23.tv/BV1ss411L7xp) https://b23.tv/BV1ss411L7xp

## 工程文件结构
* 21路舵机控制板Robot_PCB_STC15W4K48S4 _（控制器原理图及PCB图）_
* HexapodRobot_Code_STC15 _（控制器的51单片机代码）_
* 74HC573中文手册.pdf
* Robot.hex _（已编译生成的hex文件）_
* 其余的为参考图片

### 硬件篇
	1. 控制板工程文件使用Altium Designer 17绘制
	2. 使用的MCU具体为STC15W4K48S4
	3. 为了获得更快的运算效率，控制板使用的是22.1184MHz晶振。


##### PCB 3D预览图
![PCB_3D_1](https://github.com/ClimbSnail/HexapodRobot_STC15/blob/master/Robot_PCB_STC15W4K48S4_3D.jpg)

![PCB_3D_2](https://github.com/ClimbSnail/HexapodRobot_STC15/blob/master/Robot_PCB_STC15W4K48S43D.jpg)


### 软件篇
	1. 代码工程为Keil 4软件的工程。
	2. 使用定时器，采用分时控制的叠加算法，生成多路舵机可用的PWM信号。
	3. 可提供精度达10us的持续PWM信号。直接驱动模拟舵机，同样适用于数字PWM舵机。

## 引脚定义:

	P44->H1 P43->H2	P42->H3
	
	P20->L1 		P00->R1
	P21->L2 		P01->R2
	P22->L3 		P02->R3
	P23->L4 		P03->R4
	P24->L5 		P04->R5
	P25->L6 		P05->R6
	P26->L7 		P06->R7
	P27->L8 		P07->R8
	P45->L9 		P46->R9
	
	P40->Beep	P12->ADC  (采集电量)
	P30->RxD	P31->TxD  (串口1)
	P10->RxD2	P11->TxD2 (串口2)
	P41->DATA (温湿度传感器)
	P35->SCL	P34->SDA (IIC接口 可接屏幕)
	P55->LED  (驱动板上的LED指示灯)
	P32->INT0(可与板上的MOS0短接)	P33->INT1(可与板上的MOS1短接)


#### 未完成内容(当初给学弟们布置的任务)
1. 控制板没使用外部晶振，使用的是内部晶振。下载程序的勾选项见图示。
2. 大体的整体系统的使用后面会追加一个视频介绍，希望大家所作的作品能有一个强大的控制系统(把单片机内存写超了就厉害了)
3. 关于uart.c里的接收指令解析我来补(要是自己能补上也可以自行补充)
	1. ReceiveDataConvertToPwmValue();
	2. AnalysisCom(void);
	3. CheckAndDealY( void );
4. 程序里的oled.h oled.c以及IIC.c IIC.h还不能用(当然还涉及到MPU6050的兼容性) 大体代码都有了，可以调试。	
5. 以下需要你们独立补充完：
> DHT11.h
> DHT11.c
> ADC.h
> ADC.c
