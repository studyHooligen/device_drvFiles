#ifndef IMU_DEVICE_H
#define IMU_DEVICE_H

#include "struct_typedef.h"
#include "string.h"

#define IMU_RX_BUF_NUM 22u   
#define IMU_FRAME_LENGTH 11u 

typedef struct   //0X55 0X50
{
	unsigned char ucYear;
	unsigned char ucMonth;
	unsigned char ucDay;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned short usMiliSecond;
}STime;  
typedef struct       //0X55 0X51
{
	short a[3];      //-32768~0~32768 -16g~0~16g g=9.8m/s2
	short T;
}SAcc;
typedef struct       //0X55 0X52
{
	short w[3];      //-32768~0~32768 -2000~0~2000('/s)
	short T;
}SGyro;
typedef struct       //0X55 0X53
{
	short Angle[3];  //-32768~0~32768 -180~0~180
	short T;
}SAngle;

typedef struct
{
STime	stcTime;
SAcc 	stcAcc;
SGyro   stcGyro;
SAngle 	stcAngle;	
}imu_info;


void get_imu_data(imu_info *imu_info_t, uint8_t *buff);

#endif

