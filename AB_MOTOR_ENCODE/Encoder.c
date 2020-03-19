#include "Encoder.h"

//uint32_t capture_buf[3]={0};
//uint8_t capture_cnt=0;

//uint8_t IT_Status=0;  //中断状态
//uint8_t IT_Count=0;   //中断计数

//uint32_t pwm_cycle;
//double pwm_frequency=0;

//double Rotate_speed=0;
//int8_t Rotate_direction=0;

uint32_t AA1_delta,AB1_delta;  //用于判断方向的计数器
uint32_t AA2_delta,AB2_delta;
int direct1 = 1,direct2 = 1;  //方向存储

//开始工作
void Encoder_begin(void)
{
	__HAL_TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_3,TIM_INPUTCHANNELPOLARITY_FALLING);
	HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_3);
	
	__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_3,TIM_INPUTCHANNELPOLARITY_FALLING);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_3);
}

//输入捕获中断
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(TIM2 == htim->Instance) {  //如果是2电机
		if(HAL_TIM_ACTIVE_CHANNEL_3==htim->Channel) {
			__HAL_TIM_SET_COUNTER(htim,0);  //计数器清零
			AA2_delta = __HAL_TIM_GET_COMPARE(htim,A_chennel);  //获取A捕获时间			
		
			if(AA2_delta<reversalThreshold) HAL_TIM_IC_Start_IT(htim,B_chennel);
			else HAL_TIM_IC_Stop_IT(htim,B_chennel);
			return;
		}
		if(HAL_TIM_ACTIVE_CHANNEL_4==htim->Channel) {
			AB2_delta = __HAL_TIM_GET_COMPARE(htim,B_chennel);  //获取B捕获时间			
			return;
		}
	} else {  //如果是1电机
		if(TIM1 == htim->Instance) {
			if(HAL_TIM_ACTIVE_CHANNEL_3==htim->Channel) {
			__HAL_TIM_SET_COUNTER(htim,0);  //计数器清零
			AA1_delta = __HAL_TIM_GET_COMPARE(htim,A_chennel);  //获取A捕获时间			
		
			if(AA1_delta<reversalThreshold) HAL_TIM_IC_Start_IT(htim,B_chennel);
			else HAL_TIM_IC_Stop_IT(htim,B_chennel);
			return;
			}
		if(HAL_TIM_ACTIVE_CHANNEL_4==htim->Channel) {
			AB1_delta = __HAL_TIM_GET_COMPARE(htim,B_chennel);  //获取B捕获时间			
			return;
		}
		}
	}
}

//定时器周期溢出中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	return ;
}

//位置积分
void locationInteg(uint32_t * location1,uint32_t * locaiton2)
{
	if(AA1_delta){
		if(AA1_delta>reversalThreshold) {
			location1 += direct1;
		} else {
			direct1 = ( (AA1_delta - AB1_delta) > AA1_delta );
			location1 += direct1;
		}
		AA1_delta = 0;
	}
	if(AA2_delta){
		if(AA2_delta>reversalThreshold) {
			location2 += direct1;
		} else {
			direct2 = ( (AA2_delta - AB2_delta) > AA2_delta );
			location2 += direct2;
		}
		AA2_delta = 0;
	}
}

