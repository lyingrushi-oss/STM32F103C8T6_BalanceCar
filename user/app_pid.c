#include "app_pid.h"
#include "delay.h"
//
//@简介：初始化PID模块
//
void App_PID_Init(PID_TypeDef*PID_Struct,float Kp,float Ki,float Kd)
{
	PID_Struct->Kp=Kp;
	PID_Struct->Ki=Ki;
	PID_Struct->Kd=Kd;
	PID_Struct->sp=0.0f;
	PID_Struct->err_k_1=0.0f;
	PID_Struct->err_int_k_1=0.0f;
	PID_Struct->t_k_1=0;
	PID_Struct->upper_limit=3.4e38f;
	PID_Struct->lower_limit=-3.4e38f;




}
//
//@简介：改变setpoint设定值
//
void App_PID_SPChange(PID_TypeDef*PID_Struct,float sp)
{
 PID_Struct->sp=sp;
	//return sp;

}
//
//@简介：计算输出结果
//
float PID_Compute(PID_TypeDef *PID,float FB)
{
 float err=PID->sp-FB;
	uint64_t t_k=GetUs();
	float delta_t=t_k-PID->t_k_1;
	float err_int=0.0f;
	float err_dev=0.0f;
	if(PID->t_k_1!=0)//不是第一次运行
	{
	 err_int=PID->err_int_k_1+(err+PID->err_k_1)*delta_t/2.0f;
   err_dev=(err-PID->err_k_1)/delta_t;
	
	}
//	else//第一次运行 
//	{
//	 err
//	
//	}
//	
	float COp=PID->Kp*err;
	float COi=PID->Ki*err_int;
	float COd=PID->Kd*err_dev;
	float CO=COp+COi+COd;
	//float Ua=CO;
	//更新PID里面的值
	PID->t_k_1=t_k;
	PID->err_k_1=err;
	PID->err_int_k_1=err_int;
	
	//输出限幅
	if(CO>PID->upper_limit) CO=PID->upper_limit;
	if(CO<PID->lower_limit) CO=PID->lower_limit;
	
	//积分限幅
	if(PID->err_int_k_1>PID->upper_limit) PID->err_int_k_1=PID->upper_limit;
	if(PID->err_int_k_1<PID->lower_limit) PID->err_int_k_1=PID->lower_limit;

  return CO;

}
//
//@简介：设置输出范围上下限
//@参数：upper 上限 lower 下限
void PID_LimitConfig(PID_TypeDef*PID,float upper,float lower)
{
 PID->upper_limit=upper;
	PID->lower_limit=lower;

}
//
//@简介：对PID进行复位操作
//
void PID_Reset(PID_TypeDef*PID)
{
 PID->err_k_1=0;
 PID->err_int_k_1=0.0f;
 PID->t_k_1=0;
	


}
