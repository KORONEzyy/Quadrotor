#include "stm32f4xx.h"
#include <stdio.h>
#include <math.h>
#include "Acc.h"
#include "Delay.h"
#include "OLED.h"
#include "GY86.h"



#define LEARNING_RATE 0.001
#define MAX_ITERATIONS 7500

const float ACC_NORM = 1.0;
const float MAG_NORM = 0.5;


// 计算校准后的向量模
float vector_norm(Vector3f_t v) {
	
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	
}


float compute_error(Vector3f_t input, Vector3f_t offset, Vector3f_t scale, float target_norm) {
	
    Vector3f_t corrected;
	
    corrected.x = (input.x - offset.x) * scale.x;
    corrected.y = (input.y - offset.y) * scale.y;
    corrected.z = (input.z - offset.z) * scale.z;
	
    float norm = vector_norm(corrected);
	
    return (norm - target_norm) * (norm - target_norm);  // 平方误差
}


//对于每一次迭代
void Step_Grad(Vector3f_t* offset, Vector3f_t* scale,int dataSize,Vector3f_t Data[],int flag)
{	
	float grad_offset_x = 0, grad_offset_y = 0, grad_offset_z = 0;
	float grad_scale_x =0, grad_scale_y =0, grad_scale_z=0;
	
	float norm = ACC_NORM;
	
//	if(flag > 0){
////		grad_scale_x = 1, grad_scale_y = 1, grad_scale_z = 1;
//		norm = ACC_NORM;
//	}else if(flag < 0){
////	    grad_scale_x = 2, grad_scale_y = 2, grad_scale_z = 2;
//		norm = MAG_NORM;
//		
//	}
    
	for(int i=0;i<dataSize;i++){
		
		Vector3f_t mag = Data[i];
		
		Vector3f_t corrected_mag = {
				(mag.x - offset->x) * scale->x,
				(mag.y - offset->y) * scale->y,
				(mag.z - offset->z) * scale->z
		};
		
		float con_mag=vector_norm(corrected_mag);
		
		float mag_error = 2 * (con_mag - norm);//提取出公式中的公共部分
		
		Vector3f_t off_crt = {//仅对零偏误差校正，空间换时间
				mag.x - offset->x,
				mag.y - offset->y,
				mag.z - offset->z
		};
		
		grad_offset_x += mag_error *scale->x *scale->x * off_crt.x /con_mag;
		grad_offset_y += mag_error *scale->y *scale->y * off_crt.y /con_mag;
		grad_offset_z += mag_error *scale->z *scale->z * off_crt.z /con_mag;
		
		grad_scale_x += mag_error *scale->x * off_crt.x* off_crt.x /con_mag;
		grad_scale_y += mag_error *scale->y * off_crt.y* off_crt.y /con_mag;
	    grad_scale_z += mag_error *scale->z * off_crt.z* off_crt.z /con_mag;
	}
	
	offset->x -= LEARNING_RATE * grad_offset_x / dataSize;
	offset->y -= LEARNING_RATE * grad_offset_y / dataSize;
	offset->z -= LEARNING_RATE * grad_offset_z / dataSize;

	scale->x -= LEARNING_RATE * grad_scale_x / dataSize;
	scale->y -= LEARNING_RATE * grad_scale_y / dataSize;
	scale->z -= LEARNING_RATE * grad_scale_z / dataSize;
	
}


void gradientDescent(Vector3f_t accData[],Vector3f_t magData[],int dataSize,Vector3f_t* offset_a,Vector3f_t* scale_a,Vector3f_t* offset_m,Vector3f_t* scale_m)
{
	//设置初始值
	offset_a->x = 0,  offset_a->y = 0,  offset_a->z = 0;
    scale_a->x  = 1,  scale_a->y  = 1,  scale_a->z  = 1;
	offset_m->x = 0,  offset_m->y = 0,  offset_m->z = 0;
    scale_m->x  = 1,  scale_m->y  = 1,  scale_m->z  = 1;
	
	for(int i=0;i<MAX_ITERATIONS;i++){
		//得到新的scale和offset
		Step_Grad(offset_a, scale_a,dataSize,accData,1);
		
		Step_Grad(offset_m, scale_m,dataSize,magData,-1);
		
		//每1000次计算误差
		if (i % 1000 == 0){
			
			float total_error=0;
			
			for(int j = 0; j < dataSize; j++) {
				total_error += (compute_error(accData[i], *offset_a, *scale_a, ACC_NORM)
							+compute_error(magData[i], *offset_m, *scale_m, MAG_NORM))/2.0;
							
			}
		}
	}
}

void SO_Init(Vector3f_t * offset_a, Vector3f_t* offset_g ,Vector3f_t* offset_m, Vector3f_t* scale_a, Vector3f_t* scale_m){
	offset_a->x = 0.0f;
	offset_a->y = 0.0f;
	offset_a->z = 0.0f;
	
	offset_m->x = 0.0f;
	offset_m->y = 0.0f;
	offset_m->z = 0.0f;
	
	offset_g->x = 0.0f;
	offset_g->y = 0.0f;
	offset_g->z = 0.0f;
	
	scale_a->x = 1.0f;
	scale_a->y = 1.0f;
	scale_a->z = 1.0f;
	
	scale_m->x = 1.0f;
	scale_m->y = 1.0f;
	scale_m->z = 1.0f;
}
