/**
  * @file       remote_control.c/h
  * @brief      串口数据-装入遥控器数据结构体-判断遥控器数据是否出错-得到遥控器控制信息              
  * @note       本文件的入口函数为sbus_to_rc(rc_info_t *rc_ctrl, uint8_t *sbus_buf)。
                因此工程通过使用函数sbus_to_rc(rc_info_t *rc_ctrl, uint8_t *sbus_buf)来
				调用本文件得到遥控器数据rc_ctrl。
  * @history
  *  Version    Date            Author         
  *  V1.0.0     2020/3/26       zhousen                   
  */
#include "remote_control.h"

//remote control data
rc_info_t rc_ctrl;
//rc frame received from usart
uint8_t sbus_rx_buf[SBUS_RX_BUF_NUM];

/**
  * @brief          遥控器控制信息 
  * @param[in]      rc_ctrl:遥控器数据指针  
  * @retval         none
  */
static void get_remote_state(rc_info_t *rc_ctr1);
/**
  * @brief          取绝对值 
  * @param[in]      value: 
  * @retval         none
  */
static int16_t RC_abs(int16_t value);
/**
  * @brief          判断遥控器数据是否出错 
  * @param[in]      rc_ctrl:遥控器数据指针  
  * @retval         1:ture 0:false
  */
static uint8_t RC_data_is_error(rc_info_t *rc_ctr1);


/**
  * @brief          取绝对值 
  * @param[in]      value: 
  * @retval         none
  */
static int16_t RC_abs(int16_t value)
{
    if (value > 0)
    {
        return value;
    }
    else
    {
        return -value;
    }
}
/**
  * @brief          遥控器协议解析 
  * @param[out]     rc_ctrl:解析后遥控器数据指针
  * @param[in]      sbus_buf:解析前原始数据指针  
  * @retval         none
  */
void sbus_to_rc(rc_info_t *rc_ctrl, uint8_t *sbus_buf)
{
	if (sbus_buf == NULL || rc_ctrl == NULL)
    {
        return;
    }
	rc_ctrl->ch0 = (sbus_buf[0] | (sbus_buf[1] << 8)) & 0x07ff;        //!< Channel 0
    rc_ctrl->ch1 = ((sbus_buf[1] >> 3) | (sbus_buf[2] << 5)) & 0x07ff; //!< Channel 1
    rc_ctrl->ch2 = ((sbus_buf[2] >> 6) | (sbus_buf[3] << 2) |          //!< Channel 2
                         (sbus_buf[4] << 10)) &0x07ff;
    rc_ctrl->ch3 = ((sbus_buf[4] >> 1) | (sbus_buf[5] << 7)) & 0x07ff; //!< Channel 3
	
	rc_ctr1->last_sw1=rc_ctr1->sw1;
	rc_ctr1->last_sw2=rc_ctr1->sw2;
    rc_ctrl->sw1 = ((sbus_buf[5] >> 4) & 0x0003);                      //!< Switch left
    rc_ctrl->sw2 = ((sbus_buf[5] >> 4) & 0x000C) >> 2;                 //!< Switch right
	
    rc_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);               //!< Mouse X axis
    rc_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);               //!< Mouse Y axis
    rc_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);             //!< Mouse Z axis
    rc_ctrl->mouse.press_l = sbus_buf[12];                             //!< Mouse Left Is Press ?
    rc_ctrl->mouse.press_r = sbus_buf[13];                             //!< Mouse Right Is Press ?
	
    rc_ctrl->key.v = sbus_buf[14] | (sbus_buf[15] << 8);               //!< KeyBoard value
	
    rc_ctrl->wheel = sbus_buf[16] | (sbus_buf[17] << 8);
	
	rc_ctrl->ch0 -= RC_CH_VALUE_OFFSET;
    rc_ctrl->ch1 -= RC_CH_VALUE_OFFSET;
    rc_ctrl->ch2 -= RC_CH_VALUE_OFFSET;
    rc_ctrl->ch3 -= RC_CH_VALUE_OFFSET;
    rc_ctrl->wheel -= RC_CH_VALUE_OFFSET;
    	
	if(RC_data_is_error(rc_ctr1))
	{
		get_remote_state(rc_ctr1);
	}
	
}
/**
  * @brief          判断遥控器数据是否出错 
  * @param[in]      rc_ctrl:遥控器数据指针  
  * @retval         1:ture 0:false
  */
static uint8_t RC_data_is_error(rc_info_t *rc_ctr1)
{
	if (RC_abs(rc_ctrl.ch0) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (RC_abs(rc_ctrl.ch1) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (RC_abs(rc_ctrl.ch2) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (RC_abs(rc_ctrl.ch3) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (rc_ctrl.sw1 == 0)
    {
        goto error;
    }
    if (rc_ctrl.sw2 == 0)
    {
        goto error;
    }
    return 1;

error:
    rc_ctrl.ch0 = 0;
    rc_ctrl.ch1 = 0;
    rc_ctrl.ch2 = 0;
    rc_ctrl.ch3 = 0;
    rc_ctrl.ch4 = 0;
    rc_ctrl.sw1 = RC_SW_DOWN;
    rc_ctrl.sw2 = RC_SW_DOWN;
    rc_ctrl.mouse.x = 0;
    rc_ctrl.mouse.y = 0;
    rc_ctrl.mouse.z = 0;
    rc_ctrl.mouse.press_l = 0;
    rc_ctrl.mouse.press_r = 0;
    rc_ctrl.key.v = 0;
    return 0;
}
/**
  * @brief          遥控器控制信息 
  * @param[in]      rc_ctrl:遥控器数据指针  
  * @retval         none
  */
static void get_remote_state(rc_info_t *rc_ctr1)
{
	rc_ctr1->state=0;
  if(rc_ctr1->sw1 == 3)
  {
    rc_ctr1->state |= RC_S1_MID;
    rc_ctr1->state &= ~RC_S1_UP;
    rc_ctr1->state &= ~RC_S1_DOWN;
    if(rc_ctr1->last_sw1 == 1)
    {
      rc_ctr1->state |= RC_S1_UP2MID;
    }
    else if(rc_ctr1->last_sw1 == 2)
    {
      rc_ctr1->state |= RC_S1_DOWN2MID;
    }
  }
  else if(rc_ctr1->sw1 == 1)
  {
    rc_ctr1->state &= ~RC_S1_MID;
    rc_ctr1->state |= RC_S1_UP;
    rc_ctr1->state &= ~RC_S1_DOWN;
    if(rc_ctr1->last_sw1 == 3)
    {
      rc_ctr1->state |= RC_S1_MID2UP;
    }
  }
  else if(rc_ctr1->sw1 == 2)
  {
    rc_ctr1->state &= ~RC_S1_MID;
    rc_ctr1->state &= ~RC_S1_UP;
    rc_ctr1->state |= RC_S1_DOWN;
    if(rc_ctr1->last_sw1 == 3)
    {
      rc_ctr1->state |= RC_S1_MID2DOWN;
    }
		if(rc_ctr1->sw2 == 2)
		{
			rc_ctr1->state |= RC_S2_S1_DOWN;
		}
  }
  
  if(rc_ctr1->sw2 == 3)
  {
    rc_ctr1->state |= RC_S2_MID;
    rc_ctr1->state &= ~RC_S2_UP;
    rc_ctr1->state &= ~RC_S2_DOWN;
    if(rc_ctr1->last_sw2 == 1)
    {
      rc_ctr1->state |= RC_S2_UP2MID;
    }
    else if(rc_ctr1->last_sw2 == 2)
    {
      rc_ctr1->state |= RC_S2_DOWN2MID;
    }
  }
  else if(rc_ctr1->sw2 == 1)
  {
    rc_ctr1->state &= ~RC_S2_MID;
    rc_ctr1->state |= RC_S2_UP;
    rc_ctr1->state &= ~RC_S2_DOWN;
    if(rc_ctr1->last_sw2 == 3)
    {
      rc_ctr1->state |= RC_S2_MID2UP;
    }
  }
  else if(rc_ctr1->sw2 == 2)
  {
    rc_ctr1->state &= ~RC_S2_MID;
    rc_ctr1->state &= ~RC_S2_UP;
    rc_ctr1->state |= RC_S2_DOWN;
    if(rc_ctr1->last_sw2 == 3)
    {
      rc_ctr1->state |= RC_S2_MID2DOWN;
    }
  }
}











