/* 
 * DRV8432.c - The C source file of the dual H-Bridge (DRV8432) driver
 * NOTE: This file is based on HAL library of stm32 platform
 *      1. Read in utf8 encoding mode！
 *      2. 本驱动程序采用差分驱动
 *      3. 注意8432的最高驱动频率为500KHz，以及高电平保持时间
 * 		4. 该文件理论上不需要用户修改，适配过程在头文件进行
 *
 * Copyright (c) 2020-, FOSH Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2020-10-25     江榕煜     first version   2373180028@qq.com
**/
#include"DRV8432.h"

/**
 * @brief   初始化驱动芯片
 * @note    调用该函数前必须已经初始化了PWM外设
 * @param    
 *          None
 * @retval   
 *          None
**/
void init8432()
{
	//关闭PWM
	HAL_TIM_PWM_Stop(&DRV8432_AB_Timer,DRV8432_A_channel);
	HAL_TIM_PWM_Stop(&DRV8432_AB_Timer,DRV8432_B_channel);
	HAL_TIM_PWM_Stop(&DRV8432_CD_Timer,DRV8432_C_channel);
	HAL_TIM_PWM_Stop(&DRV8432_CD_Timer,DRV8432_D_channel);
	
	//关闭芯片
	HAL_GPIO_WritePin(DRV8432_RST_AB_Port, DRV8432_RST_AB_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRV8432_RST_CD_Port, DRV8432_RST_CD_PIN, GPIO_PIN_RESET);
	
	HAL_Delay(1); //防止没有成功关闭！
	lock();
	
	//开启PWM
	HAL_TIM_PWM_Start(&DRV8432_AB_Timer,DRV8432_A_channel);
	HAL_TIM_PWM_Start(&DRV8432_AB_Timer,DRV8432_B_channel);
	HAL_TIM_PWM_Start(&DRV8432_CD_Timer,DRV8432_C_channel);
	HAL_TIM_PWM_Start(&DRV8432_CD_Timer,DRV8432_D_channel);
	
	//开启芯片
	HAL_GPIO_WritePin(DRV8432_RST_AB_Port, DRV8432_RST_AB_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DRV8432_RST_CD_Port, DRV8432_RST_CD_PIN, GPIO_PIN_SET);
	
}

/**
 * @brief   设置AB相全桥输出PWM占空比速度
 * @note    占空比范围：-99.99% ~ +99.99%
 * @param
 *          duty(float)：占空比
 * @retval
 *          None
**/
void set8432Speed_AB(float duty)
{
	if(duty>PWM_maxDuty) duty = PWM_maxDuty;  //防止占空比过大使得自举电路无法工作
	else if(-duty > PWM_maxDuty) duty = -PWM_maxDuty;
	
	__HAL_TIM_SetCompare(&DRV8432_AB_Timer,DRV8432_A_channel,PWM_cycleCount/2+(duty/200*PWM_cycleCount));
	__HAL_TIM_SetCompare(&DRV8432_AB_Timer,DRV8432_B_channel,PWM_cycleCount/2-(duty/200*PWM_cycleCount));
	return;
	
}

/**
 * @brief   设置CD相全桥输出PWM占空比/速度
 * @note    占空比范围：-99.99% ~ +99.99%
 * @param
 *          duty(float)：占空比
 * @retval
 *          None
**/
void set8432Speed_CD(float duty)
{
	if(duty>PWM_maxDuty) duty = PWM_maxDuty;  //防止占空比过大使得自举电路无法工作
	else if(-duty > PWM_maxDuty) duty = -PWM_maxDuty;
	
	__HAL_TIM_SetCompare(&DRV8432_CD_Timer,DRV8432_C_channel,PWM_cycleCount/2+(duty/200*PWM_cycleCount));
	__HAL_TIM_SetCompare(&DRV8432_CD_Timer,DRV8432_D_channel,PWM_cycleCount/2-(duty/200*PWM_cycleCount));
	return;
}

/**
 * @brief   刹车
 * @note    相间连接（反向电动势刹车）
 * @param
 *          None
 * @retval
 *          None
**/
void lock()
{
	__HAL_TIM_SetCompare(&DRV8432_AB_Timer,DRV8432_A_channel,PWM_cycleCount/2);
	__HAL_TIM_SetCompare(&DRV8432_AB_Timer,DRV8432_B_channel,PWM_cycleCount/2);
	__HAL_TIM_SetCompare(&DRV8432_CD_Timer,DRV8432_C_channel,PWM_cycleCount/2);
	__HAL_TIM_SetCompare(&DRV8432_CD_Timer,DRV8432_D_channel,PWM_cycleCount/2);
}

/**
 * @brief   滑行
 * @note    关闭全桥驱动（断开相间）
 * @param
 *          None
 * @retval
 *          None
**/
void slide()
{
	HAL_GPIO_WritePin(DRV8432_RST_AB_Port, DRV8432_RST_AB_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRV8432_RST_CD_Port, DRV8432_RST_CD_PIN, GPIO_PIN_RESET);
}
