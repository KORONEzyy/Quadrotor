#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_it.h"
#include "misc.h"

#include "os_trace.h"
#include "os_cpu.h"
#include "OLED.h"

#include "math.h"
#include "Delay.h"

#define TIMIT 1

#define abs(x) (0-x)?(-x):x 

extern float PPM_data[8];

void IC_Init(void){
	RCC->APB2ENR |= (uint32_t)0x01; // ?a??TIM1米?那㊣?車
	RCC->AHB1ENR |= (uint32_t)0x01; // ?a??GPIOA米?那㊣?車

	GPIOA->MODER &= ~((uint32_t)0x03 << 16);
	GPIOA->MODER |= ((uint32_t)0x02 << 16);   // Set GPIOA_Pin_8 AF Mode [16]bit->0, [17]bit->1
	GPIOA->OSPEEDR |= ((uint32_t)0x03 << 16); // Set GPIOA_Pin_8 Speed High: [17:16]bit->11
	GPIOA->PUPDR &= ~((uint32_t)0x01 << 16);  // Set GPIOA_Pin_8 PuPd NOPULL: [17:16]bit->00

	GPIOA->AFR[1] &= ~0x0F;
	GPIOA->AFR[1] |= (uint32_t)0x01; // ??PA8?∩車??aTIM1

	TIM1->ARR = 65536 - 1; //Period
	TIM1->PSC = 84 - 1;//Prescaler		 
										// 1MHZ
	
	TIM1->CR1 &= ~(0x01 << 4);
	TIM1->CCMR1 |= 1;//CC1 input, IC1 -> TI1
	TIM1->CCMR1 &= ~(3 << 2);// no prescaler
	TIM1->CCMR1 &= ~(0xF << 4);//No filter

	TIM1->DIER |= 1 << 1;// Capture 1 interrupt enable

	//NVIC
	NVIC->IP[27] = 0xa0;
	NVIC->ISER[0] |= 1 << 27;

	TIM1->CCER |= 1;// Capture enabled
	TIM1->CR1 |= 1;//Counter enable
}



int i = 0; 

void TIM1_CC_IRQHandler(void){

#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0u;
#endif

	OS_ENTER_CRITICAL();
	OSIntEnter();// Tell uC/OS-II that we are starting an ISR
	OS_EXIT_CRITICAL();
	// receiver funtion here

	TIM1->CNT = 0;
	if(TIM1->CCR1 > 6000 || i == 8){
		 i = 0;
	}else{
		if (abs(PPM_data[i] - TIM1->CCR1) > 5){
			PPM_data[i] = TIM1->CCR1;
			i ++ ;
			if (PPM_data[i  - 1] >= 670){
				PPM_data[i  - 1] = 470;
			}
		}
	}
	OSIntExit();
}