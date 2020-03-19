#include"DRV8432.h"
#include"main.h"

//PWM速度设置
void set8432Speed_AB(float duty)
{
	if(duty>PWM_maxDuty) duty = PWM_maxDuty;  //防止占空比过大使得自举电路无法工作
	else if(-duty > PWM_maxDuty) duty = -PWM_maxDuty;
	
	__HAL_TIM_SetCompare(&DRV8432_AB_Timer,DRV8432_A_channel,PWM_cycleCount/2+(duty/200*PWM_cycleCount));
	__HAL_TIM_SetCompare(&DRV8432_AB_Timer,DRV8432_B_channel,PWM_cycleCount/2-(duty/200*PWM_cycleCount));
	return;
	
}

void set8432Speed_CD(float duty)
{
	if(duty>PWM_maxDuty) duty = PWM_maxDuty;  //防止占空比过大使得自举电路无法工作
	else if(-duty > PWM_maxDuty) duty = -PWM_maxDuty;
	
	__HAL_TIM_SetCompare(&DRV8432_CD_Timer,DRV8432_C_channel,PWM_cycleCount/2+(duty/200*PWM_cycleCount));
	__HAL_TIM_SetCompare(&DRV8432_CD_Timer,DRV8432_D_channel,PWM_cycleCount/2-(duty/200*PWM_cycleCount));
	return;
}

//刹车
void lock()
{
	__HAL_TIM_SetCompare(&DRV8432_AB_Timer,DRV8432_A_channel,PWM_cycleCount/2);
	__HAL_TIM_SetCompare(&DRV8432_AB_Timer,DRV8432_B_channel,PWM_cycleCount/2);
	__HAL_TIM_SetCompare(&DRV8432_CD_Timer,DRV8432_C_channel,PWM_cycleCount/2);
	__HAL_TIM_SetCompare(&DRV8432_CD_Timer,DRV8432_D_channel,PWM_cycleCount/2);
}

//滑行
void slide()
{
	HAL_GPIO_WritePin(DRV8432_RST_AB_Port, DRV8432_RST_AB_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRV8432_RST_CD_Port, DRV8432_RST_CD_PIN, GPIO_PIN_RESET);
}

//初始化
void init8432()
{
	//关闭PWM
	HAL_TIM_PWM_Stop(&DRV8432_AB_Timer,DRV8432_A_channel);
	HAL_TIM_PWM_Stop(&DRV8432_AB_Timer,DRV8432_B_channel);
	HAL_TIM_PWM_Stop(&DRV8432_CD_Timer,DRV8432_C_channel);
	HAL_TIM_PWM_Stop(&DRV8432_CD_Timer,DRV8432_D_channel);
	
	//关闭芯片
	HAL_GPIO_WritePin(DRV8432_RST_AB_Port, DRV8432_RST_AB_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRV8432_RST_CD_Port, DRV8432_RST_CD_PIN, GPIO_PIN_RESET);
	
	HAL_Delay(1); //防止没有成功关闭！
	lock();
	
	//开启PWM
	HAL_TIM_PWM_Start(&DRV8432_AB_Timer,DRV8432_A_channel);
	HAL_TIM_PWM_Start(&DRV8432_AB_Timer,DRV8432_B_channel);
	HAL_TIM_PWM_Start(&DRV8432_CD_Timer,DRV8432_C_channel);
	HAL_TIM_PWM_Start(&DRV8432_CD_Timer,DRV8432_D_channel);
	
	//开启芯片
	HAL_GPIO_WritePin(DRV8432_RST_AB_Port, DRV8432_RST_AB_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DRV8432_RST_CD_Port, DRV8432_RST_CD_PIN, GPIO_PIN_SET);
	
}
