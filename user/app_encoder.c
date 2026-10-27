#include "app_encoder.h"
#include "delay.h"
static volatile int64_t encoder_l=0;
static volatile int64_t encoder_r=0;
static volatile int8_t direction_l=1;//正转为1 反转为-1 用于T法测速
static volatile int8_t direction_r=1;
static volatile uint64_t t0_l=0,t1_l=0;
static volatile uint64_t t0_r=0,t1_r=0;
//static volatile uint64_t t0_cpy_l=0,t1_cpy_l=0;
//static volatile uint64_t t0_cpy_r=0,t1_cpy_r=0;
static void encoder_l_Init(void);
static void encoder_r_Init(void);
void EXTI15_10_IRQHandler(void);
void EXTI3_IRQHandler(void);
//
//@简介：对编码器进行初始化
void App_Encoder_Init(void)
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
	 
	 t1_l=t0_l;
	 t0_l=GetUs();
	 
	uint8_t a=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
	uint8_t b=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
	if((a==Bit_SET&&b==Bit_RESET)||(a==Bit_RESET&&b==Bit_SET))
	 {
	 
		 encoder_l--;//现在轮胎反转
		 if(direction_l==1)//以前正转，发生换向
		 {
		  direction_l=-2;
		 }
			else
			{direction_l=-1;}
		
	 }
	 else
	 {
	  
		 encoder_l++;//现在轮胎正转
		 if(direction_l==-1)
		 {
		  direction_l=2;//发生换向
		 }
			else
			{
			 direction_l=1;
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
	
	 t1_r=t0_r;//中断响应函数里面改变了t0,t1,direction 的值
	 t0_r=GetUs();//所以要在算速度的函数里找中间变量，保存t0,t1,direction的值
 uint8_t a=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
	uint8_t b=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
	if((a==Bit_SET&&b==Bit_RESET)||(a==Bit_RESET&&b==Bit_SET))
	 {
	 
		 encoder_r--;//现在轮胎反转
		 if(direction_r==1)//以前正转，发生换向
		 {
			 direction_r=-2;//不要直接写0，写了0以后无法判断除以稳定态还是过渡态
		 }
			else
			{direction_r=-1;}
		
	 }
	 else
	 {
	  
		 encoder_r++;//现在轮胎正转
		 if(direction_r==-1)
		 {
		  direction_r=2;//发生换向
		 }
			else
			{
			 direction_r=1;
			}
		
	 
	 }


}
//
//@简介：返回左编码器的位置测量结果
//
float App_Encoder_Getpos_L(void)
{
 return encoder_l/22.0f/(30613.0f/1500.0f)*360.0f;
}
//
//@简介：返回右编码器的位置测量结果
//
float App_Encoder_Getpos_R(void)
{
	
 return encoder_r/22.0f/(30613.0f/1500.0f)*360.0f;
}
//
//@简介：返回左轮胎的角速度 单位：rad/s
//
float App_Encoder_GetSpeed_L(void)
{
	
	
	__disable_irq();//关闭中断 保存下t0和t1的值 这部分代码不要写在中断函数里面
	 int8_t direction_cpy=direction_l;;
	uint64_t t0_cpy_l = t0_l;
	uint64_t t1_cpy_l = t1_l;
	 
	__enable_irq();//使能中断
	float T;
	uint64_t now=GetUs();
	if((t0_cpy_l-t1_cpy_l)>(now-t0_cpy_l))
	{
	 T=(t0_cpy_l-t1_cpy_l)/1.0e6f;
	}
	else 
	{
	 T=(now-t0_cpy_l)/1.0e6f;
	}
	if(direction_cpy==2|direction_cpy==-2)
	{
	 return 0.0f;
	}
	else
	{
	 return direction_cpy/T/22.0f/(30613.0f/1500.0f)*6.2831853f;
	}

}
//
//@简介：返回右轮胎的角速度 单位：rad/s
//
float App_Encoder_GetSpeed_R(void)
{ 
 
	
  __disable_irq();//关闭单片机的总中断
	 uint64_t t0_cpy_r=t0_r;
	 uint64_t t1_cpy_r=t1_r;
	int8_t direction_cpy=direction_r;//必须有符号，不然只有正值
	__enable_irq();//开启单片机的中断
	float T;//放在赋值变量下面，就没有毛刺了好神奇
 uint64_t now=GetUs();
 if((t0_cpy_r-t1_cpy_r)>(now-t0_cpy_r))
 {
  T=(t0_cpy_r-t1_cpy_r)/1.0e6f;
 }
 else 
 {
  T=(now-t0_cpy_r)/1.0e6f;
 }
 if(direction_cpy==2|direction_cpy==-2)
 {
  return 0.0f;
	 
 }
 else 
  
 {
   return direction_cpy/T/22.0f/(30613.0f/1500.0f)*6.2831853f;//获取的时间是微秒极

 }
}
