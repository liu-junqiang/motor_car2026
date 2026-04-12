/*
 * control.h
 *
 *  Created on: 2025年8月16日
 *      Author: Pathetic.
 */

#ifndef CODE_CONTROL_CONTROL_H_
#define CODE_CONTROL_CONTROL_H_

#include "zf_common_headfile.h"

typedef struct
{
    // 电机
    int16 motor_speed;
    int16 encoder_count;
    int16 encoder_speed;
    int16 speed_error_0;
    int16 speed_error_1;
    int16 speed_error_2;
    int16 motor_proportion;
    int16 motor_integral;
    int16 motor_derivation;
    float lucheng;
    // 舵机
    float steering_angle_error_0;
    float steering_angle_error_1;
    float steering_angle_abs_error_1;
    float steering_derivation;
    float steering_proportion;
    float steering_dynamic_proportion;
}control_temp_struct;

typedef struct
{
    int16 close_speed_straight;
    int16 close_speed_cross;
    int16 close_speed_island;


    int16 open_speed_straight;
    int16 open_speed_cross;
    int16 open_speed_island;


}control_speed_struct;



typedef struct
{
    float steering_kp_straight;
    float steering_kp_cross;
    float steering_kp_island;

    float steering_kd_straight;
    float steering_kd_cross;
    float steering_kd_island;

    float steering_kk_straight;
    float steering_kk_cross;
    float steering_kk_island;
}control_steering_struct;

typedef struct
{
    // 电机
    int16 setspeed;
    float motor_kp;
    float motor_ki;
    float motor_kd;
    float motor_pwm_out_lim;
    float low_pass_filter_k;
    float motor_integral_lim;

    // 舵机
    float steering_kp;
    float steering_kd;
    float steering_kk;
    float steering_dynamic_proportion_max;
    float steering_dynamic_proportion_min;
    float steering_low_pass_filter_k;


}control_parameters_struct;

typedef struct
{
    // 左电机
    int16 motor_pwm_delta;
    int16 motor_pwm_out;

    // 舵机
    int16 steering_pwm_delta;
    int16 steering_pwm_out;
}control_pwm_out_struct;

typedef struct
{
    bool Offset_flag;
    bool Close_speed_enable;
	bool keyboard_scan_enable;
	bool keyboard_timer_flag;
	bool keyboard_long_press_flag;
	uint8 yaw_flag;
	bool start_flag;
	bool breaking_flag;
}global_flag_struct;

typedef struct
{
    uint8 send_data_buffer[7];                  // 发送缓冲数组
    uint8 receive_data_buffer[7];               // 接收缓冲数组
    uint8 receive_data_count;                   // 接收计数
    uint8 sum_check_data;                       // 校验位
    int16 receive_left_speed_data;              // 接收到的左侧电机速度数据
    int16 receive_right_speed_data;             // 接收到的右侧电机速度数据
}small_device_value_struct;

extern control_temp_struct* ctrl_temp;
extern control_speed_struct* ctrl_speed;
extern control_steering_struct* ctrl_steering;
extern control_parameters_struct* ctrl_parameters;
extern control_pwm_out_struct* ctrl_pwm_out;
extern global_flag_struct* glb_flag;
extern float Pitch_u;
extern float Steering_u;
extern small_device_value_struct motor_value;
extern float Pitch_Loop_Kp;
extern float Pitch_Loop_Kd;
extern float Gyro_x_Loop_Kp;
extern float Gyro_x_Loop_Kd;
extern float Steering_Loop_kp;
extern float Steering_Loop_kd;

extern void speed_get(void);
extern void lucheng_get(void);
extern void low_pass_filter_calc(void);
extern void motor_pid_calc(void);
extern void steering_pdk_calc(void);
extern void dynamic_ctrl(void);
extern void pwm_out(void);
extern void Gyro_x_PID_Controller(float Pitch_Loop_Out);
extern void Pitch_PID_Controller(float Error_Loop_Out);
extern void uart_control_callback(void);                                   // 无刷驱动 串口接收回调函数
extern void small_driver_set_duty(int16 left_duty, int16 right_duty);      // 无刷驱动 设置电机占空比
extern void small_driver_get_speed(void);                                  // 无刷驱动 获取速度信息
extern void small_driver_uart_init(void);                                  // 无刷驱动 串口通讯初始化
extern  void pitch_pid_cnt(void);

enum sub1{
    road1,
    road_circle,
    road2
};
extern enum sub1;


#endif /* CODE_CONTROL_CONTROL_H_ */
