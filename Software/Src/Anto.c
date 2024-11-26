#include "stm32f4xx.h"                  // Device header

uint8_t DataToSend[100];
void SendByte(int8_t byte){
	
	while (!(USART1->SR &(1 << 7))){
		//wait
	}
	USART1->DR = byte & 0x01ff;
}
void usart_SendArray(vu8 *array, uint16_t num)
{
    vu8 i;
 
  for(i=0; i<num; i++)
  {

     SendByte(array[i]);
  }
    /* ?????????? */
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
}

void ANO_Send01(int16_t acc_x,int16_t acc_y,int16_t acc_z,int16_t gyro_x,int16_t gyro_y,int16_t gyro_z)
	
{
	uint8_t cnt=0;

	DataToSend[cnt++]=0xAA;
	DataToSend[cnt++]=0xFF;
	DataToSend[cnt++]=0x01;
	DataToSend[cnt++]=0x0d; 
	
	DataToSend[cnt++]=acc_x>>8;
	DataToSend[cnt++]=acc_x&0xff;
	DataToSend[cnt++]=acc_y>>8;
	DataToSend[cnt++]=acc_y&0xff;
	DataToSend[cnt++]=acc_z>>8;
	DataToSend[cnt++]=acc_z&0xff;
	DataToSend[cnt++]=gyro_x>>8;
	DataToSend[cnt++]=gyro_x&0xff;
	DataToSend[cnt++]=gyro_y>>8;
	DataToSend[cnt++]=gyro_y&0xff;
	DataToSend[cnt++]=gyro_z>>8;
	DataToSend[cnt++]=gyro_z&0xff;
	DataToSend[cnt++]=0;
	
	uint8_t sum=0,acc=0;
	for(int i=0;i<cnt;i++){
		sum += DataToSend[i];
		acc += sum;
	}
	DataToSend[cnt++]=sum;
	DataToSend[cnt++]=acc;
	
	usart_SendArray(DataToSend,cnt);
	
}

void ANO_Send03(int16_t rol,int16_t pit,int16_t yaw,int8_t fusion)
{
	uint8_t cnt=0;

	DataToSend[cnt++]=0xAA;
	DataToSend[cnt++]=0xFF;
	DataToSend[cnt++]=0x03;
	DataToSend[cnt++]=7;
	
	DataToSend[cnt++]=rol>>8;
	DataToSend[cnt++]=rol&0xff;
	DataToSend[cnt++]=pit>>8;
	DataToSend[cnt++]=pit&0xff;
	DataToSend[cnt++]=yaw>>8;
	DataToSend[cnt++]=yaw&0xff;
	DataToSend[cnt++]=fusion;
	uint8_t sum=0,acc=0;
	for(uint8_t i=0;i<cnt;i++){
		sum += DataToSend[i];
		acc += sum;
	}
	DataToSend[cnt++]=sum;
	DataToSend[cnt++]=acc;
	
	usart_SendArray(DataToSend,cnt);
	
}

void ANO_Send04(int16_t v0,int16_t v1,int16_t v2,int16_t v3,uint8_t fusion)
{
	uint8_t cnt=0;

	DataToSend[cnt++]=0xAA;
	DataToSend[cnt++]=0xFF;
	DataToSend[cnt++]=0x04;
	DataToSend[cnt++]=0x09;
	
	DataToSend[cnt++]=v0>>8;
	DataToSend[cnt++]=v0&0xff;
	DataToSend[cnt++]=v1>>8;
	DataToSend[cnt++]=v1&0xff;
	DataToSend[cnt++]=v2>>8;
	DataToSend[cnt++]=v2&0xff;
	DataToSend[cnt++]=v3>>8;
	DataToSend[cnt++]=v3&0xff;
	DataToSend[cnt++]=fusion;
	uint8_t sum=0,acc=0;
	for(int i=0;i<cnt;i++){
		sum += DataToSend[i];
		acc += sum;
	}
	DataToSend[cnt++]=sum;
	DataToSend[cnt++]=acc;
	
	usart_SendArray(DataToSend,cnt);
}


