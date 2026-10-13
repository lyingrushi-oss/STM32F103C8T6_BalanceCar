#include "encoder_test.h"
#include "app_encoder.h"
#include "app_usart2.h"
#include "delay.h"
void Encoder_Test(void)
{
 App_encoder_Init();
 App_USART2_Init();
 
 while(1)
 {
  Delay(50);
	int64_t encoder_l=encoder_l_pos();
	int64_t encoder_r=encoder_r_pos();
	My_USART_Printf(USART2,"%d,%d\n",(int32_t)encoder_l,(int32_t)encoder_r);
 
 }

}
