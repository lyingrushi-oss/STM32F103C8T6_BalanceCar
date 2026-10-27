#include "app_motor.h"
#include "app_pid.h"
#include "delay.h"
#include "app_encoder.h"
#include "app_pwm.h"
#include "app_bat.h"
#include "task.h"
static PID_TypeDef PID_L;
static PID_TypeDef PID_R;
//
//@简介：对电机进行初始化
//
void App_Motor_Init(void)
{
 App_PID_Init(&PID_L,0.5f,7.0f,0.0f);
	
	PID_LimitConfig(&PID_L,8.4f,-8.4f);
	App_PID_Init(&PID_R,0.5f,7.0f,0.0f);
	PID_LimitConfig(&PID_R,8.4f,-8.4f);
	

}
//
//@简介：电机进程函数
//
void App_Motor_Proc(void)
{
 PERIODIC (1)//延迟1ms
	//#1.获取角速度
	float omega_l=App_Encoder_GetSpeed_L();
	float omega_r=App_Encoder_GetSpeed_R();
	//#2.计算PID输出结果CO
	float ua_l=PID_Compute(&PID_L,omega_l);
	float ua_r=PID_Compute(&PID_R,omega_r);
	//#3.给电机加Ua大小的电压信号
	float vbat=App_Bat_Get();//获取电池电压
	float duty_l=ua_l/vbat*100.0f;//利用电压信号计算出左电机的占空比
	App_PWM_Set_L(duty_l);//通过获取的占空比设置对应的PWM信号
	float duty_r=ua_r/vbat*100.0f;
	App_PWM_Set_R(duty_r);
	

}
//
//@简介：设置左电机转速
//
void App_Motor_Setomega_l(float omega)
{
 PID_L.sp=omega;
	//还有一种方法是App_PID_SPChange(&PID_L,omega);

}
//
//@简介：设置右电机转速
//
void App_Motor_Setomega_r(float omega)
{
 PID_R.sp=omega;

}
//
//@简介：控制电机的工作状态
//@参数：on代表电机工作状态 0 休眠 1 工作
void App_Motor_Cmd(uint8_t on)
{
 App_PWM_Cmd(on);
	PID_Reset(&PID_L);
	PID_Reset(&PID_R);


}
