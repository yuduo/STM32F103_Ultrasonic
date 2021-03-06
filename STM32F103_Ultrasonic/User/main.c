#include "stm32f10x.h"
#include "uart2.h"
#include "delay.h"
#include "ultrasonic.h"

int main(void)
{
	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	UART2_Init(115200);
	delay_init();

	ULTRASONIC_Init();
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PC端口时钟

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIOB时钟和复用功能（要先打开复用才能修改复用功能）
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);       //要先开时钟，再重映射；这句表示关闭JTAG，使能SWD
	
	GPIO_InitStructure.GPIO_Pin = UL_PWR_PIN | UL_VBAT_PIN; // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UL_PWR_PORT, &GPIO_InitStructure);
	GPIO_Init(UL_VBAT_PORT, &GPIO_InitStructure);
	GPIO_SetBits(UL_PWR_PORT, UL_PWR_PIN);
	GPIO_SetBits(UL_VBAT_PORT, UL_VBAT_PIN);//是否使用电池包？

	// u2_printf("超声波模块测试r\n");
	//急停按钮
	
	GPIO_InitStructure.GPIO_Pin = UL_EMERGY_PIN;	  // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UL_EMERGY_PORT, &GPIO_InitStructure);
	GPIO_SetBits(UL_EMERGY_PORT, UL_EMERGY_PIN);

	while (1)
	{
		int i;
		ULTRASONIC_Measure();

		//心跳包
		// u2_printf("0xAA, 0x01, 0x00, 0xAB");
		//关机
		if (GPIO_ReadInputDataBit(UL_EMERGY_PORT, UL_EMERGY_PIN)) // key0被按下
		{
			for(i = 0; i < SEND_DATA_SIZE;i++)
			UART2_TX_BUF[i]=0x00;
			UART2_TX_BUF[0] = 0xAA;
			UART2_TX_BUF[1] = 0x0C;
			 SendData();
		}
		else
		{

			//超声波
						for(i = 0; i < SEND_DATA_SIZE;i++)
			UART2_TX_BUF[i]=0x00;
			UART2_TX_BUF[0] = 0xAA;
			UART2_TX_BUF[1] = 0x03;
			UART2_TX_BUF[2] = (ultrasonic[0].distance*100 / 256);
			UART2_TX_BUF[3] = ultrasonic[0].distance*100 % 256;
			UART2_TX_BUF[4] = ultrasonic[1].distance*100 / 256;
			UART2_TX_BUF[5] = ultrasonic[1].distance*100 % 256;
			UART2_TX_BUF[6] = ultrasonic[2].distance*100 / 256;
			UART2_TX_BUF[7] = ultrasonic[2].distance*100 % 256;
			UART2_TX_BUF[8] = ultrasonic[3].distance*100 / 256;
			UART2_TX_BUF[9] = ultrasonic[3].distance*100 % 256;
			SendData();


			
		}
		delay_ms(500);
	}
}