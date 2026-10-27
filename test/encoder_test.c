#include "encoder_test.h"
#include "app_encoder.h"
#include "app_usart2.h"
#include "delay.h"
void Encoder_Test(void)
{
 App_Encoder_Init();
 App_USART2_Init();
 
 while(1)
 {
  Delay(50);
	float encoder_l=App_Encoder_Getpos_L();
	float encoder_r=App_Encoder_Getpos_R();
	My_USART_Printf(USART2,"%f,%f\n",encoder_l,encoder_r);
 
 }

}

static float last_pos_l=0;
static float last_pos_r=0;
//
//
//
void Encoder_M_Method_Test(void)
{
 App_Encoder_Init();
 App_USART2_Init();

 while(1)
 {
  Delay(1);
	float encoder_l=App_Encoder_Getpos_L();
	float encoder_r=App_Encoder_Getpos_R();
	float M1=encoder_l-last_pos_l;
  float M2=encoder_r-last_pos_r;	 
	float omega_l=M1/0.001f;
	float omega_r=M2/0.001f;
	 My_USART_Printf(USART2,"%f,%f,%f,%f\n",encoder_l,encoder_r,omega_l,omega_r);
	 last_pos_l=encoder_l;
	 last_pos_r=encoder_r;

 }

}
//
//
//
void Encoder_T_Method_Test(void)
{
 App_Encoder_Init();
	App_USART2_Init();
 while(1)
 {
	 Delay(1);
  float omega_l=App_Encoder_GetSpeed_L();
	 float omega_r=App_Encoder_GetSpeed_R();
	 My_USART_Printf(USART2,"%.3f,%.3f\n",omega_l,omega_r);
 
 
 }
}
