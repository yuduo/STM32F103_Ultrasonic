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
  
  u2_printf("超声波模块测试r\n");   //塑料袋内航空俱乐部还不
  
  while(1)
  {
	  ULTRASONIC_Measure();
    u2_printf("UL1 Distance:%.2fcm\r\n",ultrasonic[0].distance);
    u2_printf("UL2 Distance:%.2fcm\r\n",ultrasonic[1].distance);
	  delay_ms(500);
  }
}
