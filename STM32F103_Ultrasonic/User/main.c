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
  
  u2_printf("³¬Éù²¨Ä£¿é²âÊÔr\n");   
  
  while(1)
  {
	  ULTRASONIC_Measure();
		if(ultrasonic[0].distance > 10 || 
			 ultrasonic[1].distance > 10 ||
		   ultrasonic[2].distance > 10 ||
		   ultrasonic[3].distance > 10 )
		{
			u2_printf("%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x",0xAA, 0x03, 
														printf("0x%x",(int)ultrasonic[0].distance%10), printf("0x%x",(int)ultrasonic[0].distance/10),
														printf("0x%x",(int)ultrasonic[1].distance%10), printf("0x%x",(int)ultrasonic[1].distance/10),
														printf("0x%x",(int)ultrasonic[2].distance%10), printf("0x%x",(int)ultrasonic[2].distance/10),
														printf("0x%x",(int)ultrasonic[3].distance%10), printf("0x%x",(int)ultrasonic[3].distance/10));
		}
    u2_printf("UL1 Distance:%.2fcm\r\n",ultrasonic[0].distance);
    u2_printf("UL2 Distance:%.2fcm\r\n",ultrasonic[1].distance);
	  delay_ms(500);
  }
}
