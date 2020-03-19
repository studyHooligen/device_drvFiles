# Cube工程结构
- driver
    - Application/MDK-ARM
        - 启动文件：startup_stm32xxxx.s
    - Application/User
        - 主程序文件：main.c
        - 中断服务定义文件：stm32xxxx_it.c
        - 进行芯片级别硬件初始化设置：stm32xxxx_hal_msp.c
    - Drivers/STM32xxxx_HAL_Driver
        标准HAL库驱动 
    - Drivers/CMSIS
        - 定义系统初始化函数及时钟更新函数：system_stm32xxxx.c

# HAL库小知识
- _IO成员变量修饰符：相当于const、variable等
- HAL_Delay()中断原理，所以有时会误差严重
- 注意H7的Dcache功能，若不开启强制透写，可能出现与ram数据不同步bug

# 调试编程小知识
- 在不进行固件升级下，ST-link最大速度为1.8MHz
- _weak函数修饰符：用户可以再次定义为自己的函数内容
- 内存定位
    ```C
    uint8_t mpudata[128] __attribute__((at(0x20002000)));
    ```

# GPIO
```C
HAL_GPIO_WritePin(GPIOx,GPIO_PIN_x,GPIO_PIN_xxx);   //写端口
HAL_GPIO_ReadPin(GPIOx,GPIO_PIN_x); //读端口状态
HAL_GPIO_TogglePin(GPIOx,GPIO_PIN_x);   //端口翻转
```
- 复位后一般默认输入模式

# UART
- 过采样模式，16倍过采样可获得高容错性
- 使能校验位时，MSB位是接收到的校验位
- 中断回调函数函数
    ```C
        void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) //直接定义该函数就是中断的
        {
            if(huart->Instance == USART1)   //判断是否你要处理的uart接口
            {
                /* TODO */
                
                while(HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY);  //检测是否空闲
                while(HAL_UART_Receive_IT(&huart1,bufferUART1Rx,UART1RXBUFFERSIZE)!=HAL_OK);    //重新开启接收中断
            }
        }
    ```
- 数据收发
    ```C
        HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)；  //数组数据发送
        HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size); //接收开启
    ```

# 中断
```C
HAL_NVIC_EnableIRQ(USART1_IRQn);    //使能中断
HAL_NVIC_SetPriority(USART1_IRQn,3,3);  //设置中断优先级
```
- 外部中断
    ```C
    void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin); //在该回调函数中定义各中断处理
    ```

# 看门狗
#### 独立看门狗：IWDG
- 时钟源：LSI
    ```C
    HAL_IWDG_Refresh(&hiwdg1);  //喂狗
    ```
- 超时公式
    ```math
        T = \frac{4 \cdot 2^{PRE} \cdot RLR}{CLK}
    ```
    PRE为分频系数，RLR为重装载值

#### 窗口看门狗：WWDG
- 时钟源：PCLK1
- 可采用中断触发方式喂狗
    ```C
    HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef*);  //提前唤醒中断
    HAL_WWDG_Refresh(WWDG_HandleTypeDef*);  //喂狗
    ```
- 超时公式
    ```math
        T = \frac{4096 \cdot 2^{PRE} \cdot (T[5:0]+1)}{PCLK}
    ```
# 定时器
- 高级定时器：1、8
- 通用定时器：2 ~ 5，12 ~ 17
    - TIM2、5是32位，其他为16位
    - TIM12 ~ 14不支持DMA
    - TIM12 ~ 17只能向上计数，不支持编码器
- LP开头的为低功耗计时器
- 基本定时器：6、7
- 对齐模式
    - 边沿：递增or递减
    - 中心1：交替递增递减；仅递减时，输出通道比较中断才能置1
    - 中心2：交替递增递减；仅递增时，输出通道比较中断才能置1
    - 中心3：交替递增递减；都能输出通道比较中断置1
```C
__HAL_TIM_SET_COUNTER(&htim5,0);    //设置计数器值
__HAL_TIM_DISABLE(&htim5);  //停止定时器
__HAL_TIM_ENABLE(&htim5);   //启动定时器
```

## 定时器中断
```C
HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim); //开启中断
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)；//定义改函数就可以处理更新中断
```

## PWM
```C
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel); //开启PWM输出
__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,pwmValue);    //设置PWM比较器值
__HAL_TIM_GetCompare(&htim3,TIM_CHANNEL_4);     //获取当前PWM比较器值
```
- 输出模式
    - PWM1
    - PWM2
    - C1
    - C2
    - A1
    - A2

## 输入捕获
```C
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);   //输入捕获中断回调处理
uint32_t HAL_TIM_ReadCapturedValue(TIM_HandleTypeDef *htim, uint32_t Channel)； //获取输入捕获值
TIM_RESET_CAPTUREPOLARITY(&htim5,TIM_CHANNEL_1);    //清楚原捕获设置
TIM_SET_CAPTUREPOLARITY(&htim5,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);    //设置捕获
```

# 内存保护
- Memory Protection Unit
```C
void MemManage_Handler(void);   //内存操作错误中断服务函数
NVIC_SystemReset(); //软复位
```
## 保护模式
- 背景区域：开启MPU后，特权程序可以访问无region地址，用户程序被卡
- 错误静止MPU：若有NMI和硬件fault中断，则关闭MPU
    - NMI：不可屏蔽中断
    - 硬件fault：硬件错误

## Rigion
- 最小容量：32字节
- 缓存：预先读取
- 缓冲：写入一定数量后一次性写（提高性能）
- 子Rigion属性和主一样
- Rigion必须8等分为子Rigion

# FMC
- 突发模式：一个地址，存储器自动增减地址

## FLASH
- NOR：可随机读写
- NAND：区块读写
    - 映射地址：0x8000 0000 -> 0x8FFF FFFF
- eMMC：NAND+主控

## SDRAM
- 预充电操作：SDRAM进行读写后如果要对同一Bank的另一行进行寻址，就要关闭现有行
- 刷新操作：原理上SDRAM需要不断刷新充电，刷新占用9个时钟周期（PC133标准）
- 注意：读命令下达后需要等待CL延迟才能从数据线上读数据；写数据没有这个参数
- 分为两块
    - 区域1：0xC000 0000 -> 0xCFFF FFFF
    - 区域2：0xD000 0000 -> 0xDFFF FFFF

### 控制信号
- CLK：时钟
- CKE：时钟使能，静止时自动刷新
- CS#：低电平片选
- RAS#：低电平选通行地址
- CAS#：低电平选通列地址
- WE#：低电平试能写
- BS-x:BANK地址
- DQM：数据掩码，表示数据线的有效部分

### 初始化过程
1. 上电，使能时钟，发送NOP命令；
2. 等待最少200us
3. 发送所有Bank预充电命令
4. 发送至少8次自动刷新命令（注意刷新时间间隔）
5. 设置SDRAM内部的模式寄存器

### 异步模式
- A
- B
- C
- D

# DMA
**注意**：非MDMA使用时，注意总线域访问问题
## 用于图像的DMA2D
- 用特定颜色填充图像的一部分
- 源图像一部分复制到目标图像的一部分
- 通过像素格式转换将源图像的一部分复制到目标的一部分
- 将像素格式不同的两个源部分/全部混合

## MDMA
- AXI总线和AHBS总线

## 双口DMA
- 8个数据流/DMA，115通道/数据流
- 四级32位FIFO/数据流
- 4个请求优先级
- 数据数目可以软件控制，也可以外设硬件结束
- 数据宽度不等时，自动优化带宽（仅仅FIFO）
- 可突发传输（4、8、16节拍）
- 5个事件：半传输、传输完成、传输错误、FIFO错误、直接传输错误

## BDMA

# RTC
- 闹钟A、B和中断、唤醒
- 读写掉电备份寄存器
    ```
    uint32_t HAL_RTCEx_BKUPRead(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister);
    HAL_RTCEx_BKUPWrite(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister, uint32_t Data)；
    ```
- 读写RTC时间
    ```
    HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);   //设置时间
    HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)；  //读取时间
    HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)；  //设置日期
    HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)；  //读取日期
    ```
- ***注意BUG***：读取时间后必须读取日期

# RNG
```
HAL_StatusTypeDef HAL_RNG_GenerateRandomNumber(RNG_HandleTypeDef *hrng, uint32_t *random32bit);  //获取随机数
```

# ADC
- 注意：时钟不能超过36M
- 注入通道：中断一样的转换请求，可以打断规则通道
- 芯片温度通道在ADC3
- 非触发模式下，软件直接开始转换
    ```
    HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef *hadc);
    ```
- 校准ADC
    ```
    HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef *hadc, uint32_t CalibrationMode, uint32_t SingleDiff);
    ```
- 获取目前规则通道转换值
    ```
    uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef *hadc);
    ```

# DAC
- 供电：VDDA、VSSA
- 参考电压：Vref+
- 硬件触发：3个APB时钟后传输数据；否则一个APB时钟
- DA建立时间最大2us
- 输出
    ```
    HAL_StatusTypeDef HAL_DAC_Start(DAC_HandleTypeDef* hdac, uint32_t Channel);
    HAL_StatusTypeDef HAL_DAC_SetValue(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Alignment, uint32_t Data);
    ```

# IIC
- SCL为高时，SDA不变；若SDA拉高，为结束信号，若SDA拉低为起始信号
- 接受器件拉低SDA为应答
- 7位地址+1位操作（W/R）
**注意**：使用CubeMX生产的代码，IIC初始化函数中要修正时钟初始化步骤
```
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);   //写内容到I2C存储器
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)；  //从I2C存储器读内容
```

# QSPI
- 工作模式
    - 间接模式：使用QSPI寄存器完成所有操作
    - 状态轮询：周期性读取外部FLASH状态位，若置1时会产生中断
    - 内存映射模式：当作系统内部定时器使用
- 双闪存模式：并行两个FLASH
- 可SDR和DDR
- DMA可用于间接模式
- 内部有直接拉到AXI总线的接口
- 传输阶段
    ```
    graph LR
    指令-->地址
    地址-->交替字节
    交替字节-->空指令周期
    空指令周期-->数据
    ```
- 时钟模式
    - 模式0：CS高电平（空闲）时CLK保持低电平
    - 模式3：CS高电平（空闲）时CLK保持高电平