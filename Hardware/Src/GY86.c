#include "stm32f4xx.h"                  // Device header
#include "HMC5883L.h"
#include "MPU.h"
#include "Delay.h"
#include "GY86.h"
#include "Quaternion.h"
#include "Madgwick.h"
#include "Acc.h"

float length_a = 0.01f;
float length_m = 0.01f;

void GY86_Init(void){
	MPU6050_Init();
	HMC5883L_Init();
}


void GY86_GetData(gy_t * gy86_data){
	MPU_Data mpu_data;
	HMC_Data HMC_Data_Structure;
	
	MPU6050_GetData(&mpu_data);
	gy86_data->ax = mpu_data.AccX / 16384.0f;
	gy86_data->ay = mpu_data.AccY / 16384.0f;
	gy86_data->az = mpu_data.AccZ / 16384.0f;
	gy86_data->gx = mpu_data.GyroX / 16.384f;
	gy86_data->gy = mpu_data.GyroY / 16.384f;
	gy86_data->gz = mpu_data.GyroZ / 16.384f;
	
	HMC5883L_GetData(&HMC_Data_Structure);
	
	gy86_data->mx = HMC_Data_Structure.GSX / 1090.0f ;
	gy86_data->my = HMC_Data_Structure.GSY / 1090.0f ;
	gy86_data->mz = HMC_Data_Structure.GSZ / 1090.0f ;
	
}	


void IMU_Correct(Vector3f_t *offset_g,Vector3f_t *offset_a, Vector3f_t * offset_m,Vector3f_t * scale_a, Vector3f_t * scale_m){

	Vector3f_t inputData_a[6]; 
	Vector3f_t inputData_g[6];
	Vector3f_t inputData_m[6];
	
	gy_t exp;
	float a = 0, b = 0 , c = 0 , d = 0, e = 0, f = 0, g = 0, h = 0 , m = 0 ;
	int i = 0, j = 0,k = 0;
	
	for(;k<1000;k++){
		GY86_GetData(&exp);

			g += exp.gx;
			h += exp.gy;
			m += exp.gz;
		
	}
        offset_g->x = g / 500;
		offset_g->y = h / 500;
		offset_g->z = m / 500;	
	
	for(;i < 6; i ++){
		for(;j <10 ;j++){
			GY86_GetData(&exp);
			a += exp.ax;
			b += exp.ay;
			c += exp.az;
			 
			d += exp.mx;
			e += exp.my;
			f += exp.mz;	
		}
		
		inputData_a[i].x = a / 10;
		inputData_a[i].y = b / 10;
		inputData_a[i].z = c / 10;
		                 
		inputData_m[i].x = d / 10;
		inputData_m[i].y = e / 10;
		inputData_m[i].z = f / 10;	
	}
	
	int dataSize = sizeof(inputData_a) / sizeof(inputData_a[0]);
	
	gradientDescent(inputData_a, inputData_m, dataSize, offset_a, scale_a, offset_m, scale_m);

}

void IMU_Task(gy_t *gy86_data,Vector3f_t *offset_a, Vector3f_t * offset_m,Vector3f_t * scale_a, Vector3f_t * scale_m,Quaternion * quaternion_data,float *a, float *b, float *c, Vector3f_t * offset_g){
	//采集数据
	GY86_GetData(gy86_data);
	
	//校准加速度数据
	gy86_data->ax = scale_a->x * (gy86_data->ax - offset_a->x);
	gy86_data->ay = scale_a->y * (gy86_data->ay - offset_a->y);
	gy86_data->az = scale_a->z * (gy86_data->az - offset_a->z);

	gy86_data->gx = gy86_data->gx - offset_g->x;
	gy86_data->gy = gy86_data->gy - offset_g->y;
	gy86_data->gz = gy86_data->gz - offset_g->z;
	
	gy86_data->mx = scale_m->x * (gy86_data->mx - offset_m->x);
	gy86_data->my = scale_m->y * (gy86_data->my - offset_m->y);
	gy86_data->mz = scale_m->z * (gy86_data->mz - offset_m->z);

	//更新姿态
	MadgwickAHRSupdate(quaternion_data, gy86_data,a, b, c);
	
}

