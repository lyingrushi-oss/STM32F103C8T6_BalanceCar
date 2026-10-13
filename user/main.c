#include "stm32f10x.h"
#include "bat_test.h"
#include "app_bat.h"
#include "app_button.h"
#include "app_pwm.h"
#include "pwm_test.h" 
#include "mpu6050_test.h"
#include "app_usart2.h"
#include "qmath_test.h"
#include "encoder_test.h"
 

int main(void)
{
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 Encoder_Test();
	 //App_USART2_Init();
	 //Qmath_Test();
	 //MPU6050_EularAngle_Test();
	 //App_USART2_Init();
	 //My_USART_Printf(USART2,"Hello world");
	//MPU6050_Test();
	//Bat_Test();
	App_Bat_Init();
	App_Button_Init();
	App_PWM_Init();
	PWM_Test();
	
	while(1)
	{
	  App_Bat_Proc();
		App_Button_Proc();
	
	}
}





