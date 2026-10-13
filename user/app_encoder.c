#include "app_encoder.h"
static volatile int64_t encoder_l=0;
static volatile int64_t encoder_r=0;
static void encoder_l_Init(void);
static void encoder_r_Init(void);
void EXTI15_10_IRQHandler(void);
void EXTI3_IRQHandler(void);
//
//@简介：对编码器进行初始化
void App_encoder_Init(void)
{
 encoder_l_Init();
 encoder_r_Init();

}
//
//@简介：对左编码器进行初始化
//
static void encoder_l_Init(void)
{
 //#1.初始化IO引脚 A相PB14 B相PB15 IPU
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 GPIO_InitTypeDef GPIO_InitStruct;
 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_15;
 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
 GPIO_Init(GPIOB,&GPIO_InitStruct);
 //#2.配置中断线
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
 GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
 //#3.初始化EXTI模块
 //RCC_APB2PeriphClockCmd() EXTI的时钟由系统内核直接提供，无需额外配置
 EXTI_InitTypeDef EXTI_InitStruct;
 EXTI_InitStruct.EXTI_Line=EXTI_Line14;
 EXTI_InitStruct.EXTI_LineCmd=ENABLE;
 EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
 EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising_Falling;//双边沿捕获
 EXTI_Init(&EXTI_InitStruct);
 //#4.配置NVIC模块
 NVIC_InitTypeDef NVIC_InitStruct;
 NVIC_InitStruct.NVIC_IRQChannel=EXTI15_10_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
 NVIC_Init(&NVIC_InitStruct);
 //#5.编写中断响应函数
 //EXTI15_10_IRQHandler();
 
 
 
 

}
//
//@简介：左电机中断响应函数
//
void EXTI15_10_IRQHandler(void)
{
 if(EXTI_GetFlagStatus(EXTI_Line14)==SET)
 {
  EXTI_ClearFlag(EXTI_Line14);
	uint8_t a=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
	uint8_t b=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
	if(a==Bit_SET)
	 {
	  if(b==Bit_RESET)
		{
		 encoder_l++;
		}
		else
		{
		 encoder_l--;
		}
	 }
	 else
	 {
	  if(b==Bit_SET)
		{
		 encoder_l++;
		}
		else
		{
		 encoder_l--;
		}
	 
	 }
 
 
 
 }

}
//@简介：对右编码器进行初始化
//
static void encoder_r_Init(void)
{
 //#1.初始化IO引脚 A相PB3 B相PB4 IPU
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable ,ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 GPIO_InitTypeDef GPIO_InitStruct;
 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4;
 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
 GPIO_Init(GPIOB,&GPIO_InitStruct);
 //#2.配置中断线
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
 GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
 //#3.初始化EXTI模块
 //RCC_APB2PeriphClockCmd() EXTI的时钟由系统内核直接提供，无需额外配置
 EXTI_InitTypeDef EXTI_InitStruct;
 EXTI_InitStruct.EXTI_Line=EXTI_Line3;
 EXTI_InitStruct.EXTI_LineCmd=ENABLE;
 EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
 EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising_Falling;//双边沿捕获
 EXTI_Init(&EXTI_InitStruct);
  //#4.配置NVIC模块
 NVIC_InitTypeDef NVIC_InitStruct;
 NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
 NVIC_Init(&NVIC_InitStruct);
 //#5.编写中断响应函数
 //EXTI3_IRQHandler();//中断响应函数无需调用 

}
//
//@简介：编写右电极的中断响应函数
//
void EXTI3_IRQHandler(void)
{
 EXTI_ClearFlag(EXTI_Line3);
 uint8_t a=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
	uint8_t b=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
	if(a==Bit_SET)
	 {
	  if(b==Bit_RESET)
		{
		 encoder_r++;
		}
		else
		{
		 encoder_r--;
		}
	 }
	 else
	 {
	  if(b==Bit_SET)
		{
		 encoder_r++;
		}
		else
		{
		 encoder_r--;
		}
	 
	 }


}
//
//@简介：返回左编码器的位置测量结果
//
int64_t encoder_l_pos(void)
{
 return encoder_l;
}
//
//@简介：返回左编码器的位置测量结果
//
int64_t encoder_r_pos(void)
{
 return encoder_r;
}

