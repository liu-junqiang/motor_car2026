/*
 * universal.c
 *
 *  Created on: 2025年8月17日
 *      Author: Pathetic.
 */

#include "universal.h"

uint32 parameter_arr[PARAMETER_ARR_LEN] = { 0 };
//------------------------------------------------------------------------------
// 函数简介     逐飞助手虚拟示波器
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void seekfree_assistant_oscilloscope_show_data(void)
{
    seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);
    //seekfree_assistant_oscilloscope_data.data[0] = (float)Pitch;
    seekfree_assistant_oscilloscope_data.data[0] = (float)zhou;//显示原始值，方便判断滤波效果2
    seekfree_assistant_oscilloscope_data.data[1] = (float)Gyro_x;//滤波后输出值
    seekfree_assistant_oscilloscope_data.data[2] = (float)Pitch_u;//外环输出值
   // seekfree_assistant_oscilloscope_data.data[3] = (float)Q_angle;
    seekfree_assistant_oscilloscope_data.data[3] = (float)ctrl_pwm_out->steering_pwm_out;
    seekfree_assistant_oscilloscope_data.data[4] = (float)Q_gyro;
    seekfree_assistant_oscilloscope_data.data[5] = (float)R_angle;
    seekfree_assistant_oscilloscope_data.data[6] = (float)ctrl_pwm_out->motor_pwm_out;
    seekfree_assistant_oscilloscope_data.data[7] = (float)Steering_u;//舵机输出值

}

//------------------------------------------------------------------------------
// 函数简介     逐飞助手调参
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void seekfree_assistant_parameter_control(void)
{

    for(uint8 i = 0; i < SEEKFREE_ASSISTANT_PARAMETER_CH; i++)
    {
        if(seekfree_assistant_parameter_update_flag[i] == 1)
        {

            seekfree_assistant_parameter_update_flag[i] = 0;
            seekfree_assistant_parameter_update_flag[i] = 0;
//            Q_angle = seekfree_assistant_parameter[0];
//            Q_gyro = seekfree_assistant_parameter[1];
//            R_angle = seekfree_assistant_parameter[2];
            ctrl_parameters->motor_kp = seekfree_assistant_parameter[0];
            Steering_Loop_kp = seekfree_assistant_parameter[1];
            Steering_Loop_kd = seekfree_assistant_parameter[2];
            Pitch_Loop_Kp = seekfree_assistant_parameter[3];
            Pitch_Loop_Kd = seekfree_assistant_parameter[4];
            Gyro_x_Loop_Kp = seekfree_assistant_parameter[5];
            Gyro_x_Loop_Kd = seekfree_assistant_parameter[6];
            ctrl_speed->open_speed_straight = seekfree_assistant_parameter[7]; //开环
           // ctrl_parameters->setspeed = seekfree_assistant_parameter[7];
        }
    }
    seekfree_assistant_data_analysis();
}

//------------------------------------------------------------------------------
// 函数简介     逐飞助手上位机
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void seekfree_assistant_control(void)
{
    seekfree_assistant_oscilloscope_show_data();
    seekfree_assistant_parameter_control();
}


