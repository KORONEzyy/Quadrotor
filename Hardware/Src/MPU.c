#include "stm32f4xx.h"                  // Device header
#include "MyI2C.h"
#include "MPU.h"
#include "MPU6050Adress.h"

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define MPU6050_ADDRESS		0xD0


#define false 0
#define true 1
#define M_PI 3.14159265358979323846

int16_t MPU6050_data[6];
float mpu_bias[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

//MPU_Data MPU_Data_Structure;

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}


int16_t MPU6050_ReadTwoReg(uint8_t RegAddress)
{
	int16_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(0);	
	Data = (MyI2C_ReceiveByte())|(Data << 8);
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

void MPU6050_ReadAllReg(uint8_t RegAddress, int16_t* data,uint8_t count)
{
	int16_t Data;
	uint8_t i = 0;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	while(i < count)
	{
	  Data = MyI2C_ReceiveByte();
    MyI2C_SendAck(0);	
	  Data = (MyI2C_ReceiveByte())|(Data << 8);
		data[i] = Data;
		i++;
		if(i == (count - 1))
		{
	    MyI2C_SendAck(1);
		}
		else
		{
		  MyI2C_SendAck(0);
		}
		
	}
	MyI2C_Stop();
}

//void mpu_calib(){
//	/* 校准零偏误差
//	 * 取样500次，取平均值
//	 * 注意判断是否在静止状态，否则一直等待
//	 * 两次取样差值大于500，判断为运动*/
//	int16_t mpu_last[6] = {0, 0, 0, 0, 0, 0}, mpu_cur[6] = {0, 0, 0, 0, 0, 0};
//	uint8_t iter = 5, count = 100;
//	while(iter--){
//		float tem[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
//		int16_t mpu_abs[6] = {0, 0, 0, 0, 0, 0};
//		bool mpu_calib_flag = 1;
//		uint8_t i = 0;
//		while(mpu_calib_flag){
//			for(uint8_t j = 0; j < 6; j++){
//				mpu_abs[j] = 0;
//			}
//			if(i<count){ // 取样100次
//				mpu6050_getdata(MPU6050_data);
//				for(uint8_t j = 0; j < 6; j++){
//					mpu_cur[j] = MPU6050_data[j]; 
//				}
//				for(uint8_t j = 0; j < 6; j++){
//					mpu_abs[j] += mpu_cur[j] - mpu_last[j];				// 叠加差值，相当于将新数据与初始数据作差
//				}
//				for(uint8_t j = 0; j < 6; j++){
//					mpu_last[j] = mpu_cur[j];
//					tem[j] += (float)mpu_cur[j];
//				}
//				i++;
//			}
//			if(abs(mpu_abs[0])>500 || abs(mpu_abs[1])>500 || abs(mpu_abs[2]>500) || abs(mpu_abs[3])>500 
//				|| abs(mpu_abs[4])>500 || abs(mpu_abs[5]>500)){
//				// 检测到运动，重新开始采样
//				i = 0;
//				for(uint8_t j = 0; j < 6; j++){
//					tem[j] = 0.0f;
//					mpu_abs[j] = 0;
//				}
//			}else if(i == count){
//				mpu_calib_flag = 0;
//			}
//		}
//		for(uint8_t j = 0; j < 6; j++){
//			tem[j] *= 0.002f;
//			mpu_bias[j] += tem[j];
//		}
//		
//	}	
//}

void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x01);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x01);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	//陀螺仪 2000度每秒
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x00);	//加速度计 2g
	
	//config MPU6050's regester to hang the iic to the main bus
	MPU6050_WriteReg(MPU6050_USER_CTRL,0x00);
	MPU6050_WriteReg(MPU6050_INT_PIN_CFG,0x02);
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(MPU_Data *MPU_Data_Structrue)
{
	
	MPU_Data_Structrue->AccX = MPU6050_ReadTwoReg(MPU6050_ACCEL_XOUT_H);
	MPU_Data_Structrue->AccY = MPU6050_ReadTwoReg(MPU6050_ACCEL_YOUT_H);;
	MPU_Data_Structrue->AccZ = MPU6050_ReadTwoReg(MPU6050_ACCEL_ZOUT_H);
	MPU_Data_Structrue->GyroX = MPU6050_ReadTwoReg(MPU6050_GYRO_XOUT_H);
	MPU_Data_Structrue->GyroY = MPU6050_ReadTwoReg(MPU6050_GYRO_YOUT_H);
	MPU_Data_Structrue->GyroZ = MPU6050_ReadTwoReg(MPU6050_GYRO_ZOUT_H);


}
