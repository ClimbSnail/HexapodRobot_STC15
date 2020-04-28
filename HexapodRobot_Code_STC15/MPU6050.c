#include "MPU6050.h"
#include "IIC.h"

//****************************************
//		MPU6050内部寄存器设置
//****************************************
#define	SMPRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG				0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B	//加速度计X轴检测值寄存器
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D	//加速度计y轴检测值寄存器
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F	//加速度计z轴检测值寄存器
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41	//温度检测值寄存器
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43	//陀螺仪X轴检测值寄存器
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45	//陀螺仪y轴检测值寄存器
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47	//陀螺仪z轴检测值寄存器
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	   //电源管理1寄存器
#define	WHO_AM_I			0x75	   //MPU6050的设备地址寄存器

#define	MPU6050Address	0xD0	   //默认MPU6050的设备地址

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


void InitMPU6050()//MPU6050初始化
{
	WriteI2C(MPU6050Address,PWR_MGMT_1, 0x00);	//解除休眠状态
	WriteI2C(MPU6050Address,SMPRT_DIV, 0x07);
	WriteI2C(MPU6050Address,CONFIG, 0x06);
	WriteI2C(MPU6050Address,GYRO_CONFIG, 0x18);
	WriteI2C(MPU6050Address,ACCEL_CONFIG, 0x01);
}

int GetData(uchar REG_Address)//获取指定地址下的数据
{
	char ValueH,ValueL;
	ValueH = ReadI2C(MPU6050Address,REG_Address);//获取高八位地址
	ValueL = ReadI2C(MPU6050Address,REG_Address+1);//获取低八位数据
	return (ValueH<<8)+ValueL;               //返回得到的值
}

void UpDataAll()		//更新所有数据
{
	AccelXValue = GetData(ACCEL_XOUT_H);
	AccelYValue = GetData(ACCEL_YOUT_H);
	AccelZValue = GetData(ACCEL_ZOUT_H);
	
	GyroXValue = GetData(GYRO_XOUT_H);
	GyroYValue = GetData(GYRO_YOUT_H);
	GyroZValue = GetData(GYRO_ZOUT_H);
}	