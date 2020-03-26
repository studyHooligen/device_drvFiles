#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include "struct_typedef.h"

#define SBUS_RX_BUF_NUM 36u   
#define RC_FRAME_LENGTH 18u   
/* -----------------------rc channel definition---------------------------------- */
#define RC_CH_VALUE_MIN         ((uint16_t)364)
#define RC_CH_VALUE_OFFSET      ((uint16_t)1024)
#define RC_CH_VALUE_MAX         ((uint16_t)1684)
/* -----------------------remote control information----------------------------- */
#define RC_S1_UP2MID   ((uint16_t)1 << 0u)
#define RC_S1_MID2UP   ((uint16_t)1 << 1u)
#define RC_S1_DOWN2MID ((uint16_t)1 << 2u)
#define RC_S1_MID2DOWN ((uint16_t)1 << 3u)

#define RC_S2_UP2MID   ((uint16_t)1 << 4u)
#define RC_S2_MID2UP   ((uint16_t)1 << 5u)
#define RC_S2_DOWN2MID ((uint16_t)1 << 6u)
#define RC_S2_MID2DOWN ((uint16_t)1 << 7u)

#define RC_S1_UP       ((uint16_t)1 << 8u)
#define RC_S1_MID      ((uint16_t)1 << 9u)
#define RC_S1_DOWN     ((uint16_t)1 << 10u)
#define RC_S2_UP       ((uint16_t)1 << 11u)
#define RC_S2_MID      ((uint16_t)1 << 12u)
#define RC_S2_DOWN     ((uint16_t)1 << 13u)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W            ((uint16_t)1 << 0)
#define KEY_PRESSED_OFFSET_S            ((uint16_t)1 << 1)
#define KEY_PRESSED_OFFSET_A            ((uint16_t)1 << 2)
#define KEY_PRESSED_OFFSET_D            ((uint16_t)1 << 3)
#define KEY_PRESSED_OFFSET_SHIFT        ((uint16_t)1 << 4)
#define KEY_PRESSED_OFFSET_CTRL         ((uint16_t)1 << 5)
#define KEY_PRESSED_OFFSET_Q            ((uint16_t)1 << 6)
#define KEY_PRESSED_OFFSET_E            ((uint16_t)1 << 7)
#define KEY_PRESSED_OFFSET_R            ((uint16_t)1 << 8)
#define KEY_PRESSED_OFFSET_F            ((uint16_t)1 << 9)
#define KEY_PRESSED_OFFSET_G            ((uint16_t)1 << 10)
#define KEY_PRESSED_OFFSET_Z            ((uint16_t)1 << 11)
#define KEY_PRESSED_OFFSET_X            ((uint16_t)1 << 12)
#define KEY_PRESSED_OFFSET_C            ((uint16_t)1 << 13)
#define KEY_PRESSED_OFFSET_V            ((uint16_t)1 << 14)
#define KEY_PRESSED_OFFSET_B            ((uint16_t)1 << 15)
/* ----------------------- Data Struct ------------------------------------- */
typedef __packed struct
{
  /* rocker channel information */
  int16_t ch0;
  int16_t ch1;
  int16_t ch2;
  int16_t ch3;
  
  int16_t wheel;
  /* left and right lever information */
  uint8_t sw1; 
  uint8_t sw2;
	
  uint8_t last_sw1;
  uint8_t last_sw2;
  /*remote control information*/
  uint16_t state;
  /*pc mouse information*/
	__packed struct 
  {
    int16_t x;
    int16_t y;
    int16_t z;

    uint8_t press_l;
    uint8_t press_r;
  } mouse;
  /* keyboard key information */
   uint16_t key_code;
} rc_info_t;

extern rc_info_t rc_ctrl;
extern uint8_t sbus_rx_buf[SBUS_RX_BUF_NUM];

/**
  * @brief          遥控器协议解析 
  * @param[out]     rc_ctrl:解析后遥控器数据指针
  * @param[in]      sbus_buf:解析前原始数据指针  
  * @retval         none
  */
void sbus_to_rc(rc_info_t *rc_ctrl, uint8_t *sbus_buf);

#endif
































