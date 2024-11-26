#ifndef __PID_H
#define __PID_H

typedef struct PID_Struct{
	float e;
	float e_pre;
	float e_sum;
	float T;
	float Kp;
	float Ki;
	float Kd;
	float output;
}PID_t;

void Get_T(float * T);

void Single_PID_Init(PID_t* pid);

void PID_Init(void);

float PID_Calc(PID_t* inner, PID_t* outer, float outerError, float innerStatus);

void Motor_Calc(gy_t gy86_data, float pitch , float roll ,float yaw , float expectRoll, float expectPicth, float expectYaw, float expectThr, float *motor1,
	float * motor2, float *motor3, float *motor4);

void Expect_Angle_Calc(float *PWM_IN , float * roll, float *pitch, float *yaw, float *thr);
#endif