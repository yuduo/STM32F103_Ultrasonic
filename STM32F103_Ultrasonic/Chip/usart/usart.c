/**
  ******************************************************************************
  * @file    usart.c
  * @author  xiaoyuan
  * @version V2.0
  * @date    2018-1-28
  * @brief   USARTģ���ʼ���������ṩ�����ֲ�ͬ�ķ�ʽ֧��printf������
  *          �ṩ���жϽ��պ���
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "usart.h"


/**
  * @brief  ��ʼ������1
  * @param  baud�����ڲ����ʣ�һ������Ϊ115200
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

/* ѡ��ͬ�ķ�ʽ֧��printf������Ҫ����stdio.hͷ�ļ� */
#ifdef USE_MICROLIB  //ʹ��microLib�ķ���֧��printf���� 

/**
  * @brief  ���¶���c�⺯��printf������1�����¶�����ʹ��printf������ӡ��Ϣ
  * @param  ch��Ҫ���͵��ֽ�����
  * @param  *f
  * @retval ch
  */
int fputc(int ch, FILE *f)
{
  /* ����һ���ֽ����ݵ����� */
  USART_SendData(USART2, (uint8_t) ch);
  
  /* �ȴ�������� */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		

  return (ch);
}

/**
  * @brief  ���¶���c�⺯��scanf������1�����¶�����ʹ��scanf��getchar����
  * @param  *f
  * @retval ���ڽ��յ�������
  */
int fgetc(FILE *f)
{
  /* �ȴ������������� */
  while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

  return (int)USART_ReceiveData(USART2);
}

#else  //��ʹ��microlib�Ϳ���֧��printf����  

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;

/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */   
void _sys_exit(int x) 
{ 
	x = x; 
}

/**
  * @brief  ���¶���c�⺯��printf������1�����¶�����ʹ��printf������ӡ��Ϣ
  * @param  ch��Ҫ���͵��ֽ�����
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

#if EN_USART1_RX   //���ʹ���˽���

///**
//  * @brief  ���¶���c�⺯��printf������1�����¶�����ʹ��printf������ӡ��Ϣ
//  * @param  ch��Ҫ���͵��ֽ�����
//  * @param  *f
//  * @retval ch
//  */ 
//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
//	uint8_t rec;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d //0x0a��β)
//	{
//		rec = USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������

//  } 
//} 

#endif

