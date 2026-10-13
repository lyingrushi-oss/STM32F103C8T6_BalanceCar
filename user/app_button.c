#include "app_button.h"
#include "button.h"
#include "app_pwm.h"
//
//
//
static Button_TypeDef UserKey;
static void OnUserKey_Clicked(uint8_t);
//
//@简介：按钮初始化
//
void App_Button_Init(void)
{
  Button_InitTypeDef Button_InitStruct;
	Button_InitStruct.GPIOx=GPIOA;
	Button_InitStruct.GPIO_Pin=GPIO_Pin_11;
	My_Button_Init(&UserKey,&Button_InitStruct);
	My_Button_SetClickCb(&UserKey,OnUserKey_Clicked);
}
//
//
//
void App_Button_Proc(void)
{
   My_Button_Proc(&UserKey);


}
//声明一个变量记录电机的工作状态
static uint8_t pwm_on=0;//0 电机休眠 1 电机工作
//
//@简介：回调函数
//
static void OnUserKey_Clicked(uint8_t clicks)
{
if(clicks==1)//单击时才处理
 {
   if(pwm_on==0)//翻转工作状态
	 {pwm_on=1;}
	 else
	 {pwm_on=0;}
	 App_PWM_Cmd(pwm_on);
	 
 
 }


}
