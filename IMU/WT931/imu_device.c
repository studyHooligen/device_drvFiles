/**
  * @file       remote_control.c/h
  * @brief      串口数据-判断遥控器数据是否出错-装入imu数据结构体              
  * @note       本文件的入口函数为get_imu_data,
                因此工程通过使用函数get_imu_data来调用本文件得到imu数据imu_info_t。
  * @history
  *  Version    Date            Author         
  *  V1.0.0     2020/3/29       zhousen                   
  */
#include "imu_device.h"


/**
  * @brief          判断imu数据是否出错 
  * @param[in]      buff:数据指针  
  * @retval         1:ture 0:false
  */
static uint8_t imu_buff_is_error(uint8_t *buff);

/**
  * @brief          协议解析 
  * @param          imu_info_t:解析后imu数据指针
  * @param          buff:解析前原始数据指针  
  * @retval         none
  */
void get_imu_data(imu_info *imu_info_t, uint8_t *buff)
{
	if(buff[0]!=0x55)
	{
		return;
	}
	else
	{
		switch(buff[1])
		{
			case 0x50:	
			{
				if(imu_buff_is_error(buff))
				memcpy(&imu_info_t->stcTime,&buff[2],8);
			}break;
			case 0x51:	
			{
				if(imu_buff_is_error(buff))
				memcpy(&imu_info_t->stcAcc,&buff[2],8);
			}break;
			case 0x52:	
			{
				if(imu_buff_is_error(buff))
				memcpy(&imu_info_t->stcGyro,&buff[2],8);
			}break;
			case 0x53:	
			{
				if(imu_buff_is_error(buff))
				memcpy(&imu_info_t->stcAngle,&buff[2],8);
			}break;
		}        		
	}	
}
/**
  * @brief          判断imu数据是否出错 
  * @param[in]      buff:数据指针  
  * @retval         1:ture 0:false
  */
static uint8_t imu_buff_is_error(uint8_t *buff)
{
	int16_t sum=0;
	uint8_t i;
	for(i=0;i<=9;i++)
	{
		sum+=buff[i];
	}
	if(buff[10]==sum)
	{
		return 1;
	}
	else return 0;	
}






