#ifndef APP_MOTOR_H
#define APP_MOTOR_H
#include "stm32f10x.h"
void App_Motor_Init(void);
void App_Motor_Cmd(uint8_t on);
void App_Motor_Proc(void);
void App_Motor_Setomega_l(float omega);
void App_Motor_Setomega_r(float omega);
#endif

