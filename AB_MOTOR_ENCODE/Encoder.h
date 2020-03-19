#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;
extern uint32_t location1;
extern uint32_t location2;
#define reversalThreshold 1000 //进入低速模式的阈值
#define A_chennel TIM_CHANNEL_3
#define B_chennel TIM_CHANNEL_4

void Encoder_begin(void);

void locationInteg(uint32_t * location1,uint32_t * locaiton2);

#endif
