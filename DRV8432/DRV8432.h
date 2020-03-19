#ifndef DRV8432_H
#define DRV8432_H

#include "main.h"

/*-----NOTICE--------
1. Read in utf8 encoding mode！
2. 本驱动程序采用差分驱动，注意8432的最高驱动频率为500KHz，以及高电平保持时间
3. 使用本驱动库前需要移植以下几个参数
--------------------*/

//采用三角计数模式，PWM模式一输出
#define timer_countFrequency 240000000  //240M
#define PWM_cycleCount 1199		//1199个计数周期
#define PWM_maxDuty 99  //最大占空比周期

//PWM通道定义
#define DRV8432_AB_Timer htim5
#define DRV8432_CD_Timer htim5
extern TIM_HandleTypeDef htim5;
#define DRV8432_A_channel TIM_CHANNEL_1
#define DRV8432_B_channel TIM_CHANNEL_2
#define DRV8432_C_channel TIM_CHANNEL_3
#define DRV8432_D_channel TIM_CHANNEL_4

//复位GPIO定义
#define DRV8432_RST_AB_Port RST_AB_GPIO_Port
#define DRV8432_RST_CD_Port RST_CD_GPIO_Port

#define DRV8432_RST_AB_PIN RST_AB_Pin
#define DRV8432_RST_CD_PIN RST_CD_Pin


//PWM速度设置（占空比在-99.99 ~ +99.99）
void set8432Speed_AB(float duty);
void set8432Speed_CD(float duty);

//刹车
void lock(void);

//滑行
void slide(void);

//初始化
void init8432(void);


#endif
