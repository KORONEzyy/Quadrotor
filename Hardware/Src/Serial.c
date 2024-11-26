#include "stm32f4xx.h"                  // Device header
#include "USART.h"
#include "Delay.h"

#define delayTime 200

void Serial_Init(void)
{
	My_USART1_Init();
}


void Serial_SendString(char* String){
	int i = 0;
	while (String[i] != '\0'){
		USART_SendByte(String[i++]);
	}
}


void Serial_SendNum(int Num){
	int data ;
	data = Num/10;
	if (data != 0){
		Serial_SendNum(data);
	}
	USART_SendByte(Num%10+'0');
}

//用来输出带符号的整数
void IfPrintInt(int a)
{
	
	if (a > 0)
	{
		Serial_SendNum(a);
	}
	else
	{
		Serial_SendString("-");
		Serial_SendNum(a * -1);
	}
}
