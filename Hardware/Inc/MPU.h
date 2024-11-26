#ifndef __MPU_H_
#define __MPU_H_

typedef struct MPU_Data
{
	int16_t AccX;
	int16_t AccY;
	int16_t AccZ;	
	int16_t GyroX;
	int16_t GyroY;
	int16_t GyroZ;
}MPU_Data;

extern MPU_Data MPU_Data_Structure;

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
int16_t MPU6050_ReadTwoReg(uint8_t RegAddress);
void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(MPU_Data *MPU_Data_Structrue);
void MPU6050_ReadAllReg(uint8_t RegAddress, int16_t* data,uint8_t count);

#endif
