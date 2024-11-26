#ifndef  __SERIAL_H
#define  __SERIAL_H

#include "GY86.h"

void Serial_Init(void) ;

void Serial_SendString(char* String);

void Serial_SendNum(int Num);

void IfPrintInt(int a);

#endif

