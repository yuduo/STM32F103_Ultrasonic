#include "uart2.h"
#include <stdarg.h>
#include  <stdio.h>
#include  <string.h>

void UART2_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    // 1������ʱ��ʹ�� GPIOʱ��ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    // 2�����ڸ�λ
    USART_DeInit(USART2);
    // 3��GPIO�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2 TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//PA3 RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // 4�����ڲ�����ʼ��
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    // 5����ʼ��NVIC
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // 6�������ж�
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    // 7��ʹ�ܴ���
    USART_Cmd(USART2, ENABLE);

}

void USART2_IRQHandler(void)
{
    u8 res;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        res = USART_ReceiveData(USART2);
        USART_SendData(USART2, res);			//�ѽ��յ������ݷ��ͳ�ȥ
    }

}
//�Զ��崮��2 ��printf ����
char UART2_TX_BUF[200];
void u2_printf(char* fmt, ...)					//�޷��г����ݺ���������ʵ�ε����ͺ���Ŀʱ,������ʡ�Ժ�ָ��������
{
    u16 i, j;
    va_list ap;													//va_list ��һ���ַ�ָ�룬�������Ϊָ��ǰ������һ��ָ�룬ȡ�α���ͨ�����ָ����С�
    va_start(ap, fmt);									//va_start��������ȡ�����б��еĲ�����ʹ����Ϻ����va_end()����
    vsprintf((char*)UART2_TX_BUF, fmt, ap);	// �����ɵĸ�ʽ�����ַ������������
    va_end(ap);
    i = strlen((const char*)UART2_TX_BUF);              //�˴η������ݵĳ���
    for(j = 0; j < i; j++)                                                    //ѭ����������
    {
        while((USART2->SR & 0X40) == 0);                    //ѭ������,ֱ���������
        USART2->DR = UART2_TX_BUF[j];
    }
}


