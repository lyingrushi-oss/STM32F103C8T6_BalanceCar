#include "app_bat.h"
#include "delay.h"
static void TIM2_TRGO_Init(void);
static void ADC1_Init(void);
static void LED_Init(void);
void App_Bat_Proc(void);
 volatile float vbat=0.0f;
 
 //
 //@简介：初始化LED
 //
 static void LED_Init(void)
 {
  //PA4 PA5 PA6 Out_PP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
 
 
 }
 
 //
 //@简介：电池进程函数
//
static uint32_t lastTime=0;
 static uint8_t stage=0;
void App_Bat_Proc(void)
{
 if(vbat>7.9)//亮三颗
  {
	  GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
		GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET);
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
	}
	else if(vbat>7.4)//亮两颗
	{
	 GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
		GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET);
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
	}
	else if(vbat>7)//亮一颗
	{
	 GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
		GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
	}
	else if(vbat>6.5)//熄灭
	{
	 GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
		GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
	}
	else//快闪
	{
	  uint32_t now=GetTick();
		if(now-lastTime>100)
		 {
		   switch (stage)
			  {
				case 0://当前熄灭
				 {GPIO_WriteBit(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6,Bit_SET);
				   stage=1;
					 lastTime=now;
					 break;
				
				
				 }
				case 1://当前熄灭
				{GPIO_WriteBit(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6,Bit_RESET);}
				  stage=0;
					lastTime=now;
					break;
				}
		 
		 }

	
	}

}
//@简介：初始化电池模块
//
void App_Bat_Init(void)
{
  //#1.初始化TIM2定时器
	TIM2_TRGO_Init();
	
	//#2.初始化ADC模块
	ADC1_Init();
	
	//#3.初始化LED
	LED_Init();
}
//
//
//@简介：ADC模块中断响应函数
//
void ADC1_2_IRQHandler(void)
{
  //#1.等待JOEC标志位为1
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC)==SET)
	{
	  //#2.清除标志位，避免重复触发中断
		ADC_ClearFlag(ADC1,ADC_FLAG_JEOC);
		//#3.将JDR寄存器中的值读取出来
		uint16_t jdr1=ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1);
		vbat=jdr1/4095.0f*3.3*8.4f/3.3;
	
	
	}
	


}
//
//@简介：初始化定时器2TRGO,产生脉冲信号
//

static void TIM2_TRGO_Init(void)
{
  //#1.1开启TIM2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	
	//#1.2初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=9999;
	TIM_TimeBaseInitStruct.TIM_Prescaler=71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	
	
	//#1.3配置定时器的TRGO
	TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_Update);
	
	//#1.4闭合总开关
	TIM_Cmd(TIM2,ENABLE);

}
//
//@简介：初始化ADC1
//
static void ADC1_Init(void)
{
  //#2.1配置ADC时钟为12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//#2.2初始化IO引脚 PB0 模拟模式
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//#2.3初始化ADC本身
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;
	
	ADC_Init(ADC1,&ADC_InitStruct);
	//#2.4配置注入序列
	ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_T2_TRGO);
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_1Cycles5);
	ADC_InjectedSequencerLengthConfig(ADC1,1);
	//ADC_ExternalTrigConvCmd(ADC1,ENABLE);
	ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE);
	//#2.5闭合总开关
	ADC_Cmd(ADC1,ENABLE);
	
	//#2.6配置注入序列JEOC的中断
	ADC_ITConfig(ADC1,ADC_IT_JEOC,ENABLE);
	//#2.7配置NVIC模块
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel=ADC1_2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStruct);
	
	//#2.5闭合总开关
	ADC_Cmd(ADC1,ENABLE);//最后闭合总开关


}
//
//@简介：获取电池的当前电压，单位是V
//
float App_Bat_Get(void)
{
  return vbat;


}

