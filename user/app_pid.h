#ifndef APP_PID_H
#define APP_PID_H
#include "stm32f10x.h"
typedef struct
{
  float Kp;
	float Ki;
	float Kd;
	float sp;
	uint64_t t_k_1;//t[k-1]上一次PID运行的时刻
	float err_k_1;//err[k-1]上一次运行的误差值
	float err_int_k_1;//err_int[k-1]上一次的积分值
	float upper_limit;//输出上限
	float lower_limit;//输出下限
	


}PID_TypeDef;
void App_PID_Init(PID_TypeDef*PID_Struct,float Kp,float Ki,float Kd);
void App_PID_SPChange(PID_TypeDef*PID_Struct,float sp);
//float PID_Compute(PID_TypeDef *PID,float FB);
float PID_Compute(PID_TypeDef *PID,float FB);
void PID_LimitConfig(PID_TypeDef*PID,float upper,float lower);
void PID_Reset(PID_TypeDef*PID);//对PID进行复位操作
#endif
