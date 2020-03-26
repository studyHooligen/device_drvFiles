# FLASH

- NAND FLASH
  - SLC：1bit/cell，速度最快，寿命最长，价格最贵
  - MLC：2bit/cell，已经淘汰
  - TLC：3bit/cell，市场主流
  - QLC：4bit/cell，速度最慢，寿命最短，价格最便宜
- NOR FLASH

## NAND FLASH

- 信号线
  - CLE：命令锁存使能，高电平代表写入命令
  - ALE：地址锁存使能，高电平代表写入地址
  - CE#：使能脚
  - RE#：读使能脚
  - WE#：写使能
  - WP#：写保护
  - R/B：ready/Busy
  - IOx：数据/地址传输线

- 存储单元
  - page
    - 数据存储区（data area）
    - 备用区域（spare area）：一般存放ECC校验值
  - block：最小擦除单位
  - plane
  - device

- 地址
  - 块地址
  - 页地址
  - 列地址

- ECC算法
  - 汉明码
    - 可纠错1bit错误
    - 检测多于1bit错误
    - STM32H7硬件支持
  - RS码
  - BCH码

- FTL闪存转换层
  - 负责坏块管理和磨损均衡