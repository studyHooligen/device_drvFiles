#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "struct_typedef.h"
#include "string.h"

#define ENCODER_ANGLE_RATIO (8192.0f / 360.0f)

typedef struct
{
  uint16_t ecd;       //0~8191/0~360
  int16_t speed_rpm;  //RPM
//M3508:-16384~0~16384/-20A~0~20A M2006: -10000~0~10000/-10A~0~10A M6020:-30000~0~30000
  int16_t motor_current;  
  uint8_t temperate;  //'C

  uint16_t last_ecd;   
  int32_t round_cnt;
  int32_t total_ecd;
	float  angle;
}motor_data;
typedef enum device_can
{
  MOTOR_CAN1 = 0,
  MOTOR_CAN2,
}motor_can;
typedef struct
{
  motor_data motor_data_t;
  motor_can motor_can_t;
  
// M3508/M2006:0X201~0X208 M6020:0X205~0X20B
  uint16_t can_receive_id;
// M3508/M2006:0X200(1/2/3/4),0X1FF(5/6/7/8)  M6020:0X1FF(1/2/3/4),0X2FF(5/6/7)
  uint16_t can_send_id;  
  int16_t current;             
}motor_device;


void motor_register(motor_device *motor_device_t, 
         motor_can motor_can_t, uint16_t can_receive_id, uint16_t can_send_id); 
void get_motor_measure(motor_device *motor_device_t, motor_can rx_can, uint16_t rx_can_id, uint8_t rx_data[]);
void set_motor_current(motor_device *motor_device_t, int16_t current);


#endif



