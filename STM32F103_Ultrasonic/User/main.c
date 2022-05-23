#include "stm32f10x.h"
#include "uart2.h"
#include "delay.h"
#include "ultrasonic.h"

int main(void)
{
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  UART2_Init(115200);
  delay_init();
	
  ULTRASONIC_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);        //使能PC端口时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = UL_PWR_PIN | UL_VBAT_PIN; // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                         //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UL_PWR_PORT, &GPIO_InitStructure);
	GPIO_Init(UL_VBAT_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(UL_PWR_PORT, UL_PWR_PIN);
	GPIO_SetBits(UL_VBAT_PORT, UL_VBAT_PIN);
	
	
  //u2_printf("超声波模块测试r\n");
	//急停按钮
	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Pin = UL_EMERGY_PIN; // 端口配置
  GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;                                         //推挽输出
  GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UL_EMERGY_PORT, &GPIO_InitStructure2);
	GPIO_ResetBits(UL_EMERGY_PORT, UL_EMERGY_PIN);
	
  while (1)
  {
    ULTRASONIC_Measure();
		
		//心跳包
		//u2_printf("0xAA, 0x01, 0x00, 0xAB");
		//关机
		if(GPIO_ReadInputDataBit(UL_EMERGY_PORT, UL_EMERGY_PIN))//key0被按下
			{
				u2_printf("0xAA,0x04");
			}else{
				
				//超声波
				u2_printf("%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x", 
										0xAA, 0x03,
										printf("0x%x", (int)ultrasonic[0].distance % 10), printf("0x%x", (int)ultrasonic[0].distance / 10),
										printf("0x%x", (int)ultrasonic[1].distance % 10), printf("0x%x", (int)ultrasonic[1].distance / 10),
										printf("0x%x", (int)ultrasonic[2].distance % 10), printf("0x%x", (int)ultrasonic[2].distance / 10),
										printf("0x%x", (int)ultrasonic[3].distance % 10), printf("0x%x", (int)ultrasonic[3].distance / 10));
				
				delay_ms(500);
			}
		}

}