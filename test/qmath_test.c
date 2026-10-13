#include "qmath_test.h"
#include "delay.h"
#include "app_usart2.H"
void Qmath_Test(void)
{
//对整数加法进行调试
My_USART_Printf(USART2,"现在开始对整数加法进行测试:");
	int a=1,b=2,c=3;
 uint32_t t1=GetTick();
 for(uint32_t i=0;i<1000000;i++)
 {
   c=a+b;
	 (void)c;
 }
 uint32_t t2=GetTick();
 My_USART_Printf(USART2,"每次运算所需要的时间为：%.3fus\n",(t2-t1)/1000000.0f*1000.0f);
}
