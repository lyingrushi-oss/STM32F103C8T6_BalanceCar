#ifndef APP_ENCODER_H
#define APP_ENCODER_H
//#include "stm32f10x.h"
#include "stm32f10x.h"
void App_encoder_Init(void);
int64_t encoder_r_pos(void);
int64_t encoder_l_pos(void);

#endif
