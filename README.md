# FOSHDP描述

- 项目全称：免费free开源Open硬件Hardware驱动Driver计划Project
- 基于STM32HAL库写的常用芯片、设备驱动库

# License

本框架中的所有代码都采用了Apache协议，使用者可以闭源修改后的代码，但是必须保留源自作者的版权声明

# 使用方式

## 文件结构

``` 
--+ device_drvFiles
    --+ EEPROM
        --- EEPROM_24C02.h
        --- ...
        --- 其他型号设备驱动文件
    --+ FLASH
    --+ ...
    --+ 按类型区分的设备文件夹
```

## 文件说明

- 采用Apache协议开源
- 非超巨大规模设备驱动文件，采用纯C头文件格式
- 头文件格式
  - 权限声明
  - 预编译选项
  - 移植前必修改量定义(如HAL外设句柄）
  - 其他常量定义
  - 驱动结构定义
  - 驱动函数声明and注释说明
  - 驱动函数详细定义

## 使用步骤

**Tip**：以FLASH芯片W25Q256为例

- 在CubeMX中，设置STM32芯片上与FLASH通信的QSPI外设参数
- 使用CubeMX生成工程
  - 工程中会自动有在cubeMX中配好的QSPI外设初始化函数
  - 工程中的main.h头文件链接起所有的库文件（所以该工程中的每一个芯片H文件都默认添加了"main.h"）
- 打开工程，在工程编译目录中添加上FLASH_W25Q256.h文件（建议拷贝到当前工程文件目录后再添加）
- 打开FLASH_W25Q256.h头文件
- 更改FLASH256QSPI句柄的定义（改为CubeMX生成的句柄，如：hqspi）
- 移植完成，可以开始使用头文件中声明的所有功能函数了

# 合作开发

欢迎和我一起开发这个STM32HAL设备驱动文件库

- QQ:2373180028
- mail:2373180028@qq.com
- 地址:华中科技大学启明学院6楼
