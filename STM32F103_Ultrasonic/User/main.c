#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "ultrasonic.h"

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  usart1_init(115200);
  delay_init();
  ULTRASONIC_Init();

  printf("超声波模块测试r\n"); //

  while (1)
  {
    ULTRASONIC_Measure();
    printf("UL1 Distance:%.2fcm\r\n", ultrasonic[0].distance);
    printf("UL2 Distance:%.2fcm\r\n", ultrasonic[1].distance);
    printf("UL3 Distance:%.2fcm\r\n", ultrasonic[2].distance);
    printf("UL4 Distance:%.2fcm\r\n", ultrasonic[3].distance);
    delay_ms(500);
  }
}
