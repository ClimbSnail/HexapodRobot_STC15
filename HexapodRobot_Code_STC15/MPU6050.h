#ifndef __MPU6050_H
#define __MPU6050_H

#include "stc15w.h"
#include "config.h"

//加速度各分量的值
static int AccelXValue;
static int AccelYValue;
static int AccelZValue;
//角度各分量的值
static int GyroXValue;
static int GyroYValue;
static int GyroZValue;
//温度值
static int TempValue;

extern void InitMPU6050();             //MPU6050初始化
extern int GetData(uchar REG_Address); //获取指定地址下的数据
extern void UpDataAll();               //更新所有数据

#endif