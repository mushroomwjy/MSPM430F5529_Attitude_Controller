#ifndef MPU6050_H
#define MPU6050_H

#include <msp430.h>
#include "parameter_type.h"
#define PI 3.14159265358979323846

//********Mpu6050�����У׼ֵ**************
#define MPU6050_ZERO_ACCELL 378
#define MPU6050_ZERO_GYRO 13
//*************����MPU6050�ڲ���ַ*******************
#define SMPLRT_DIV      0x19    //���8λ�޷���λ�������Ƶ���������ò���Ƶ�ʵļĴ�����������=�����ǵ������/��1+�üĴ���ֵ),��ͨ�˲���ʹ��ʱ���������Ϊ8k����֮1k��
#define CONFIG          0x1A    //���õ�ͨ�˲����ļĴ���
#define GYRO_CONFIG     0x1B    //��������Ƕȵ����Ҳ��Ժ�����
#define ACCEL_CONFIG    0x1C    //����������ٶȵ����Ҳ��Ժ�����
/***************���ٶȴ������Ĵ���******************/
#define ACCEL_XOUT_H            0x3B
#define ACCEL_XOUT_L            0x3C
#define ACCEL_XOUT      ACCEL_XOUT_H    // X���ȡ��ַ����λΪ��ʼλ
#define ACCEL_YOUT_H            0x3D
#define ACCEL_YOUT_L            0x3E
#define ACCEL_YOUT      ACCEL_YOUT_H    // Y���ȡ��ַ����λΪ��ʼλ
#define ACCEL_ZOUT_H            0x3F
#define ACCEL_ZOUT_L            0x40
#define ACCEL_ZOUT      ACCEL_ZOUT_H    // Z���ȡ��ַ����λΪ��ʼλ
/*****************�¶ȴ������Ĵ���****************/
#define TEMP_OUT_H      0x41
#define TEMP_OUT_L      0x42
#define TEMP_OUT          TEMP_OUT_H    // �¶ȴ�������ȡ��ַ����λΪ��ʼλ
//////////////////////////////////////
/////��������ؼĴ���//////////////////
//////////////////////////////////////
#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44
#define GYRO_XOUT        GYRO_XOUT_H    // ������X���ȡ��ַ����λΪ��ʼλ
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_YOUT        GYRO_YOUT_H    // ������Y���ȡ��ַ����λΪ��ʼλ
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48
#define GYRO_ZOUT        GYRO_ZOUT_H    // ������Z���ȡ��ַ����λΪ��ʼλ
///////////////////////////////////////
////////////�����Ĵ���/////////////////
//////////////////////////////////////
#define PWR_MGMT_1      0x6B    //��Դ����
#define WHO_AM_I        0x75    //6λiic��ַ�����һλA0����
//////////////////////////////////////
///iic��ַ����////////////////////////
/////////////////////////////////////
#define SlaveAddress    0xD0    //A0�ӵأ�����3.3v��0xD1

extern void ByteWrite6050(uint8 REG_Address,uint8 REG_data);
extern uint8 ByteRead6050(uint8 REG_Address);
extern int Get6050Data(uint8 REG_Address);
extern void InitMPU6050();
extern float Mpu6050AccelAngle(int8 dir);
extern float Mpu6050GyroAngle(int8 dir);

#endif
