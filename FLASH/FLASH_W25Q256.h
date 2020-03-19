#ifndef _FLASH_W25Q256_H_

#define _FLASH_W25Q256_H_

#include "main.h"

/*******移植前请适配该quad spi句柄***********/
#define FLASH256QSPI hqspi

/********芯片常量定义********/
#define SectorSize 4096

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
#define FLASH256_WritEnable 0x06

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
  * @brief 读取FLASH内部存储数据内容
  * @param readBuf: 读出数据存储位置指针
  * @param readAddr: 数据在Flash内的地址
  * @param readNum: 读出数据个数
  * @retval None
  */
void FLASH256read(uint8_t * readBuf,uint32_t readAddr,uint16_t readNum);

/**
  * @brief 写入FLASH内部存储数据内容
  * @param writeBuf: 读出数据存储位置指针
  * @param writeAddr: 数据在Flash内的地址
  * @param writeNum: 读出数据个数
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
  * @brief 初始化W25Q256芯片
  * @note  调用该函数前必须已经初始化了QSPI外设
  * @param None
  * @retval 初始化是否成功，HAL_OK或者HAL_ERROR
  */
HAL_StatusTypeDef FLASH256devInit(void);

/**
  * @brief 接受FLASH的QSPI传输数据
  * @param dataBuf: 接收到数据的缓存
  * @param dataLen: 接受数据长度
  * @retval None
  */
inline void FLASH256receive(uint8_t * dataBuf,uint32_t dataLen);

/**
  * @brief 发送FLASH的QSPI传输数据
  * @param dataBuf: 发送数据的缓存
  * @param dataLen: 发送数据长度
  * @retval None
  */
inline void FLASH256transmit(uint8_t * dataBuf,uint32_t dataLen);

/**
  * @brief 使能W25Q256的QSPI传输功能
  * @param None
  * @retval None
  */
inline void FLASH256enableQSPI(void);

/**
  * @brief 使能W25Q256的写入功能
  * @param None
  * @retval None
  */
inline void FLASH256enableWrite(void);

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
    
    HAL_QSPI_Command(&FLASH256QSPI,handlerPtr,5000);
}

inline void FLASH256receive(uint8_t * dataBuf,uint32_t dataLen)
{
    FLASH256QSPI.Instance->DLR = dataLen-1;         //配置数据传输长度
    HAL_QSPI_Receive(&FLASH256QSPI,dataBuf,5000);   //接收数据
}

inline void FLASH256transmit(uint8_t * dataBuf,uint32_t dataLen)
{
    FLASH256QSPI.Instance->DLR = dataLen - 1;       //配置数据传输长度
    HAL_QSPI_Transmit(&FLASH256QSPI,dataBuf,5000);  //发送数据
}

void FLASH256read(uint8_t * readBuf,uint32_t readAddr,uint16_t readNum)
{
    
}

//********************TODO*************************
HAL_StatusTypeDef FLASH256devInit(void)
{
    QSPI_CommandTypeDef * handlerPtr = &(deviceFLASH256.cmdHandler);
    
    handlerPtr->SIOOMode = QSPI_SIOO_INST_EVERY_CMD;    //每次都发送指令
    handlerPtr->AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  //无交替字节
    handlerPtr->DdrMode = QSPI_DDR_MODE_DISABLE;    //不使用DDR模式
    handlerPtr->DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;   //无意义
    
    //使能FLASH的QSPI模式
    
}

#endif
