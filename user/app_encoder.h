#ifndef APP_ENCODER_H
#define APP_ENCODER_H
//#include "stm32f10x.h"
#include "stm32f10x.h"
void App_Encoder_Init(void);
float App_Encoder_Getpos_R(void);
float App_Encoder_Getpos_L(void);
float App_Encoder_GetSpeed_L(void);//T法测速
float App_Encoder_GetSpeed_R(void);

#endif
