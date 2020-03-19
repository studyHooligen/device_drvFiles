#ifndef SDRAM_H

#define SDRAM_H

#include "main.h"

#define SDRAM_MODE 0x220    //突发读单点写，CAS=2，连续地址，突发长度1
#define SDRAM_CAPACITY 32*1024*1024 //RAM容量
#define SDRAM_WORD_SIZE 16  //RAM字长
#define SDRAM_BASE_ADDR 0xC0000000

HAL_StatusTypeDef SDRAM_TEST(void); //测试RAM读取是否正常
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef* handler);
HAL_StatusTypeDef SDRAM_Send_Cmd(SDRAM_HandleTypeDef* handler,uint8_t bankx,uint8_t cmd,uint8_t refresh,uint16_t regval);

HAL_StatusTypeDef SDRAM_TEST(void)
{
    uint32_t temp=0;
    uint32_t i=0;
    for(i =0;i < SDRAM_CAPACITY;i += SDRAM_WORD_SIZE*1024)
    {
        *(uint32_t *)(SDRAM_BASE_ADDR + i) = temp;
        temp++;
    }
    
    for(i = 0, temp = 0;i < SDRAM_CAPACITY;i += SDRAM_WORD_SIZE*1024)
    {
        if(temp != *(uint32_t *)(SDRAM_BASE_ADDR + i))
            return HAL_ERROR;
    }
    return HAL_OK;
}

/*
 *  @brief 发送SDRAM命令
 */
HAL_StatusTypeDef SDRAM_Send_Cmd(SDRAM_HandleTypeDef* handler,uint8_t bankx,uint8_t cmd,uint8_t refresh,uint16_t regval)
{
    FMC_SDRAM_CommandTypeDef command;
    
    if(bankx ==1) command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    else if(bankx == 2) command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
    
    command.CommandMode = cmd;  //命令
    command.AutoRefreshNumber = refresh;    //自刷新次数
    command.ModeRegisterDefinition = regval;    //写入模式寄存器的值
    
    return HAL_SDRAM_SendCommand(handler,&command,0x1000);
}

void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef* handler)
{   //这个序列和芯片有关
    SDRAM_Send_Cmd(handler,1,FMC_SDRAM_CMD_CLK_ENABLE,1,0); //时钟配置使能
    HAL_Delay(1);   //至少延迟200us
    SDRAM_Send_Cmd(handler,1,FMC_SDRAM_CMD_PALL,1,0);   //对所有区域预充电
    SDRAM_Send_Cmd(handler,1,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);   //设置自刷新次数
    SDRAM_Send_Cmd(handler,1,FMC_SDRAM_CMD_LOAD_MODE,1,SDRAM_MODE); //发送模式寄存器命令
}

#endif
