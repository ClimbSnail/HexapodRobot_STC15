#ifndef __IIC_H
#define __IIC_H

#include "stc15w.h"
#include "config.h"

//--��config.h���Ѷ���IIC�ӿ�--//
//--����ʹ�õ�IO��--//
//sbit I2C_SCL = P3^5;
//sbit I2C_SDA = P3^4;

//--����ȫ�ֱ���--//
extern void I2C_Start();//��ʼ�ź�
extern void I2C_Stop();//��ֹ�ź�
extern void I2C_SendByte(uchar dat);//����һ���ֽ�
extern uchar I2C_ReadByte();//��ȡһ���ֽ�
extern void I2C_SendACK(bit ack);//����Ӧ���ź�
extern bit I2C_RecvACK();//����Ӧ���ź�
extern void WriteI2C(uchar SlaveAddress,uchar REG_Address,uchar REG_data);//ָ����ַ��д������
extern uchar ReadI2C(uchar SlaveAddress,uchar REG_Address);//��ȡָ����ַ�µ�����

#endif

