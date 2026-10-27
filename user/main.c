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
#include "app_encoder.h"
#include "app_pid.h"
#include "app_motor.h"
#include "task.h"
#include "app_usart2.h"
#include "delay.h"
 
static void USART2_Proc(void);
static float target_omega;
int main(void)
{
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	App_Button_Init();
	App_Bat_Init();
	App_PWM_Init();
	//App_PWM_Cmd(1);
	App_Encoder_Init();
	//App_PID_Init();
	App_Motor_Init();
	App_USART2_Init();
	
	//Encoder_T_Method_Test();
   //Encoder_M_Method_Test();
	 //Encoder_Test();
	 //App_USART2_Init();
	 //Qmath_Test();
	 //MPU6050_EularAngle_Test();
	 //App_USART2_Init();
	 //My_USART_Printf(USART2,"Hello world");
	//MPU6050_Test();
	//Bat_Test();
	//App_Bat_Init();
	//App_Button_Init();
	//App_PWM_Init();
	//PWM_Test();
	
	while(1)
	{
	  
		App_Button_Proc();
		App_Bat_Proc();
		App_Motor_Proc();
		USART2_Proc();
	
	}
}

static void USART2_Proc(void)
{
	PERIODIC(10)
 target_omega=(GetTick()/1000)%10*2.0f;
 float omega_l=App_Encoder_GetSpeed_L();
	float omega_r=App_Encoder_GetSpeed_R();
	My_USART_Printf(USART2,"%.3f,%.3f,%.3f",target_omega,omega_l,omega_r);
	

}



