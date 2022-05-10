#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "stm32f10x.h"
//端口宏定义
//VBAT
#define UL_VBAT_PORT GPIOA
#define UL_VBAT_PIN GPIO_Pin_1
//PWR
#define UL_PWR_PORT GPIOB
#define UL_PWR_PIN GPIO_Pin_3
//急停按钮
#define UL_EMERGY_PORT GPIOB
#define UL_EMERGY_PIN GPIO_Pin_14
//B4
#define UL1_TRIG_CLK RCC_APB2Periph_GPIOB
#define UL1_TRIG_PORT GPIOB
#define UL1_TRIG_PIN GPIO_Pin_4

//A8
#define UL1_ECHO_CLK RCC_APB2Periph_GPIOA
#define UL1_ECHO_PORT GPIOA
#define UL1_ECHO_PIN GPIO_Pin_8

//B5
#define UL2_TRIG_CLK RCC_APB2Periph_GPIOB
#define UL2_TRIG_PORT GPIOB
#define UL2_TRIG_PIN GPIO_Pin_5

//B15
#define UL2_ECHO_CLK RCC_APB2Periph_GPIOB
#define UL2_ECHO_PORT GPIOB
#define UL2_ECHO_PIN GPIO_Pin_15

//B6
#define UL3_TRIG_CLK RCC_APB2Periph_GPIOB
#define UL3_TRIG_PORT GPIOB
#define UL3_TRIG_PIN GPIO_Pin_6

//B8
#define UL3_ECHO_CLK RCC_APB2Periph_GPIOB
#define UL3_ECHO_PORT GPIOB
#define UL3_ECHO_PIN GPIO_Pin_8

//B7
#define UL4_TRIG_CLK RCC_APB2Periph_GPIOB
#define UL4_TRIG_PORT GPIOB
#define UL4_TRIG_PIN GPIO_Pin_7

//B9
#define UL4_ECHO_CLK RCC_APB2Periph_GPIOB
#define UL4_ECHO_PORT GPIOB
#define UL4_ECHO_PIN GPIO_Pin_9

typedef struct
{
  float distance;
  uint8_t flag; //上升沿触发标志位
  uint32_t start_time;
  uint32_t end_time;
} UltrasonicDef;

extern UltrasonicDef ultrasonic[];
//
#define ULTRASONIC_NUMBER 2
#define ULTRASONIC_TIM_ARR 0XFFFF
#define ULTRASONIC_TIM_PSC 719           //计数频率：72M/(719+1) = 100000，即记一个数的时间为：1S/100000=10us
#define MAX_MEASURE_DISTANCE 255         //单位：cm
#define ULTRASONIC_TIM_MAX_TIME (15 + 5) //测量最大距离所需的时间(单位：ms)：MAX_MEASURE_DISTANCE * 2 /100 / 340 * 1000
#define ULTRASONIC_TIM_MAX_COUNT 1500

extern float distance_ultrasonic[2];

void ULTRASONIC_Init(void);
void ULTRASONIC_Measure(void);

#endif /* __ULTRASONIC_H*/
