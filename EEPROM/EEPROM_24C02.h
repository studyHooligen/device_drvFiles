#ifndef _EEPROM_24C02_H_

#define _EEPROM_24C02_H_

#include "main.h"

#define EEPROM_DEV_ADDR 0xa0
#define IIC_CLK_FRE 400000
#define EEPROM_CAP 256
#define EEPROM_PAGE_N 32
#define EEPROM_PAGE_SIZE 8

/* @brief 测试EEPROM头尾存储点读写性能
 * @param 无
 * @retv  测试结果，HAL_OK或者HAL_ERROR
 */
HAL_StatusTypeDef EEPROM24C02memoryTest(void)
{
    uint8_t testData[2] = {'P','R'};
    HAL_I2C_Mem_Write(&hi2c2,EEPROM_DEV_ADDR,0x00,1,testData,1,0xff);
    HAL_Delay(5);
    HAL_I2C_Mem_Write(&hi2c2,EEPROM_DEV_ADDR,EEPROM_CAP - 1,1,testData+1,1,0xff);
    HAL_Delay(5);
    uint8_t cache[2];
    HAL_I2C_Mem_Read(&hi2c2,EEPROM_DEV_ADDR,0x00,1,cache,1,0xff);
    HAL_I2C_Mem_Read(&hi2c2,EEPROM_DEV_ADDR,EEPROM_CAP - 1,1,cache+1,1,0xff);
    if(cache[0] == testData[0] && cache[1] == testData[1])
        return HAL_OK;
    else
        return HAL_ERROR;
    return HAL_OK;
}

/* @brief 测试EEPROM页读写性能
 * @param 无
 * @retv  测试结果，HAL_OK或者HAL_ERROR
 */
HAL_StatusTypeDef EEPROM24C02memoryPageTest(void)
{
    uint8_t testData[EEPROM_PAGE_SIZE];
    uint8_t testCache[EEPROM_PAGE_SIZE];
    uint32_t i;
    for(i = 0;i<EEPROM_PAGE_SIZE;i++)
        testData[i] = 'a' + i;
    
    HAL_I2C_Mem_Write(&hi2c2,EEPROM_DEV_ADDR,0x00,1,testData,EEPROM_PAGE_SIZE,0xff);
    HAL_Delay(5);
    HAL_I2C_Mem_Read(&hi2c2,EEPROM_DEV_ADDR,0x00,1,testCache,EEPROM_PAGE_SIZE,0xff);
    
    for(i = 0;i<EEPROM_PAGE_SIZE;i++)
    {
        if(testData[i] != testCache[i]) return HAL_ERROR;
    }
    return HAL_OK;
}

#endif
