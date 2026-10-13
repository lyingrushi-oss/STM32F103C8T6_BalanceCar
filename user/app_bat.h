#ifndef APP_BAT_H
#define APP_BAT_H

#include "stm32f10x.h"
void App_Bat_Init(void);
void ADC1_2_IRQHandler(void);
float App_Bat_Get(void);
void App_Bat_Proc(void);
//static void LED_Init(void);









#endif //防止头文件被重复引用

