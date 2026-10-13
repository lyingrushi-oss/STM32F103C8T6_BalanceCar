#include "bat_test.h"
#include "app_usart2.h"
#include "app_bat.h"
#include "delay.h"
//
//@简介：电池电压监控模块的测试程序
//      通过串口2把电压发送给电脑
//      通过vofa把电压结果显示出来
//

void Bat_Test(void)
{
  //#1.初始化串口2
	App_USART2_Init();
	
	
	//#2.初始化电压监控模块
	App_Bat_Init();
	
	while(1)
	{
	
	//间隔10ms采集一次电压值
	//My_USART_Printf(USART2,"HELLO\r");
	float volt=App_Bat_Get();
	My_USART_Printf(USART2,"%.3f\r\n",volt);
	 Delay(10);
	 
	
	}
}

