#include "stm32f4xx.h" // Device header

#include "ucos_ii.h"	//OS related
#include "os_trace.h"
#include "os_cpu.h"

#include "Serial.h"	//Hardware related
#include "Receiver.h"
#include "Motor.h"
#include "GY86.h"
#include "OLED.h"

#include "Anto.h"	//Software related
#include "Acc.h"
#include "Quaternion.h"
#include "PID.h"

//Task Priolity
#define MUTEX_IIC_PRIO                  (5)
#define STK_TASK_PRIO                  	(4)
#define GY86_TASK_PRIO                  (6)	
#define Anto_TASK_PRIO                  (7)
#define PID_TASK_PRIO                   (8)
#define OLED_TASK_PRIO                  (9)

//Stack Size
#define GY86_STK_SIZE                   (2048)
#define TASK_STK_SIZE 					(2048)
#define Anto_STK_SIZE                   (2048)
#define PID_STK_SIZE                    (2048)
#define OLED_STK_SIZE                   (2048)

//Task's Stack
OS_STK TASK_STK[TASK_STK_SIZE];
OS_STK GY86_TASK_STK[GY86_STK_SIZE];
OS_STK Anto_TASK_STK[Anto_STK_SIZE];
OS_STK PID_TASK_STK[PID_STK_SIZE];
OS_STK OLED_TASK_STK[OLED_STK_SIZE];

//Mutex
OS_EVENT* IICMutex;

gy_t gy86_data;//GY86 Sensor's Data

Vector3f_t offset_a, offset_m, offset_g;	//Bias Error

Vector3f_t scale_a, scale_m;		//Scale Error

Quaternion quaternion_data;	//Position Quaternion

float roll, pitch, yaw;	//Euler angle

float PPM_data[8];	//Receiver's 8 channel

float motor1, motor2, motor3, motor4;	//Motor

int flag = 0;	//counter for recording task running

//Task List
void Task_Start();

void Task_GY86();

void Task_Anto();

void Task_PID();

void Task_OLED();


int main(){
	OSInit();
	
	OS_TRACE_INIT();
	
	OS_CPU_SysTickInitFreq(84000000);

	OSTaskCreate(Task_Start,(void *)0,&TASK_STK[TASK_STK_SIZE-1],STK_TASK_PRIO);
	
	OSStart();
	
	return 0;
}


void Task_GY86(){
	
	INT8U err;
	
	while(1){
		OSMutexPend(IICMutex, 0, &err);
		
		IMU_Task(&gy86_data,&offset_a, &offset_m, &scale_a, &scale_m,&quaternion_data, &roll, &pitch , &yaw, &offset_g);
		
		OSMutexPost(IICMutex);	
		
		OSTimeDlyHMSM(0,0,0,5);
	};
}


void Task_Anto(){
	
	INT8U err;
	
	while(1){
		
		flag++;
		
		if(flag > 100){
			
			flag = 0;
			
		}
		
		OSMutexPend(IICMutex, 0, &err);
		
		ANO_Send03(roll,pitch,yaw,1);
		
		OSMutexPost(IICMutex);
		
		OSTimeDlyHMSM(0,0,0,5);
	}
		
}


void Task_PID(){
	INT8U err;
	
	
	float ppm_in[4];
	int i=0;
	float expectRoll,expectPicth,expectYaw;
	float expectThr; 	//油门
	
	
	while(1){
		//接收8个通道数据
		for(;i<4;i++){
			ppm_in[i] = PPM_data[i];
		}
		
		Expect_Angle_Calc(ppm_in,&expectRoll,&expectPicth, &expectYaw, &expectThr);
		
		OSMutexPend(IICMutex, 0, &err);
			
		Motor_Calc(gy86_data, pitch , roll ,yaw,expectRoll,expectPicth, expectYaw, expectThr, &motor1, &motor2, &motor3, &motor4);
		
		OSMutexPost(IICMutex);
		
		Motor_OUT(motor1, motor2, motor3, motor4);
		
		OSTimeDlyHMSM(0,0,0,5);
	}
}


void Task_OLED(){
	
	INT8U err;
	int i = 0;
	
	while(1){

		OSMutexPend(IICMutex, 0, &err);
		
//		OLED_ShowNum(1,7,flag,3);
		
//		OLED_ShowString(1,1,"motor");
		
		OLED_ShowNum(1,1,(PPM_data[0] - 316)/1.6,5);
		
		OLED_ShowNum(2,1,(PPM_data[1] - 316)/1.6,5);
		OLED_ShowNum(3,1,(PPM_data[2] - 316)/1.6,5);
		OLED_ShowNum(4,1,(PPM_data[3] - 316)/1.6,5);  
		
		OLED_ShowNum(1,7,(PPM_data[4] - 316)/1.6,5);
		
		OLED_ShowNum(2,7,(PPM_data[5] - 316)/1.6,5);
		OLED_ShowNum(3,7,(PPM_data[6] - 316)/1.6,5);
		OLED_ShowNum(4,7,(PPM_data[7] - 316)/1.6,5);
		
//		OLED_ShowFloat(2,1,motor1,8);
//		OLED_ShowFloat(3,1,motor2,8);
//		OLED_ShowFloat(4,1,motor3,8);

		
//		OLED_ShowFloat(2,7,motor1,3);
//		OLED_ShowFloat(3,7,motor2,3);
//		OLED_ShowFloat(4,7,motor3,3);
//		OLED_ShowFloat(5,7,motor4,3);
		
		OSMutexPost(IICMutex);
		
		OSTimeDlyHMSM(0,0,0,10);
	}
		
}


void Task_Start(){
	INT8U err;

	Serial_Init();

	GY86_Init();
	
	Quater_Init(&quaternion_data,  0.824, 0.080,  0.001,  -0.561);
	
	PID_Init();
	
	IC_Init();
	
	Motor_Init();

	OLED_Init();
	OLED_Clear();

	IMU_Correct(&offset_g, &offset_a, &offset_m, &scale_a, &scale_m);
	
	IICMutex  = OSMutexCreate(MUTEX_IIC_PRIO,&err);

	
	OSTaskCreate(Task_GY86,(void *)0,(OS_STK*)&GY86_TASK_STK[GY86_STK_SIZE-1],GY86_TASK_PRIO);
	
	OSTaskCreate(Task_Anto,(void *)0,(OS_STK*)&Anto_TASK_STK[Anto_STK_SIZE-1],Anto_TASK_PRIO);
	
	OSTaskCreate(Task_PID,(void *)0,(OS_STK*)&PID_TASK_STK[PID_STK_SIZE-1],PID_TASK_PRIO);
	
	OSTaskCreate(Task_OLED,(void *)0,(OS_STK*)&OLED_TASK_STK[OLED_STK_SIZE-1],OLED_TASK_PRIO);
	
	
	OSTaskNameSet(GY86_TASK_PRIO,(void *)"Task_GY86",NULL);
	
	OSTaskNameSet(Anto_TASK_PRIO,(void *)"Task_Anto",NULL);
	
	OSTaskNameSet(PID_TASK_PRIO,(void *)"Task_PID",NULL);
	
	OSTaskNameSet(OLED_TASK_PRIO,(void *)"Task_OLED",NULL);
	
	OSTaskDel(OS_PRIO_SELF);

}
