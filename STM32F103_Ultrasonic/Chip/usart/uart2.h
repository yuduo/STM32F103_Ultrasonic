#ifndef __UART2_H
#define __UART2_H
#include "stm32f10x.h"
#define SEND_DATA_SIZE 10
extern char UART2_TX_BUF[SEND_DATA_SIZE];
void UART2_Init(u32 bound);
//void u2_printf(const char* fmt,...);
void SendData(void);
#endif


