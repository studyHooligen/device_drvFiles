#include "Pid.h"

PID_struct * PID_Parament_Init(PID_struct *pid,int T,
	float Kp,float Ki,float Kd,float max_out, float min_out)
{
	pid->T = T;
	
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
	
	pid->Err_N = 0;
	pid->Err_N1 = 0;
	pid->Err_N2 = 0;

	pid->OUT_MAX = max_out;
	pid->OUT_MIN = min_out;

	pid->cur_value = 0;
	pid ->set_value = 0;
	pid->Delta_out = 0;
	pid->Out = 0;
	return pid;
}

float PID_Calc(PID_struct *pid,float goal,float curr)
{
	float dk1,dk2;  

	pid->cur_value = curr;
	pid->set_value = goal;
	
	pid->Err_N = pid->set_value - pid->cur_value;
	dk1 = pid->Err_N - pid->Err_N1;
	dk2 = pid->Err_N - 2 * pid->Err_N1 + pid->Err_N2;

	pid->Delta_out = pid->Kp * dk1 + pid->Ki * pid->Err_N + pid->Kp * dk2;
	
	pid->Out += pid->Delta_out;
	
	pid->Err_N2 = pid->Err_N1;  
	pid->Err_N1 = pid->Err_N;

	if(pid->Out>pid->OUT_MAX) pid->Out = pid->OUT_MAX ;  //????
	if(pid->Out<pid->OUT_MIN) pid->Out = pid->OUT_MIN ;

	return pid->Out;
}


