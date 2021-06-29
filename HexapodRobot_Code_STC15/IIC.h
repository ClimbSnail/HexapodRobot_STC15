#ifndef __IIC_H
#define __IIC_H

#include "stc15w.h"
#include "config.h"

//--在config.h中已定义IIC接口--//
//--定义使用的IO口--//
//sbit I2C_SCL = P3^5;
//sbit I2C_SDA = P3^4;

//--声明全局变量--//
extern void I2C_Start();                                                     //起始信号
extern void I2C_Stop();                                                      //终止信号
extern void I2C_SendByte(uchar dat);                                         //发送一个字节
extern uchar I2C_ReadByte();                                                 //读取一个字节
extern void I2C_SendACK(bit ack);                                            //发送应答信号
extern bit I2C_RecvACK();                                                    //接收应答信号
extern void WriteI2C(uchar SlaveAddress, uchar REG_Address, uchar REG_data); //指定地址下写入数据
extern uchar ReadI2C(uchar SlaveAddress, uchar REG_Address);                 //获取指定地址下的数据

#endif