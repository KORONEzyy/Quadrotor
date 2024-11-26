#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

const double tans = 65536 / 20000;
//a = ( PSC2 + 1 ) * ( ARR2 + 1 )/ ( PSC1 + 1 ) / ( ARR1 + 1 ) 
//a = 16 * 65536 / 16 / 20000


void PWM_Init(void)
{


//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC->APB1ENR |= (uint32_t)0x02;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC->AHB1ENR |= (uint32_t)0x02;
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3);
//	GPIOB->AFR[0] |= 2;
//	GPIOB->AFR[0] |= 1 << 5;
//	GPIOB->AFR[0] |= 1 << 17;
//	GPIOB->AFR[0] |= 1 << 21;

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;//PA6
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
//    GPIOB->MODER |= ((uint32_t)0x000A);
//    GPIOB->MODER |= ((uint32_t)0x000A << 8);

////	//GPIOB->MODER |= ((uint32_t)0x02 << 1);
////	GPIOB->MODER |= ((uint32_t)0x0002 << 4);
////	GPIOB->MODER |= ((uint32_t)0x0002 << 5);					//Set GPIOB_Pin_0/1/4/5 AF Mode: [1:0]->10
//	GPIOB->OTYPER |= (uint32_t)0x0000;						//Set Output Push-pull:Default 0
//	GPIOB->OSPEEDR |= ((uint32_t)0x000F);				
//   // GPIOB->OSPEEDR |= ((uint32_t)0x03 << 1);
//	GPIOB->OSPEEDR |= ((uint32_t)0x0003 << 4);
//	GPIOB->OSPEEDR |= ((uint32_t)0x0003 << 5);				//Set GPIOB_Pin_0/1/4/5 Speed High: 

//	TIM_InternalClockConfig(TIM3);
	TIM3->SMCR &=  (uint16_t)~0x0007;						//Slave mode disabled: Enable InternalClock
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 16 -1;	
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);  

//	TIM3->CR1 &= (uint16_t)(~(0x0070));						//Set CounterMode_Up, Edge-aligned Mode: [6:4]bit->000
//	TIM3->ARR = 20000 - 1;									//Set Period
//	TIM3->PSC = 16 -1;										//Set Prescaler
//	TIM3->EGR = 0x01;        								//Enable Update Generation
//	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		


//TIM3->CCMR2 &= 0;
//TIM3->CCMR1 &= 0;
//TIM3->CCER &= 0;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	

//	TIM3->CCER &= (uint16_t)~(0x0001 );
//	TIM3->CCMR1 &= (uint16_t)~(0x0070 );				//Clear bits
//	TIM3->CCMR1 &= (uint16_t)~(0x0003 );				//Set output mode
//	TIM3->CCMR1 |= (uint16_t)(0x0060 );					//Set PWM mode 1
//	TIM3->CCER &= (uint16_t)~(0x0002);					//Set active high
//	
//	TIM3->CCER &= (uint16_t)~(0x0010 );
//	TIM3->CCMR1 &= (uint16_t)~(0x7000 );				//Clear bits
//	TIM3->CCMR1 &= (uint16_t)~(0x0300 );				//Set output mode
//	TIM3->CCMR1 |= (uint16_t)(0x6000 );					//Set PWM mode 1
//	TIM3->CCER &= (uint16_t)~(0x0020);					//Set active high
//	
//	TIM3->CCER &= (uint16_t)~(0x0100 );
//	TIM3->CCMR2 &= (uint16_t)~(0x0070 );				//Clear bits
//	TIM3->CCMR2 &= (uint16_t)~(0x0003 );				//Set output mode
//	TIM3->CCMR2 |= (uint16_t)(0x0060 );					//Set PWM mode 1
//	TIM3->CCER &= (uint16_t)~(0x0200);					//Set active high
//	
//	TIM3->CCER &= (uint16_t)~(0x1000 );
//	TIM3->CCMR2 &= (uint16_t)~(0x7000 );				//Clear bits
//	TIM3->CCMR2 &= (uint16_t)~(0x0300 );				//Set output mode
//	TIM3->CCMR2 |= (uint16_t)(0x6000 );					//Set PWM mode 1
//	TIM3->CCER &= (uint16_t)~(0x2000);					//Set active high

//	TIM3->CCER |= (uint16_t)0x1111;	
//	TIM_Cmd(TIM3, ENABLE);
    TIM3->CR1 |= 0x0001;									//Enable TIM3
}



void PWM_SetCompare(uint16_t PPMdata )
{
	TIM3->CCR1 = PPMdata * tans;
	TIM3->CCR2 = PPMdata * tans;
	TIM3->CCR3 = PPMdata * tans;
	TIM3->CCR4 = PPMdata * tans;
}



