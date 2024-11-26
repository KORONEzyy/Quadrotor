#include "stm32f4xx.h"                  // Device header
#include "PWM.h"

void Motor_Init(void)
{
	PWM_Init();
}

void Motor_SetCompare(uint16_t compare)
{
	PWM_SetCompare(compare);
}

void Motor_OUT(float motor1, float motor2, float motor3, float motor4){
    TIM3->CCR1 = motor2;
    TIM3->CCR2 = motor4;
    TIM3->CCR3 = motor3;
    TIM3->CCR4 = motor1;
}
