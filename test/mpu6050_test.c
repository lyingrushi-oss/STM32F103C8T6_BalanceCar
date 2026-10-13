#include "mpu6050_test.h"
#include "app_mpu6050.h"
#include "app_usart2.h"
#include "usart.h"
#include "delay.h"
#include "task.h"
#include "qmath.h"
//
//
//
void MPU6050_Test(void)
{
 App_USART2_Init();
 App_MPU6050_Init();
 while(1)
 {
  //My_USART_Printf(USART2,"Hello world");
  App_MPU6050_Update();
 float ax=APP_MPU6050_GetAx();
 float ay=APP_MPU6050_GetAy();
 float az=APP_MPU6050_GetAz();
 float gx=APP_MPU6050_GetGx();
 float gy=APP_MPU6050_GetGy();
 float gz=APP_MPU6050_GetGz();
 float temperature=APP_MPU6050_GetTemperature();
 My_USART_Printf(USART2,"%f,%f,%f,%f,%f,%f,%f",ax,ay,ax,gx,gy,gz,temperature);
 Delay(100);
 }
 
 
 

}


//
//
//
static void USART_Proc(void)
{
 PERIODIC (100)
 float yaw=App_MPU6050_GetYaw();
 float pitch=App_MPU6050_GetPitch();
 float roll=App_MPU6050_GetRoll();
 float ax=APP_MPU6050_GetAx();
 float ay=APP_MPU6050_GetAy();
 float az=APP_MPU6050_GetAz();
 float gx=APP_MPU6050_GetGx();
 float gy=APP_MPU6050_GetGy();
 float gz=APP_MPU6050_GetGz();
 float temperature=APP_MPU6050_GetTemperature();
 //My_USART_Printf(USART2,"%f",temperature);
 My_USART_Printf(USART2,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",yaw,pitch,roll,ax,ay,ax,gx,gy,gz,temperature);


}
//
//@简介：测试欧拉角 （vofa）
//
void MPU6050_EularAngle_Test(void)
{
 App_MPU6050_Init();
 App_USART2_Init();
 while(1)
 {
  App_MPU6050_Proc();
	USART_Proc();
	//PERIODIC(10);
 
 
 }


}
