/* 
 * drv8889.h - The C head file of the SPI PWM MotorDriver(drv8889) driver
 * NOTE: This file is based on HAL library of stm32 platform
 *
 * Copyright (c) 2020-, FOSH Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2020-04-03     StudyHooligen     first version   2373180028@qq.com
 */
#ifndef _DRV8889_H_
//powered by studyhooligen@HUST_STI_K10
#define _DRV8889_H_
#include "main.h"

/******使用前请适配以下通用GPIO口和SPI句柄******/
#define DRV8889nSLEEP_Pin GPIO_PIN_11		//芯片休眠引脚
#define DRV8889nSLEEP_GPIO_Port GPIOE

#define DRV8889DRVoff_Pin GPIO_PIN_12		//使能MOS-H桥关闭引脚
#define DRV8889DRVoff_GPIO_Port GPIOE

#define DRV8889DIR_Pin GPIO_PIN_13			//方向引脚
#define DRV8889DIR_GPIO_Port GPIOE

#define DRV8889STEP_Pin GPIO_PIN_14		//PWM引脚（STEP）
#define DRV8889STEP_GPIO_Port GPIOE

#define DRV8889SPI hspi4	//SPI外设句柄

#define DRV8889Status HAL_StatusTypeDef	//设备返回状态直接采用HAL库定义状态
#define DRV8889_OK HAL_OK
#define DRV8889_ERROR HAL_ERROR


/********************END*******************/

//芯片状态句柄
typedef struct drv8889_status
{
	uint8_t FAULT_STATUS;			//芯片错误状态位
	uint8_t MOS_STATUS;				//MOS管H桥过流状态
	uint8_t TempMotorStatus;		//过温报告及负载情况
}drv8889_status;

/*********status register from MSE to LSE***************
--FAULT_STATUS(r)
	--FAULT					this bit = ~nFault Pin
	--SPI_ERROR				if SPI clock happen error ,this bit become high
	--UVLO					Indicates an undervoltage lockout fault condition. Latched high after power-up, remains set until it is cleared through the CLR_FLT bit or an nSLEEP reset pulse.
	--CPUV					Indicates charge pump undervoltage fault condition. Latched high after power-up, remains set until it is cleared through the CLR_FLT bit or an nSLEEP reset pulse. 
	--OCP					Indicates overcurrent fault condition
	--STL					Indicates motor stall condition
	--TF					Logic OR of the overtemperature warning, undertemperature warning and overtemperature shutdown.
	--OL					Indicates open-load condition
	
--DIAG_STATUS_1(r)
	--OCP_LS2_B				Indicates overcurrent fault on the low-side FET of half bridge 2 in BOUT 
	--OCP_HS2_B				Indicates overcurrent fault on the high-side FET of half bridge 2 in BOUT 
	--OCP_LS1_B				Indicates overcurrent fault on the low-side FET of half bridge 1 in BOUT 
	--OCP_HS1_B 			Indicates overcurrent fault on the high-side FET of half bridge 1 in BOUT 
	--OCP_LS2_A 			Indicates overcurrent fault on the low-side FET of half bridge 2 in AOUT 
	--OCP_HS2_A 			Indicates overcurrent fault on the high-side FET of half bridge 2 in AOUT 
	--OCP_LS1_A 			Indicates overcurrent fault on the low-side FET of half bridge 1 in AOUT 
	--OCP_HS1_A 			Indicates overcurrent fault on the high-side FET of half bridge 1 in AOUT

--DIAG_STATUS_2(r)
	--UTW					Indicates undertemperature warning
	--OTW					overtemperature warning
	--OTS					overtemperature shutdown
	--STL_LRN_OK			stall detection learning is successful 
	--STALL					motor stall condition 
	--OL_B 					open-load detection on BOUT 
	--OL_A 					open-load detection on AOUT

************************/

//芯片控制句柄
typedef struct drv8889_ctrl
{
	uint8_t CTRL1;		//DAC范围、输出摆率
	uint8_t CTRL2;		//输出使能、PWM关断时间、衰减模式
	uint8_t CTRL3;		//方向源、步进源、微步模式
	uint8_t CTRL4;		//所有错误清除位！芯片锁！掉载检测使能、过流过温模式选择、温度报告选择
	uint8_t CTRL5;		//电机失步检测学习使能！使能失步检测、失步检测报告
	uint8_t CTRL6;		//失速阈值
	uint8_t CTRL7;		//检测到的失速量
}drv8889_ctrl;


/*******************control register***********************
--CTRL1(R/W)
	--TRQ_DAC[7:4]			default:0000b(100%)
	--SLEW_RATE[1:0]		default:00b(10V/us)
	
--CTRL2(R/W)
	--DIS_OUT[7]			default:0b(normal output)
	--TOFF[4:3]				default:01b(16us)
	--DECAY[2:0]			default:111b(smart tune ripple control)
	
--CTRL3(R/W)
	--DIR[7]				default:0b	attention:Direction input. Logic '1' sets the direction of stepping, when SPI_DIR = 1. 
	--STEP[6]				default:0b	attention:Step input. Logic '1' causes the indexer to advance one step, when SPI_STEP = 1. 
	--SPI_DIR[5]			default:0b(outputs follow input pin DIR)
	--SPI_STEP[4]			default:0b(outputs follow input pin STEP)
	--MICROSTEP_MODE[3:0]	default:0000b(full step with 100% current)

--CTRL4(R/W)
	--CLR_FLT[7]     		default:0b  attention:write 1 to clear all latched fault bits!
	--LOCK[6:4]      		default:011b(unlock all register)
	--EN_OL[3]       		default:0b  attention:write 1 to enable open load detection
	--OCP_MODE[2]    		default:0b(Overcurrent condition causes a latched fault )
	--OTSD_MODE[1]   		default:0b(Overtemperature condition will cause latched fault )
	--TW_REP[0]      		default:0b(Overtemperature or undertemperature warning is not reported on the nFAULT line )

--CTRL5(R/W)
	--STL_LRN[5]      		default:0b  attention:Write '1' to learn stall count for stall detection. This bit automatically returns to '0' when the stall learning process is complete
	--DIS_STL[4]			default:0b(stall detecion is enabled)
	--STL_REP[3]        	default:1b(stall detecion is reported on nFAULT)
	
--CTRL6[R/W]
	--STALL_TH[7:0]  		default:00001111b
	
--CTRL7(R/W)
--TRQ_COUNT[7:0]   			default:11111111b
*/

//枚举类型：电机微分步模式
typedef enum DRV8889_MicroStepMode {
	DRV8889_STEP_FULL_100C 	= 	0x0,		//DEFAULT
	DRV8889_STEP_FULL_71C 	=	0x1,
	DRV8889_STEP_2div_NoCir = 	0x2,
	DRV8889_STEP_2div 		= 	0x3,
	DRV8889_STEP_4div 		= 	0x4,
	DRV8889_STEP_8div 		= 	0x5,
	DRV8889_STEP_16div 		= 	0x6,
	DRV8889_STEP_32div 		= 	0x7,
	DRV8889_STEP_64div 		= 	0x8,
	DRV8889_STEP_128div 	= 	0x9,
	DRV8889_STEP_256div 	= 	0xA
} DRV8889_MicroStepMode;

//枚举类型：输出口压摆率,单位：V/us
typedef enum DRV8889_SR{
	DRV8889_SR_10 	= 0,
	DRV8889_SR_35 	= 1,
	DRV8889_SR_50 	= 2,
	DRV8889_SR_105 	= 3
} DRV8889_SR;

//枚举类型：最大电机电流
typedef enum DRV8889_CurrentScalar{
	DRV8889_CurrentScalar_100P = 0x0,
	DRV8889_CurrentScalar_93P75 = 0x1,
	DRV8889_CurrentScalar_87P5 = 0x2,
	DRV8889_CurrentScalar_81P25 = 0x3,
	DRV8889_CurrentScalar_75P = 0x4,
	DRV8889_CurrentScalar_68P75 = 0x5,
	DRV8889_CurrentScalar_62P5 = 0x6,
	DRV8889_CurrentScalar_56P25 = 0x7,
	DRV8889_CurrentScalar_50P = 0x8,
	DRV8889_CurrentScalar_43P75 = 0x9,
	DRV8889_CurrentScalar_37P5 = 0xa,
	DRV8889_CurrentScalar_31P25 = 0xb,
	DRV8889_CurrentScalar_25P = 0xc,
	DRV8889_CurrentScalar_18P75 = 0xd,
	DRV8889_CurrentScalar_12P5 = 0xe,
	DRV8889_CurrentScalar_6P25 = 0xf
} DRV8889_CurrentScalar;


/**
  * @brief 复位重启芯片
  * @param None
  * @retval None
  */
void DRV8889_Reset();

/**
  * @brief 清除所有错误位
  * @param deviceStatusHandler，状态句柄
  * @param deviceCtrlHandler，控制句柄
  * @retval DRV8889Status，返回清除错误成功（DRV8889_OK)或者失败(DRV8889_ERROR)
  */
DRV8889Status DRV8889_ClearErrorFlag(drv8889_status * deviceStatusHandler,drv8889_ctrl* deviceCtrlHandler);

/**
  * @brief 芯片进入睡眠状态
  * @param None
  * @retval None
  */
void DRV8889_Sleep();

/**
  * @brief 芯片回到工作状态
  * @param None
  * @retval None
  */
void DRV8889_ExitSleep();

/**
  * @brief 芯片进入寄存器状态锁定模式
  * @param None
  * @retval None
  */
void DRV8889_LockMotorReg();

/**
  * @brief 设置芯片的微步模式
  * @param deviceCtrlHandler，控制句柄
  * @param divMode为微步模式，输入参数为DRV8889_MicroStepMode类型枚举量
  * @retval None
  */
void DRV8889_SetCurrentDiv(drv8889_ctrl* deviceCtrlHandler,DRV8889_MicroStepMode divMode);

/**
  * @brief 设置芯片的输出口压摆率
  * @param deviceCtrlHandler，控制句柄
  * @param divMode，微步模式，输入参数为DRV8889_SR类型枚举量
  * @retval None
  */
void DRV8889_SetSlewRate(drv8889_ctrl* deviceCtrlHandler,DRV8889_SR devSR);

/**
  * @brief 设置芯片的电机电流最大电流比
  * @param deviceCtrlHandler，控制句柄
  * @param divMode为微步模式，输入参数为DRV8889_CurrentScalar类型枚举量
  * @retval None
  */
void DRV8889_SetMaxCurrentScalar(drv8889_ctrl* deviceCtrlHandler,DRV8889_CurrentScalar currScal);

/**
  * @brief 将芯片寄存器参数读出
  * @param deviceStatusHandler，状态句柄
  * @param deviceCtrlHandler，控制句柄
  * @retval None
  */
void DRV8889_getAllReg(drv8889_status * deviceStatusHandler,drv8889_ctrl* deviceCtrlHandler);

/**
  * @brief 将调整的芯片寄存器参数刷入寄存器
  * @note	适用于一次性修改所有参数后一次性写入
  * @param deviceStatusHandler，状态句柄
  * @param deviceCtrlHandler，控制句柄
  * @retval None
  */
void DRV8889_flushReg(drv8889_status * deviceStatusHandler,drv8889_ctrl* deviceCtrlHandler);


#endif
