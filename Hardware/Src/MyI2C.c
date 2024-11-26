#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"
#include "Delay.h"

#define MyI2C_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))//PB8
#define MyI2C_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))//PB9
#define MyI2C_R_SDA()			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//READ


void MyI2C_Init(void)
{
	RCC->AHB1ENR |= 1 << 1;//GPIOB clock is enable
	
	GPIOB->MODER &= ~((3 << 16)|(3 << 18)); //return to 00
	GPIOB->MODER |= (1 << 16)|(1 << 18);//Gen out
	GPIOB->OTYPER |= ((1 << 8)|(1 << 9));//OD
	GPIOB->OSPEEDR |= ((1 << 17)|(1 << 19));// High speed
	GPIOB->PUPDR &= ~((3 << 16)|(3 << 18));//NO pull-up,pull down
	//PB1 and PB2 are set to be OD
	GPIOB->ODR |= 1 << 8;
	GPIOB->ODR |= 1 << 9;
	
	}

void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1); 
		if (MyI2C_R_SDA() == 1){
			Byte |= (0x80 >> i);
		}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
