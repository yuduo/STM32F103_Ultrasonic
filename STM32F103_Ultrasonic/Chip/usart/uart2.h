#ifndef __UART2_H
#define __UART2_H
#include "stm32f10x.h"
void UART2_Init(u32 bound);
void u2_printf(char* fmt,...);

#endif


