/**
  ******************************************************************************
  * @file    usart.c
  * @author  xiaoyuan
  * @version V2.0
  * @date    2018-1-28
  * @brief   USART模块初始化函数，提供了两种不同的方式支持printf函数，
  *          提供了中断接收函数
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "usart.h"


/**
  * @brief  初始化串口1
  * @param  baud：串口波特率，一般设置为115200
  * @retval None
  */
void usart1_init(uint32_t bound) 
{
 //GPIO????
  GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    //??USART2,GPIOA??//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //??????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.2
   
  //USART2_RX      GPIOA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.3  

  //USART2 NVIC ??
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //????3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ????
    NVIC_Init(&NVIC_InitStructure);    //??????????NVIC???//USART ?????

    USART_InitStructure.USART_BaudRate = bound;//?????
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
    USART_InitStructure.USART_Parity = USART_Parity_No;//??????
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //????

  USART_Init(USART2, &USART_InitStructure); //?????2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//????????
  USART_Cmd(USART2, ENABLE);                    //????2
}

/* 选择不同的方式支持printf函数，要包含stdio.h头文件 */
#ifdef USE_MICROLIB  //使用microLib的方法支持printf函数 

/**
  * @brief  重新定向c库函数printf到串口1，重新定向后可使用printf函数打印信息
  * @param  ch：要发送的字节数据
  * @param  *f
  * @retval ch
  */
int fputc(int ch, FILE *f)
{
  /* 发送一个字节数据到串口 */
  USART_SendData(USART2, (uint8_t) ch);
  
  /* 等待发送完毕 */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		

  return (ch);
}

/**
  * @brief  重新定向c库函数scanf到串口1，重新定向后可使用scanf、getchar函数
  * @param  *f
  * @retval 串口接收到的数据
  */
int fgetc(FILE *f)
{
  /* 等待串口输入数据 */
  while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

  return (int)USART_ReceiveData(USART2);
}

#else  //不使用microlib就可以支持printf函数  

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;

/* 定义_sys_exit()以避免使用半主机模式 */   
void _sys_exit(int x) 
{ 
	x = x; 
}

/**
  * @brief  重新定向c库函数printf到串口1，重新定向后可使用printf函数打印信息
  * @param  ch：要发送的字节数据
  * @param  *f
  * @retval ch
  */ 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
  
  USART_SendData(USART2,(uint8_t)ch);   

	return ch;
}

#endif  //USE_MICROLIB

#if EN_USART1_RX   //如果使能了接收

///**
//  * @brief  重新定向c库函数printf到串口1，重新定向后可使用printf函数打印信息
//  * @param  ch：要发送的字节数据
//  * @param  *f
//  * @retval ch
//  */ 
//void USART1_IRQHandler(void)                	//串口1中断服务程序
//{
//	uint8_t rec;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d //0x0a结尾)
//	{
//		rec = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据

//  } 
//} 

#endif

