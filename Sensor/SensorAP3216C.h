#ifndef _Sensor_AP3216_H_

#define _Sensor_AP3216_H_

#define AP3216C_DEV_ADDR 0x3C
#include "main.h"

#define Sensor3216_I2C_HANDLER hi2c2

#define AP3216C_REG_ADDR_SYSC   0x00    //System Configuration
#define AP3216C_REG_ADDR_INTS   0x01    //interrupt Status
#define AP3216C_REG_ADDR_INTC   0x02    //INT Clear Manner
#define AP3216C_REG_ADDR_IRDL   0x0A    //IR Data Low
#define AP3216C_REG_ADDR_IRDH   0x0B    //IR Data High
#define AP3216C_REG_ADDR_LSDL   0x0C    //ALS Data Low
#define AP3216C_REG_ADDR_LSDH   0x0D    //ALS Data High
#define AP3216C_REG_ADDR_PSDL   0x0E    //PS Data Low
#define AP3216C_REG_ADDR_PSDH   0x0F    //PS Data High

/***还有的配置详细相关功能的中断寄存器没有写出来****/

/**********Note***************/
//该芯片内部ADC数据转换需要时间112.5ms

/* @brief 测试AP3216设备连接情况
 * @param 无
 * @retv  测试结果，HAL_OK或者HAL_ERROR
 */
inline HAL_StatusTypeDef Sensor3216devCheck(void)
{
    return HAL_I2C_IsDeviceReady(&Sensor3216_I2C_HANDLER,AP3216C_DEV_ADDR,5,0xff);
}

/* @brief 初始化AP3216设备，为ALS+PS+IR功能全开启
 * @param 无
 * @retv  初始化结果，HAL_OK或者HAL_ERROR
 */
HAL_StatusTypeDef Sensor3216devInit(void)
{
    //检查设备在线与否
    if(Sensor3216devCheck() == HAL_ERROR) return HAL_ERROR;
    uint8_t Command[2] = {0x00,0x04};
    
    //复位设备
    HAL_I2C_Master_Transmit(&Sensor3216_I2C_HANDLER,AP3216C_DEV_ADDR,Command,2,0xff);
    HAL_Delay(20);
    
    //设置设备工作模式
    Command[1] = 0x03;
    HAL_I2C_Master_Transmit(&Sensor3216_I2C_HANDLER,AP3216C_DEV_ADDR,Command,2,0xff);
    Command[1] = 0xff;
    
    //检查设置结果
    HAL_I2C_Master_Transmit(&Sensor3216_I2C_HANDLER,AP3216C_DEV_ADDR,Command,1,0xff);
    HAL_I2C_Master_Receive(&Sensor3216_I2C_HANDLER,AP3216C_DEV_ADDR,(Command+1),1,0xff);
    if(Command[1] == 0x03)
        return HAL_OK;
    return HAL_ERROR;
}

/* @brief 读取AP3216设备转换结果数据
 * @NOTE! 调用数据获取函数时间间隔必须大于112.5ms
 * @param IR红外数据
 * @param PS距离传感器数据
 * @param ALS数字环境光传感器数据
 * @retv  初始化结果，HAL_OK或者HAL_ERROR
 */
HAL_StatusTypeDef Sensor3216getData(uint16_t * ir,uint16_t * ps,uint16_t * als)
{
    uint8_t cache[6];
    uint8_t dataBaseAddr = AP3216C_REG_ADDR_IRDL;
    
    //读取6个结果寄存器内容
    for(int i =0;i<6;i++)
    {            
        HAL_I2C_Master_Transmit(&Sensor3216_I2C_HANDLER,AP3216C_DEV_ADDR,&dataBaseAddr,1,0xff);
        HAL_I2C_Master_Receive(&Sensor3216_I2C_HANDLER,AP3216C_DEV_ADDR,(cache+i),1,0xff);
        dataBaseAddr ++;
    }
    
    //解析红外数据
    if(cache[0] & 0x80)
            *ir = 0;
    else
        *ir = ( ((uint16_t)cache[1]) << 2 ) | (cache[0] & 0x03);
    
    //解析光数据
    *als = ( ((uint16_t)cache[3]) << 8 ) | cache[2];
    
    //解析距离数据,还可解析数物体方向，此处不写
    if( (cache[4]&0x40) && (cache[5]&0x40))
        *ps = 0;
    else
        *ps = ( (uint16_t)(cache[5]&0x3f) << 4) | (cache[4] & 0x0f);
    return HAL_OK;
}

#endif
