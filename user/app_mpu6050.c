#include "app_mpu6050.h"
#include "i2c.h"
#include "delay.h"
#include "task.h"
#include "qmath.h"
static void reg_write(uint8_t reg,uint8_t value);
static uint8_t reg_read(uint8_t reg);
static float ax,ay,az;//加速度计的结果 单位g
static float gx,gy,gz;//轴上的角速度大小 单位°/s
static float yaw,pitch,roll;
static float temperature;//温度计的值 单位摄氏度
//
//@简介：初始化MPU6050模块
//
void App_MPU6050_Init(void)
{
  //#1.初始化I2C总线 PB8 PB9 AF_PP
	//配置重映射
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
	//初始化IO引脚PB8 PB9 AF_PP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_OD;
  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
  GPIO_Init(GPIOB,&GPIO_InitStruct);
	//#2.初始化I2C本身
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct. I2C_ClockSpeed=400000;
	I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle=I2C_DutyCycle_2;
	//I2C_InitStruct
	I2C_Init(I2C1,&I2C_InitStruct);
	//#3.配置MPU6050的参数
	reg_write(0x6b,0x80);//复位
	Delay(100);
	reg_write(0x6b,0x00);//关闭睡眠模式
	reg_write(0x1b,0x18);//配置陀螺仪的量程为+-2000°/s
	reg_write(0x1c,0x00);//将加速计的量程配置为+-2g
	//#4.读取原始数据
	

}
//
//@简介：MPU6050的进程函数
//
#define alfa 0.95238;
void App_MPU6050_Proc(void)
{
  //进程函数每5ms运行一次
 PERIODIC (5)
 //更新传感器的值
 App_MPU6050_Update();
 //通过陀螺仪测量欧拉角（漂移）
 float yaw_g=yaw+gz*0.005;
 float pitch_g =pitch +gx*0.005;
 float roll_g=roll-gx*0.005;
 
 //通过加速度计测量欧拉角
 //float yaw_a=0.0f;
 float pitch_a=qatan2(ay,az)/3.1415927f*180.0f;
 float roll_a=qatan2(ax,az)/3.1415927f*180.0f;
 
 //互补滤波计算欧拉角
 yaw=yaw_g;
 pitch=0.95238 * pitch_g+(1-0.95238) * pitch_a;
 roll=0.95238*roll_g+(1-0.95238)*roll_a;
 

}

//
//@简介：发送数据
//@参数：reg 寄存器地址 value 发送的值
//
static void reg_write(uint8_t reg,uint8_t value)
{
 uint8_t bytesToSend[]={reg,value};
 My_I2C_SendBytes(I2C1,0xd0,bytesToSend,2);//发送寄存器地址和发送的内容

}
//
//@简介：读取寄存器里的数据
//@参数：reg 要读取的寄存器地址 
//@返回值：读取出来的数据
//
static uint8_t reg_read(uint8_t reg)
{
 My_I2C_SendBytes(I2C1,0xd0,&reg,1);//发送寄存器的地址
 uint8_t regValue;
 
 My_I2C_ReceiveBytes(I2C1,0xd0,&regValue,1);//读取一个字节
 return regValue;


}
//
//@简介：更新MPU6050的值
//
void App_MPU6050_Update(void)
{
  int16_t ax_raw=(int16_t)((reg_read(0x3b)<<8)+reg_read(0x3c));//ax的原始数据
	int16_t ay_raw=(int16_t)((reg_read(0x3d)<<8)+reg_read(0x3e));//ay的原始数据
	int16_t az_raw=(int16_t)((reg_read(0x3f)<<8)+reg_read(0x40));//az的原始数据
	ax=ax_raw*6.1035e-5f;
	ay=ay_raw*6.1035e-5f;
	az=ax_raw*6.1035e-5f;
	int16_t temperature_raw=(int16_t)((reg_read(0x41)<<8)+reg_read(0x42));//温度的原始数据
	temperature=temperature_raw/333.87f+21.0f;//MPU6500
	int16_t gx_raw=(int16_t)((reg_read(0x43)<<8)+reg_read(0x44));//gx的原始数据
	int16_t gy_raw=(int16_t)((reg_read(0x45)<<8)+reg_read(0x46));//gy的原始数据
	int16_t gz_raw=(int16_t)((reg_read(0x47)<<8)+reg_read(0x48));//gz的原始数据
	gx=gx_raw*6.1035e-5f;
	gy=gy_raw*6.1035e-5f;
	gz=gx_raw*6.1035e-5f;
	
	

}
//
//@简介：获取Ax加速度 x轴加速度 单位g
//
float APP_MPU6050_GetAx(void)
{
 return ax;

}
//
//@简介：获取Ay y轴加速度
//
float APP_MPU6050_GetAy(void)
{
 return ay;
}
//
//@简介：获取Az z轴加速度
//
float APP_MPU6050_GetAz(void)
{
 return az;
}
//
//@简介：获取Gx x轴角速度
//
float APP_MPU6050_GetGx(void)
{
return gx;
}
//
//@简介：获取Gy y轴角速度
//
float APP_MPU6050_GetGy(void)
{
return gy;
}
//
//@简介：获取Gz z轴角速度
//
float APP_MPU6050_GetGz(void)
{
return gz;
}
//
//@简介：获取温度 单位摄氏度
//
float APP_MPU6050_GetTemperature(void)
{
return temperature;
}
//
//@简介:获取偏航角 单位°
//
float App_MPU6050_GetYaw(void)
{
 return yaw;
}
//
//@简介:获取俯仰角 单位°
//
float App_MPU6050_GetPitch(void)
{
 return pitch;
}
//
//@简介:获取翻滚角 单位°
//
float App_MPU6050_GetRoll(void)
{
 return roll;
}
