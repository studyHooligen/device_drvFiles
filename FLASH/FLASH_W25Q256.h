/* 
 * FLASH_W25Q256.h - The C head file of the SPI FLASH(W25Q256) driver
 * NOTE: This file is based on HAL library of stm32 platform
 *       The default initialization device is in QSPI mode!!!
 *
 * Copyright (c) 2020-, FOSH Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2020-03-20     StudyHooligen     first version   2373180028@qq.com
 */
#ifndef _FLASH_W25Q256_H_

#define _FLASH_W25Q256_H_

#include "main.h"

/*******移植前请适配该quad spi句柄***********/
#define FLASH256QSPI hqspi

/********芯片常量定义********/
#define BlockNum    512     //块数量
#define BlockSize   (SectorNum*SectorSize)     //块大小
#define SectorNum   16      //扇区数量/块
#define SectorSize  4096    //扇区大小
#define PageNum     16      //页数量/扇区
#define PageSize    256     //页大小（单位：字节）
#define FLASH256MF  0xEF    //芯片制造序列
#define FLASH256ID  0x18    //芯片ID

/********W25Q256芯片指令集********/
#define FLASH256_ReadStatusReg1 0x05
#define FLASH256_ReadStatusReg2 0x35
#define FLASH256_ReadStatusReg3 0x15

#define FLASH256_WriteStatusReg1 0x01
#define FLASH256_WriteStatusReg2 0x31
#define FLASH256_WriteStatusReg3 0x11

#define FLASH256_ManufactureDeviceID 0x90

#define FLASH256_EnterQspiMode 0x38
#define FLASH256_Enable4ByteAddr 0xB7
#define FLASH256_WriteEnable 0x06

#define FLASH256_SetReadParameter 0xC0

#define FLASH256_FastReadData 0x0B
#define FLASH256_PageProgram 0x02
#define FLASH256_SectorErase 0x20
#define FLASH256_ChipErase 0xC7

struct FLASH_W25Q256
{
    uint8_t statusRegister[3];  //芯片内部状态寄存器情况
    uint8_t QSPI;   //芯片QSPI模式
    uint8_t ID[2];  //芯片ID
    uint8_t SectorCache[SectorSize];    //扇区读写处理缓存
    QSPI_CommandTypeDef cmdHandler;     //命令发送句柄
} deviceFLASH256;

/**
  * @brief 初始化W25Q256芯片
  * @note  调用该函数前必须已经初始化了QSPI外设，FLASH芯片上电后默认SPI传输模式
  * @param None
  * @retval 初始化是否成功，HAL_OK或者HAL_ERROR
  */
HAL_StatusTypeDef FLASH256devInit(void);

/**
  * @brief 读取FLASH内部存储数据内容
  * @param readBuf: 读出数据存储位置指针
  * @param readAddr: 数据在Flash内的地址
  * @param readNum: 读出数据个数
  * @retval None
  */
void FLASH256read(uint8_t * readBuf,uint32_t readAddr,uint16_t readNum);

/**
  * @brief 写入FLASH内部存储数据内容
  * @param writeBuf:  读出数据存储位置指针
  * @param writeAddr: 数据在Flash内的地址
  * @param writeNum:  读出数据个数
  * @retval None
  */
void FLASH256write(uint8_t * writeBuf,uint32_t writeAddr,uint16_t writeNum);

/**
  * @brief 发送FLASH命令
  * @param cmd: 命令内容
  * @param addr: 命令地址
  * @param dummyCycles: 空指令周期数
  * @param cmdMode: 命令模式
  * @param addrMode: 地址模式
  * @param addrSize: 地址长度
  * @param dataMode: 数据模式
  * @retval None
  */
void FLASH256sendCMD(uint32_t cmd,uint32_t addr,uint32_t dummyCycles,uint32_t cmdMode,uint32_t addrMode,uint32_t addrSize,uint32_t dataMode);

/**
  * @brief 擦除FLASH内部单个扇区
  * @param sectorID: 扇区号
  * @retval None
  */
void FLASH256eraseSector(uint32_t sectorID);

/**
  * @brief 写入FLASH内部单个扇区
  * @param sectorID: 扇区号
  * @param sectorData: 扇区数据指针
  * @retval None
  */
void FLASH256writeSector(uint32_t sectorID,uint8_t * sectorData);

/**
  * @brief 接受FLASH的QSPI传输数据
  * @param dataBuf: 接收到数据的缓存
  * @param dataLen: 接受数据长度
  * @retval None
  */
void FLASH256receive(uint8_t * dataBuf,uint32_t dataLen);

/**
  * @brief 发送FLASH的QSPI传输数据
  * @param dataBuf: 发送数据的缓存
  * @param dataLen: 发送数据长度
  * @retval None
  */
void FLASH256transmit(uint8_t * dataBuf,uint32_t dataLen);

/**
  * @brief 等待W25Q256退出Busy
  * @param None
  * @retval None
  */
void FLASH256waitBusy(void);

/**
  * @brief 串口打印W25Q256ID
  * @param None
  * @retval None
  */
void FLASH256getID(uint8_t * data);

/***************开始定义函数***************/
void FLASH256sendCMD(uint32_t cmd,uint32_t addr,uint32_t dummyCycles,uint32_t cmdMode,uint32_t addrMode,uint32_t addrSize,uint32_t dataMode)
{
    QSPI_CommandTypeDef * handlerPtr = &(deviceFLASH256.cmdHandler);
    
    handlerPtr->Instruction = cmd;  //指令内容
    handlerPtr->Address = addr; //指令地址
    handlerPtr->DummyCycles = dummyCycles;  //空指令周期
    handlerPtr->InstructionMode = cmdMode;  //指令模式
    handlerPtr->AddressMode = addrMode;     //地址模式
    handlerPtr->AddressSize = addrSize;     //地址长度
    handlerPtr->DataMode = dataMode;        //数据模式
    
    HAL_QSPI_Command(&FLASH256QSPI,handlerPtr,500);
}

void FLASH256receive(uint8_t * dataBuf,uint32_t dataLen)
{
    FLASH256QSPI.Instance->DLR = dataLen-1;         //配置数据传输长度
    HAL_QSPI_Receive(&FLASH256QSPI,dataBuf,5000);   //接收数据
}

void FLASH256transmit(uint8_t * dataBuf,uint32_t dataLen)
{
    FLASH256QSPI.Instance->DLR = dataLen - 1;       //配置数据传输长度
    HAL_QSPI_Transmit(&FLASH256QSPI,dataBuf,5000);  //发送数据
}

void FLASH256read(uint8_t * readBuf,uint32_t readAddr,uint16_t readNum)
{
    FLASH256waitBusy();
    FLASH256sendCMD(FLASH256_FastReadData,readAddr,8,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES);
    FLASH256receive(readBuf,readNum);
}

void FLASH256write(uint8_t * writeBuf,uint32_t writeAddr,uint16_t writeNum)
{
    uint16_t sectorPos = writeAddr/SectorSize;    //第一个写入的扇区号
    uint16_t sectorOffset = writeAddr%SectorSize;  //第一个扇区的偏移量
    uint16_t sectorRemain = SectorSize - sectorOffset; //第一扇区内剩余空间大小
    uint16_t writeSectorNum = (writeNum - sectorRemain)/SectorSize;  //写整个扇区的数量
    uint16_t writeRemainNum = writeNum - sectorRemain - writeSectorNum * SectorSize; //剩余尾扇区写入字节量
    
    //第一部分，非整扇区后写入
    if(sectorOffset)
    {
        FLASH256read(deviceFLASH256.SectorCache,sectorPos*SectorSize,SectorSize);
        FLASH256eraseSector(sectorPos);
        while(sectorRemain--)
        {
            deviceFLASH256.SectorCache[sectorOffset+sectorRemain] = writeBuf[sectorRemain];
        }
        writeBuf += (SectorSize - sectorOffset);
        //HAL_UART_Transmit(&huart1,deviceFLASH256.SectorCache,SectorSize,0XFFF);
        FLASH256waitBusy();
        FLASH256writeSector(sectorPos,deviceFLASH256.SectorCache);
    }
    else{
        writeSectorNum ++;
        sectorPos --;
    }
    
    //第二部分，整扇区写入
    while(writeSectorNum--)
    {
        sectorPos++;
        FLASH256eraseSector(sectorPos);
        FLASH256waitBusy();
        FLASH256writeSector(sectorPos,writeBuf);
        writeBuf += SectorSize;
    }
    sectorPos++;
    
    //第三部分，尾阶段非整扇区前写入
    if(!writeRemainNum) return; //没有尾扇区写
    FLASH256read(deviceFLASH256.SectorCache,sectorPos*SectorSize,SectorSize);
    FLASH256eraseSector(sectorPos);
    while(writeRemainNum--)
    {
        deviceFLASH256.SectorCache[writeRemainNum] = writeBuf[writeRemainNum];
    }
    FLASH256waitBusy();
    FLASH256writeSector(sectorPos,deviceFLASH256.SectorCache);
    
    return;
}

void FLASH256eraseSector(uint32_t sectorID)
{
    //使能芯片的写功能
    FLASH256sendCMD(FLASH256_WriteEnable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    
    FLASH256waitBusy();
    
    //发送扇区擦除命令
    FLASH256sendCMD(FLASH256_SectorErase,sectorID*SectorSize,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_NONE);
}


void FLASH256writeSector(uint32_t sectorID,uint8_t * sectorData)
{
    for(uint32_t addr = 0;addr < SectorSize;addr +=PageSize)
    {
        //等待芯片空闲
        FLASH256waitBusy();        
        
        //使能芯片的写功能
        FLASH256sendCMD(FLASH256_WriteEnable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
        
        //数据写入页
        FLASH256sendCMD(FLASH256_PageProgram,sectorID*SectorSize + addr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES);
        FLASH256transmit(sectorData+addr,PageSize);
    }
}

HAL_StatusTypeDef FLASH256devInit(void)
{
    QSPI_CommandTypeDef * handlerPtr = &(deviceFLASH256.cmdHandler);
    
    handlerPtr->SIOOMode = QSPI_SIOO_INST_EVERY_CMD;    //每次都发送指令
    handlerPtr->AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  //无交替字节
    handlerPtr->DdrMode = QSPI_DDR_MODE_DISABLE;    //不使用DDR模式
    handlerPtr->DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;   //无意义
    
    //读取芯片的状态寄存器
    FLASH256sendCMD(FLASH256_ReadStatusReg1,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE);
    FLASH256receive(deviceFLASH256.statusRegister,1);   //读取状态寄存器1
    FLASH256sendCMD(FLASH256_ReadStatusReg2,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE);
    FLASH256receive((deviceFLASH256.statusRegister)+1,1);   //读取状态寄存器2
    FLASH256sendCMD(FLASH256_ReadStatusReg3,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE);
    FLASH256receive((deviceFLASH256.statusRegister)+2,1);   //读取状态寄存器3
    
    //开启QSPI模式前需要设置状态寄存器2的QE位为1，否则写入使能QSPI命令无效
    if((deviceFLASH256.statusRegister[1] & 0x02)==0)
    {
        //使能芯片的写功能
        FLASH256sendCMD(FLASH256_WriteEnable,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
        
        //写QE位为1
        deviceFLASH256.statusRegister[1] |= 0x02;
        FLASH256sendCMD(FLASH256_WriteStatusReg2,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE);
        FLASH256transmit((deviceFLASH256.statusRegister)+1,1);
    }
    
    HAL_Delay(20);
    
    //使能芯片的写功能
    FLASH256sendCMD(FLASH256_WriteEnable,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    
    //使能FLASH的QSPI模式
    FLASH256sendCMD(FLASH256_EnterQspiMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    
    deviceFLASH256.QSPI = 1;    //已经进入QSPI模式
    
    //使能芯片的写功能
    FLASH256sendCMD(FLASH256_WriteEnable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    
    //使能FLASH的4字节模式
    FLASH256sendCMD(FLASH256_Enable4ByteAddr,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    
    //使能芯片的写功能
    FLASH256sendCMD(FLASH256_WriteEnable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    
    //设置芯片读参数为：104MHz
    FLASH256sendCMD(FLASH256_SetReadParameter,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES);
    uint8_t readParam = 3<<4;
    FLASH256transmit(&readParam,1);
    
    //使能芯片的写功能
    FLASH256sendCMD(FLASH256_WriteEnable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    
    //获取芯片ID
    FLASH256sendCMD(FLASH256_ManufactureDeviceID,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);
    FLASH256receive(deviceFLASH256.ID,2);
    
    //校验QSPI模式读取出来的ID是否准确
    if((deviceFLASH256.ID[0] == FLASH256MF) && (deviceFLASH256.ID[1] == FLASH256ID))
        return HAL_OK;
    else return HAL_ERROR;
}

void FLASH256waitBusy(void)
{
    while(1)    //等待轮询芯片退出busy
    {
        FLASH256sendCMD(FLASH256_ReadStatusReg1,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES);
        FLASH256receive(deviceFLASH256.statusRegister,1);   //读取状态寄存器1
        if((deviceFLASH256.statusRegister[0] & 0x01)!= 0x01)  break;
    }
}


void FLASH256getID(uint8_t * data)
{
    data[0] = deviceFLASH256.ID[0];
    data[1] = deviceFLASH256.ID[1];
    return;
}    

#endif
