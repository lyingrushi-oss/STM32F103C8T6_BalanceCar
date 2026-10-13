#include "pwm_test.h"
#include "app_pwm.h"
#include "delay.h" 
//
//@简介:测试PWM调速模块
//先初始化电机 再让它工作
//调速 正转30%（2s）正转60%（2s） 正转90%（2s）
void PWM_Test(void)
{
App_PWM_Init();//初始化电机
App_PWM_Cmd(1);//唤醒电机
 App_PWM_Set_L(30);//调速
  App_PWM_Set_R(30);
 Delay(2000);
 App_PWM_Set_R(60);
App_PWM_Set_L(60);
Delay(2000);
App_PWM_Set_R(90);
App_PWM_Set_L(90);
Delay(2000);
App_PWM_Cmd(0);//关闭电机
}
