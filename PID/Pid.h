#ifndef __PID_H__
#define __PID_H__

#include "main.h"

typedef struct{
	int T;   //PID采样周期
	
	float cur_value;  //当前值
	float set_value;  //目标值
	
	float Err_N;  //本次误差
	float Err_N1;  //上次误差
	float Err_N2;  //上上次误差
	
	float Kp;  //比例系数
	float Ki;  //积分系数
	float Kd;  //微分系数

	float OUT_MAX;  //输出限幅（最大）
	float OUT_MIN;  //输出限幅（最小）
	
	float Delta_out;  //输出差值
	float Out;  //输出
} PID_struct;

PID_struct * PID_Parament_Init(PID_struct *pid,int T,
	float Kp,float Ki,float Kd,float max_out, float min_out);
float PID_Calc(PID_struct *pid,float goal,float curr);

#endif
