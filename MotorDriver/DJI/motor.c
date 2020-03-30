/**
  * @file       motor.c/h
  * @brief      注册某电机、(can口数据+id)-装入某一指定电机结构体中、设置电机电流
  * @note       使用本文件时，应先在工程中使用函数motor_register注册要使用的电机
  * @history
  *  Version    Date            Author           
  *  V1.0.0     2020/3/28       zhousen           
  */

#include "motor.h"

/**
  * @brief          注册某电机
  * @param          motor_device_t:待注册的电机
  * @param          motor_can_t：待注册的电机的can口
  * @param          can_receive_id：待注册的电调的发送ID
  * @param          can_send_id：待注册的电调的接收ID
  * @retval         none
  */
void motor_register(motor_device *motor_device_t, 
	            motor_can motor_can_t, uint16_t can_receive_id, uint16_t can_send_id)
{
	if(motor_device_t == NULL)
	{
		return;
	}
	motor_device_t->motor_can_t=motor_can_t;
	motor_device_t->can_receive_id=can_receive_id;
	motor_device_t->can_send_id=can_send_id;
}
/**
  * @brief          协议解析
  * @param          motor_device_t:已注册的电机
  * @param          rx_can：数据接收的can口
  * @param          rx_can_id：接收数据的ID
  * @param          rx_data[]：接受的数据
  * @retval         none
  */
void get_motor_measure(motor_device *motor_device_t, 
	             motor_can rx_can, uint16_t rx_can_id, uint8_t rx_data[])
{
	if(motor_device_t == NULL)
	{
		return;
	}	
	if(motor_device_t->motor_can_t == rx_can)
	{
		if(motor_device_t->can_receive_id == rx_can_id)
		{
			motor_device_t->motor_data_t.last_ecd = motor_device_t->motor_data_t.ecd;                                   \
            motor_device_t->motor_data_t.ecd = (uint16_t)(rx_data[0] << 8 | rx_data[1]);            
            motor_device_t->motor_data_t.speed_rpm = (uint16_t)(rx_data[2] << 8 | rx_data[3]);      
            motor_device_t->motor_data_t.motor_current = (uint16_t)(rx_data[4] << 8 | rx_data[5]);  
            motor_device_t->motor_data_t.temperate = rx_data[6];
			
			motor_device_t->motor_data_t.angle=motor_device_t->motor_data_t.ecd/ENCODER_ANGLE_RATIO;
			if(motor_device_t->motor_data_t.ecd - motor_device_t->motor_data_t.last_ecd > 4096)
			motor_device_t->motor_data_t.round_cnt --;
		    else if(motor_device_t->motor_data_t.ecd - motor_device_t->motor_data_t.last_ecd < -4096)
			motor_device_t->motor_data_t.round_cnt ++;
		}		
	}	
}
/**
  * @brief          设置电机电流
  * @param          motor_device_t:已注册的电机
  * @param          current：设置控制电机的电流
  * @retval         none
  */
void set_motor_current(motor_device *motor_device_t, int16_t current)
{
	if(motor_device_t == NULL)
	{
		return;
	}	
  motor_device_t->current = current;		
}









