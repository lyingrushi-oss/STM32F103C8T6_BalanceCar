#include "app_pwm.h"
static void STBY_Pin_Init(void);
static void Motor_L_Init(void);//左电机初始化
static void Motor_R_Init(void);//右电机初始化
void App_PWM_Cmd(uint8_t on);
//
//@简介：初始化TB6612
//
void App_PWM_Init(void)
{
 STBY_Pin_Init();//STBY引脚初始化
 Motor_L_Init();//左电机初始化
 Motor_R_Init();//右电机初始化

}
//
//@简介：初始化STBY引脚
//PA1 Out_PP
//
static void STBY_Pin_Init(void)
{
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
 GPIO_InitTypeDef GPIO_InitStruct;
 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
 GPIO_Init(GPIOA,&GPIO_InitStruct);

}

//
//@简介：TB6612工作状态的控制函数
//@参数： 0 待机   非零 工作  
void App_PWM_Cmd(uint8_t on)
{
 if(on==0)//切换工作状态
 {
 GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
 }
else
{GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);}
}
//
//@简介：初始化左电机
//PA9 AIN1 PA10 AIN2 Out_PP
static void Motor_L_Init(void)
{
 //初始化AIN1 AIN2
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
 GPIO_InitTypeDef GPIO_InitStruct;
 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
 GPIO_Init(GPIOA,&GPIO_InitStruct);
 //初始化PWM
 //#1.初始化PA8引脚（接定时器1 ch1） AF_PP
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
 //GPIO_InitTypeDef GPIO_InitStruct;
 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
 GPIO_Init(GPIOA,&GPIO_InitStruct);
 //#2.初始化TIM1
 //#2.1.初始化时基单元
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
 TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStruct;
 TIM_TimeBase_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;
 TIM_TimeBase_InitStruct.TIM_Period=999;
 TIM_TimeBase_InitStruct.TIM_Prescaler=0;
 TIM_TimeBase_InitStruct.TIM_RepetitionCounter=0;
 TIM_TimeBaseInit(TIM1,&TIM_TimeBase_InitStruct);
 //#2.2配置输出比较
 TIM_OCInitTypeDef TIM_OCInitStruct;
 
 TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
 TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
 TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
 TIM_OCInitStruct.TIM_Pulse=0;
 TIM_OC1Init(TIM1,&TIM_OCInitStruct);
 //#2.3配置MOE
 TIM_CtrlPWMOutputs(TIM1,ENABLE);
 //#2.4闭合总开关
 TIM_Cmd(TIM1,ENABLE);

}
//
//@简介：右电机初始化
//BIN1 PB5 BIN2 PB7
//
static void Motor_R_Init(void)
{
 //初始化BIN1 BIN2
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 GPIO_InitTypeDef GPIO_InitStruct;
 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;
 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
 GPIO_Init(GPIOB,&GPIO_InitStruct);
 //初始化PWM
 //#1.初始化引脚 PB6 AF_PP
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 //GPIO_InitTypeDef GPIO_InitStruct;
 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
 GPIO_Init(GPIOB,&GPIO_InitStruct);
 //#2.初始化TIM4
 //#2.1.初始化时基单元
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
 TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStruct;
 TIM_TimeBase_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;
 TIM_TimeBase_InitStruct.TIM_Period=999;
 TIM_TimeBase_InitStruct.TIM_Prescaler=0;
 TIM_TimeBase_InitStruct.TIM_RepetitionCounter=0;
 TIM_TimeBaseInit(TIM4,&TIM_TimeBase_InitStruct);
 //#2.2配置输出比较
 TIM_OCInitTypeDef TIM_OCInitStruct;
 
 TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
 TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
 TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
 TIM_OCInitStruct.TIM_Pulse=0;
 TIM_OC1Init(TIM4,&TIM_OCInitStruct);
 //#2.3配置MOE
 //TIM_CtrlPWMOutputs(TIM4,ENABLE);
 //#2.4闭合总开关
 TIM_Cmd(TIM4,ENABLE);
}
#include "math.h"
//
//@简介：配置左电机占空比
//@参数：Duty:-100.0f~100.0f
//
void App_PWM_Set_L(float Duty)
{
 float sign;//符号：正数+1 负数-1
 if(Duty>=0) sign=1;
 else sign =-1;
 Duty=fabsf(Duty);
 if (sign>=0)//正转
 {
 GPIO_WriteBit(GPIOA,GPIO_Pin_9,Bit_SET);//AIN1 H
 GPIO_WriteBit(GPIOA,GPIO_Pin_10,Bit_RESET);//AIN2 L
 }
 else//反转
 {
  GPIO_WriteBit(GPIOA,GPIO_Pin_9,Bit_RESET);//AIN1 L
	GPIO_WriteBit(GPIOA,GPIO_Pin_10,Bit_SET);//AIN2 H
 }
  uint16_t ccr=Duty/100.0f*999;
	TIM_SetCompare1(TIM1,ccr);

}
//
//@简介：配置右电机占空比
//@参数：Duty:-100.0f~100.0f
//
void App_PWM_Set_R(float Duty)
{
 float sign;//符号：正数+1 负数-1
 if(Duty>=0) sign=1;
 else sign =-1;
 Duty=fabsf(Duty);
 if (sign>=0)//正转
 {
 GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET);//BIN1 H
 GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_RESET);//BIN2 L
 }
 else//反转
 {
  GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET);//BIN1 L
	GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_SET);//BIN2 H
 }
  uint16_t ccr=Duty/100.0f*999;
  TIM_SetCompare1(TIM4,ccr);


}
