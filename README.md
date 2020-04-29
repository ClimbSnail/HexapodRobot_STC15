# HexapodRobot_STC15
HexapodRobot for STC15

此项目是本人在本科期间，基于STC15系列的MCU制作的六足机器人。

包含了硬件以及控制程序代码两个部分。
硬件部分附有详细的原理图及其对应的PCB文件。程序部分较为精简使之可靠得运行在低速MCU上。


可自行去[STC官网](http://www.stcmcu.com)下载[STC15](http://www.stcmcudata.com/datasheet/stc/STC-AD-PDF/STC15.pdf)系列的芯片手册

为了获得更快的运算效率，控制板使用的是22.1184MHz晶振。

#### PCB 3D预览图
![PCB_3D](https://github.com/ClimbSnail/HexapodRobot_STC15/blob/master/Robot_PCB_STC15W4K48S4_3D.jpg)

![PCB_3D](https://github.com/ClimbSnail/HexapodRobot_STC15/blob/master/Robot_PCB_STC15W4K48S43D.jpg)

#### 引脚定义:

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

	P40->Beep   P12->ADC  (采集电量)
	P30->RxD	P31->TxD  (串口1)
	P10->RxD2	P11->TxD2 (串口2)
	P41->DATA (温湿度传感器)
	P35->SCL	P34->SDA (IIC接口 可接屏幕)
	P55->LED  (驱动板上的LED指示灯)
	P32->INT0(可与板上的MOS0短接)	P33->INT1(可与板上的MOS1短接)
