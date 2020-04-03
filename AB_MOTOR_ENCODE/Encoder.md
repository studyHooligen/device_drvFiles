# ENCODER

- 信号线
  - AP
  - BP
  - ZP：每转一圈跳变一次

- STM32自带定时器编码器
    ```C
    HAL_StatusTypeDef HAL_TIM_Encoder_Start(TIM_HandleTypeDef *htim, uint32_t Channel); //开启编码器

    __HAL_TIM_GET_COUNTER(TIM_HandleTypeDef*);   //获取编码器位置计数
    ```
    - 使用Z相信号中断处理中复位位置信息
      ```C
      __HAL_TIM_SET_COUNTER(&htim1, 0);
      ```