/* 
 * drv8889.h - The C source file of the SPI PWM MotorDriver(drv8889) driver
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
#include "DRV8889.h"


typedef struct writeDataStructure{
	uint8_t  address;	//MSB
	uint8_t data;		//LSB
}SPIsendData;

typedef struct getDataStructure{
	uint8_t status;		//MSB
	uint8_t report;		//LSB
}SPIgetData;

#define DRV8889ADDR_FAULT_STATUS			0X00
#define DRV8889ADDR_DIAG_STATUS_1 			0X02
#define DRV8889ADDR_DIAG_STATUS_2 			0X04
#define DRV8889ADDR_CTRL1					0X06
#define DRV8889ADDR_CTRL2					0X08
#define DRV8889ADDR_CTRL3					0X0a
#define DRV8889ADDR_CTRL4					0x0c
#define DRV8889ADDR_CTRL5					0x0e
#define DRV8889ADDR_CTRL6					0x10
#define DRV8889ADDR_CTRL7					0x12

//如果使用了操作系统，最好把延迟函数改一下来提高系统性能
void DRV8889_Reset()  
{
	HAL_GPIO_WritePin(DRV8889nSLEEP_GPIO_Port,DRV8889nSLEEP_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRV8889DRVoff_GPIO_Port,DRV8889DRVoff_Pin,GPIO_PIN_SET);
	HAL_Delay(1);	//75us设备即已重启
	HAL_GPIO_WritePin(DRV8889nSLEEP_GPIO_Port,DRV8889nSLEEP_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(DRV8889DRVoff_GPIO_Port,DRV8889DRVoff_Pin,GPIO_PIN_RESET);
}

DRV8889Status DRV8889_ClearErrorFlag(drv8889_status * deviceStatusHandler,drv8889_ctrl* deviceCtrlHandler)
{	//软件清除错误，向CTRL4寄存器的CLR_FLT[7]写1
	SPIsendData sendCache;
    SPIgetData getCache;
    
	sendCache.address = DRV8889ADDR_CTRL4;	//写控制寄存器
    sendCache.data = (deviceCtrlHandler->CTRL4) | 0x80;
    HAL_SPI_TransmitReceive(&DRV8889SPI,(uint8_t *)&sendCache,(uint8_t *)&getCache,2,1);
	HAL_Delay(1);
	
	sendCache.address = 0x40 | DRV8889ADDR_FAULT_STATUS;	//读控制寄存器
	sendCache.data = 0x00;
    HAL_SPI_TransmitReceive(&DRV8889SPI,(uint8_t *)&sendCache,(uint8_t *)&getCache,2,1);
	deviceStatusHandler->FAULT_STATUS = getCache.report;
	if(getCache.report != 0x00)
	{	//错误依旧存在
		return DRV8889_ERROR;
	}
	else
	{	//错误已经清除
		deviceStatusHandler->MOS_STATUS = 0;
		deviceStatusHandler->TempMotorStatus = 0;
		return DRV8889_OK;
	}
	
}


void DRV8889_Sleep()
{
	HAL_GPIO_WritePin(DRV8889nSLEEP_GPIO_Port,DRV8889nSLEEP_Pin,GPIO_PIN_RESET);
}

void DRV8889_ExitSleep()
{
	HAL_GPIO_WritePin(DRV8889nSLEEP_GPIO_Port,DRV8889nSLEEP_Pin,GPIO_PIN_SET);
}

void DRV8889_SetCurrentDiv(drv8889_ctrl* deviceCtrlHandler,DRV8889_MicroStepMode divMode)
{	//写控制寄存器的CTRL3的[3:0]
	SPIsendData sendCache;
    
	sendCache.address = DRV8889ADDR_CTRL3;	//写控制寄存器
    sendCache.data = ((deviceCtrlHandler->CTRL3) & 0xf0) | divMode ;
	deviceCtrlHandler->CTRL3 = sendCache.data;
    HAL_SPI_Transmit(&DRV8889SPI,(uint8_t *)&sendCache,2,1);
}

void DRV8889_SetSlewRate(drv8889_ctrl* deviceCtrlHandler,DRV8889_SR devSR)
{	//写控制寄存器的CTRL1的[1:0]
	SPIsendData sendCache;
    
	sendCache.address = DRV8889ADDR_CTRL1;	//写控制寄存器
    sendCache.data = ((deviceCtrlHandler->CTRL1) & 0xfc) | devSR ;
	deviceCtrlHandler->CTRL1 = sendCache.data;
    HAL_SPI_Transmit(&DRV8889SPI,(uint8_t *)&sendCache,2,1);
}

void DRV8889_SetMaxCurrentScalar(drv8889_ctrl* deviceCtrlHandler,DRV8889_CurrentScalar currScal)
{	//写控制寄存器的CTRL1的[1:0]
	SPIsendData sendCache;
    
	sendCache.address = DRV8889ADDR_CTRL1;	//写控制寄存器
    sendCache.data = ((deviceCtrlHandler->CTRL1) & 0x0f) | (currScal << 4) ;
	deviceCtrlHandler->CTRL1 = sendCache.data;
    HAL_SPI_Transmit(&DRV8889SPI,(uint8_t *)&sendCache,2,1);
}
