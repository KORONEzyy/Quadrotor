#ifndef __ANTO_H
#define __ANTO_H

#include "GY86.h"

void ANO_Send01(int16_t acc_x,int16_t acc_y,int16_t acc_z,int16_t gyro_x,int16_t gyro_y,int16_t gyro_z);
void ANO_Send03(int16_t rol,int16_t pit,int16_t yaw,int8_t fusion);
void ANO_Send04(int16_t v0,int16_t v1,int16_t v2,int16_t v3,uint8_t fusion);


#endif

