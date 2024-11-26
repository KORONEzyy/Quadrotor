#ifndef  __USART_H
#define  __USART_H

void My_USART1_Init(void) ;

void USART_SendByte(uint8_t Byte);

void USART1_IRQHandler(void);

#endif
