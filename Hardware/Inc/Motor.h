#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);

void Motor_SetCompare(uint16_t compare);

void Motor_OUT(float motor1, float motor2, float motor3, float motor4);

#endif

