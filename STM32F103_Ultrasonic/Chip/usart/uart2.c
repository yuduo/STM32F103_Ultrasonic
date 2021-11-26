#include "uart2.h"
#include <stdarg.h>
#include  <stdio.h>
#include  <string.h>

void UART2_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    // 1、串口时钟使能 GPIO时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    // 2、串口复位
    USART_DeInit(USART2);
    // 3、GPIO端口设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2 TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//PA3 RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // 4、串口参数初始化
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    // 5、初始化NVIC
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // 6、开启中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    // 7、使能串口
    USART_Cmd(USART2, ENABLE);

}

void USART2_IRQHandler(void)
{
    u8 res;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        res = USART_ReceiveData(USART2);
        USART_SendData(USART2, res);			//把接收到的数据发送出去
    }

}
//自定义串口2 的printf 函数
char UART2_TX_BUF[200];
void u2_printf(char* fmt, ...)					//无法列出传递函数的所有实参的类型和数目时,可以用省略号指定参数表
{
    u16 i, j;
    va_list ap;													//va_list 是一个字符指针，可以理解为指向当前参数的一个指针，取参必须通过这个指针进行。
    va_start(ap, fmt);									//va_start函数来获取参数列表中的参数，使用完毕后调用va_end()结束
    vsprintf((char*)UART2_TX_BUF, fmt, ap);	// 把生成的格式化的字符串存放在这里
    va_end(ap);
    i = strlen((const char*)UART2_TX_BUF);              //此次发送数据的长度
    for(j = 0; j < i; j++)                                                    //循环发送数据
    {
        while((USART2->SR & 0X40) == 0);                    //循环发送,直到发送完毕
        USART2->DR = UART2_TX_BUF[j];
    }
}


