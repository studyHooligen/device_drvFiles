/* 
 * DRV8432.h - The C head file of the dual H-Bridge (DRV8432) driver
 * NOTE: This file is based on HAL library of stm32 platform
 *      1. Read in utf8 encoding mode！
 *      2. 本驱动程序采用差分驱动
 *      3. 注意8432的最高驱动频率为500KHz，以及高电平保持时间
 *
 * Copyright (c) 2020-, FOSH Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2020-10-25     江榕煜     first version   2373180028@qq.com
**/
#ifndef DRV8432_H
#define DRV8432_H

#include "main.h"

/*********NOTICE*********
 * 使用前请适配以下参数：
 * 1. PWM计数周期
 * 2. 最大占空比
 * 3. PWM通道定义（HAL对象）
 * 4. 控制GPIO口定义（HAL对象）
 * 如果不会请联系：2373180028@qq.com
******* NOTICE-END*******/

//采用三角计数模式，PWM模式一输出
#define timer_countFrequency 240000000  //240M
#define PWM_cycleCount 1199		//1199个计数周期
#define PWM_maxDuty 99  //最大占空比周期

//PWM通道定义
#define DRV8432_AB_Timer htim5
#define DRV8432_CD_Timer htim5
extern TIM_HandleTypeDef htim5;
#define DRV8432_A_channel TIM_CHANNEL_1
#define DRV8432_B_channel TIM_CHANNEL_2
#define DRV8432_C_channel TIM_CHANNEL_3
#define DRV8432_D_channel TIM_CHANNEL_4

//复位GPIO定义
#define DRV8432_RST_AB_Port RST_AB_GPIO_Port
#define DRV8432_RST_CD_Port RST_CD_GPIO_Port

#define DRV8432_RST_AB_PIN RST_AB_Pin
#define DRV8432_RST_CD_PIN RST_CD_Pin

/*********NOTICE*********
 * 接下来是你可用的封装接口：
 * 1. 设备初始化
 * 2. 全桥输出占空比设置
 * 3. 刹车
 * 4. 滑行
******* NOTICE-END*******/

/**
 * @brief   初始化驱动芯片
 * @note    调用该函数前必须已经初始化了PWM外设
 * @param    
 *          None
 * @retval   
 *          None
**/
void init8432(void);

/**
 * @brief   设置AB相全桥输出PWM占空比速度
 * @note    占空比范围：-99.99% ~ +99.99%
 * @param
 *          duty(float)：占空比
 * @retval
 *          None
**/
void set8432Speed_AB(float duty);

/**
 * @brief   设置CD相全桥输出PWM占空比/速度
 * @note    占空比范围：-99.99% ~ +99.99%
 * @param
 *          duty(float)：占空比
 * @retval
 *          None
**/
void set8432Speed_CD(float duty);

/**
 * @brief   刹车
 * @note    相间连接（反向电动势刹车）
 * @param
 *          None
 * @retval
 *          None
**/
void lock(void);

/**
 * @brief   滑行
 * @note    关闭全桥驱动（断开相间）
 * @param
 *          None
 * @retval
 *          None
**/
void slide(void);

#endif
