#ifndef _CHIP_TEMP_H_
#define _CHIP_TEMP_H_

#include "main.h"

float chipTemp =25.0;  //STM32芯片实时温度，范围：-40~125°C

void chipInternalTempSynchronize(void)
{
    uint16_t TS_CAL1 ,TS_CAL2;
    
    
    TS_CAL1 = *(uint16_t *)(0x1FF1E820);    //出场校准的温度30°C值
    TS_CAL2 = *(uint16_t *)(0x1FF1E840);    //出场校准的温度110°C值
    HAL_ADC_Start(&hadc3);
    chipTemp = (float)((110.0-30.0) / (TS_CAL2 - TS_CAL1)) * ((uint16_t)HAL_ADC_GetValue(&hadc3) - TS_CAL1) + 30.0;   //线性计算公式
}

#endif
