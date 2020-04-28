#include "MPU6050.h"
#include "IIC.h"

//****************************************
//		MPU6050�ڲ��Ĵ�������
//****************************************
#define	SMPRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG				0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define	ACCEL_XOUT_H	0x3B	//���ٶȼ�X����ֵ�Ĵ���
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D	//���ٶȼ�y����ֵ�Ĵ���
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F	//���ٶȼ�z����ֵ�Ĵ���
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41	//�¶ȼ��ֵ�Ĵ���
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43	//������X����ֵ�Ĵ���
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45	//������y����ֵ�Ĵ���
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47	//������z����ֵ�Ĵ���
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	   //��Դ����1�Ĵ���
#define	WHO_AM_I			0x75	   //MPU6050���豸��ַ�Ĵ���

#define	MPU6050Address	0xD0	   //Ĭ��MPU6050���豸��ַ

//���ٶȸ�������ֵ
static int AccelXValue;
static int AccelYValue;
static int AccelZValue;
//�Ƕȸ�������ֵ
static int GyroXValue;
static int GyroYValue;
static int GyroZValue;
//�¶�ֵ
static int TempValue;


void InitMPU6050()//MPU6050��ʼ��
{
	WriteI2C(MPU6050Address,PWR_MGMT_1, 0x00);	//�������״̬
	WriteI2C(MPU6050Address,SMPRT_DIV, 0x07);
	WriteI2C(MPU6050Address,CONFIG, 0x06);
	WriteI2C(MPU6050Address,GYRO_CONFIG, 0x18);
	WriteI2C(MPU6050Address,ACCEL_CONFIG, 0x01);
}

int GetData(uchar REG_Address)//��ȡָ����ַ�µ�����
{
	char ValueH,ValueL;
	ValueH = ReadI2C(MPU6050Address,REG_Address);//��ȡ�߰�λ��ַ
	ValueL = ReadI2C(MPU6050Address,REG_Address+1);//��ȡ�Ͱ�λ����
	return (ValueH<<8)+ValueL;               //���صõ���ֵ
}

void UpDataAll()		//������������
{
	AccelXValue = GetData(ACCEL_XOUT_H);
	AccelYValue = GetData(ACCEL_YOUT_H);
	AccelZValue = GetData(ACCEL_ZOUT_H);
	
	GyroXValue = GetData(GYRO_XOUT_H);
	GyroYValue = GetData(GYRO_YOUT_H);
	GyroZValue = GetData(GYRO_ZOUT_H);
}	