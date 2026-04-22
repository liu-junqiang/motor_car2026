/*
 * init.c
 *
 *  Created on: 2025年8月16日
 *      Author: Pathetic.
 */

#include "init.h"

//------------------------------------------------------------------------------
// 函数简介     显示屏初始化
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0    ips200
//------------------------------------------------------------------------------
void screen_init(void)
{
    // 显示方向
    ips200_set_dir(IPS200_PORTAIT);
    // 显示颜色
    ips200_set_color(RGB565_BLACK, RGB565_39C5BB);
    // 显示字号
    ips200_set_font(IPS200_8X16_FONT);
    ips200_init(IPS200_TYPE_SPI);
    ips200_clear();
}
//------------------------------------------------------------------------------
// 函数简介     舵机初始化
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void servo_init(void)
{
    // 舵机
    ips200_show_string(0, 16*1, "steering");
    pwm_init(steering_pwm, steering_freq, steering_middle);
    ips200_show_string(16*11, 16*1, "success");
}
//------------------------------------------------------------------------------
// 函数简介     姿态传感器初始化
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0    imu660ra
//------------------------------------------------------------------------------
void imu_init(void)
{
    ips200_show_string(0, 16*5, "imu");
    while(imu660ra_init());
    ips200_show_string(16*11, 16*5, "success");
}

//------------------------------------------------------------------------------
// 函数简介     无线初始化
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0    无线串口
//------------------------------------------------------------------------------
void wireless_init(void)
{
    ips200_show_string(0, 16*6, "wireless");
    while(wireless_uart_init());
    ips200_show_string(16*11, 16*6, "success");
}

//------------------------------------------------------------------------------
// 函数简介     端口初始化
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void io_init(void)
{
    ips200_show_string(0, 16*7, "io");
    // 陀螺仪零漂指示灯
    gpio_init(LED, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    // 矩阵键盘
    gpio_init(first_line,       GPI, GPIO_LOW, GPI_PULL_DOWN);
    gpio_init(second_line,      GPI, GPIO_LOW, GPI_PULL_DOWN);
    gpio_init(third_line,       GPI, GPIO_LOW, GPI_PULL_DOWN);
    gpio_init(fourth_line,      GPI, GPIO_LOW, GPI_PULL_DOWN);
    gpio_init(first_premolar,   GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(second_premolar,  GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(third_premolar,   GPO, GPIO_LOW, GPO_PUSH_PULL);
    // 蜂鸣器
    gpio_init(buzzer, GPO, GPIO_LOW, GPO_PUSH_PULL);
    ips200_show_string(16*11, 16*7, "success");
}

//------------------------------------------------------------------------------
// 函数简介     逐飞助手初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void seekfree_assistant_init(void)
{
    ips200_show_string(0, 16*8, "seekfree_assistant");
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIRELESS_UART);
    seekfree_assistant_oscilloscope_data.channel_num = 8;
    ips200_show_string(16*11, 16*8, "success");
}



//------------------------------------------------------------------------------
// 函数简介     控制中间变量结构体初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void control_temp_struct_init(void)
{
    // 左电机
    ctrl_temp->motor_speed = 0;
    ctrl_temp->encoder_count = 0;
    ctrl_temp->encoder_speed = 0;
    ctrl_temp->speed_error_0 = 0;
    ctrl_temp->speed_error_1 = 0;
    ctrl_temp->speed_error_2 = 0;
    ctrl_temp->motor_proportion = 0;
    ctrl_temp->motor_integral = 0;
    ctrl_temp->motor_derivation = 0;
    ctrl_temp->lucheng = 0;
    // 舵机
    ctrl_temp->steering_angle_error_0 = 0;
    ctrl_temp->steering_angle_error_1 = 0;
    ctrl_temp->steering_angle_abs_error_1 = 0;
    ctrl_temp->steering_derivation = 0;
    ctrl_temp->steering_proportion = 0;
    ctrl_temp->steering_dynamic_proportion = 0;
}

//------------------------------------------------------------------------------
// 函数简介     控制速度结构体初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void control_speed_struct_init(void)
{
    ctrl_speed->close_speed_straight = 0;
    ctrl_speed->close_speed_cross = 0;
    ctrl_speed->close_speed_island = 0;

    ctrl_speed->open_speed_straight = 2000;
    //ctrl_speed->open_speed_straight = 1000;
    //ctrl_speed->open_speed_straight = 0;
    ctrl_speed->open_speed_cross = 0;
    ctrl_speed->open_speed_island = 0;


}



//------------------------------------------------------------------------------
// 函数简介     控制舵机结构体初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void control_steering_struct_init(void)
{
    ctrl_steering->steering_kp_straight = 0;
    ctrl_steering->steering_kp_cross = 0;
    ctrl_steering->steering_kp_island = 0;

    ctrl_steering->steering_kd_straight = 0;
    ctrl_steering->steering_kd_cross = 0;
    ctrl_steering->steering_kd_island = 0;

    ctrl_steering->steering_kk_straight = 0;
    ctrl_steering->steering_kk_cross = 0;
    ctrl_steering->steering_kk_island = 0;
}

//------------------------------------------------------------------------------
// 函数简介     控制参数结构体初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void control_parameters_struct_init(void)
{
    // 左电机
    ctrl_parameters->setspeed = 0;
    ctrl_parameters->motor_kp = 180.0f;
    ctrl_parameters->motor_ki = 0.82f;
    ctrl_parameters->motor_kd = 5.0f;
    ctrl_parameters->motor_pwm_out_lim = 0.6f;
    ctrl_parameters->low_pass_filter_k = 0.15f;
    ctrl_parameters->motor_integral_lim = 2000;
    // 舵机
    ctrl_parameters->steering_kp = 4;
    ctrl_parameters->steering_kd = 40;
    ctrl_parameters->steering_kk = 0;
    ctrl_parameters->steering_dynamic_proportion_max = 0;
    ctrl_parameters->steering_dynamic_proportion_min = 0;
    // 负压

}

//------------------------------------------------------------------------------
// 函数简介     控制输出结构体初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void control_pwm_out_struct_init(void)
{
    // 电机
    ctrl_pwm_out->motor_pwm_delta = 0;
    ctrl_pwm_out->motor_pwm_out = 0;

    // 舵机
    ctrl_pwm_out->steering_pwm_delta = 0;
    ctrl_pwm_out->steering_pwm_out = 0;
}

//------------------------------------------------------------------------------
// 函数简介     姿态传感器中间变量结构体初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void inertial_measurement_unit_temp_struct_init(void)
{
    imu_temp->gyro_integral_z = 0;
    imu_temp->sampling_data = 0;
    imu_temp->yaw_integral = 0;
}

//------------------------------------------------------------------------------
// 函数简介     姿态传感器参数结构体初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void inertial_measurement_unit_parameters_struct_init(void)
{
    imu_parameters->sampling_times = 300;
    imu_parameters->sampling_delay_time = 5;
    imu_parameters->sampling_dt = 0.0025f;
    imu_parameters->sampling_data_delta = 0.015f;
}

//------------------------------------------------------------------------------
// 函数简介     键盘中间参数结构体初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void keyboard_temp_struct_init(void)
{
//    kybd_temp->count_key = 0;
//    kybd_temp->count_dot = 0;
//    kybd_temp->count_flash_dot = 0;
//    kybd_temp->count_keyboard_dot = 0;
//    kybd_temp->number_temp[KEYBOARD_NUM_INPUT_LEN] = 0;
}

//------------------------------------------------------------------------------
// 函数简介     键盘长短按计时结构体初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void timer_keyboard_struct_init(void)
{
    tim_temp->count_time = 0;
}

//------------------------------------------------------------------------------
// 函数简介     全局标志位结构体初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void global_flag_struct_init(void)
{
    glb_flag->Offset_flag = 0;
    glb_flag->Close_speed_enable = 0;
    glb_flag->keyboard_scan_enable = 0;
    glb_flag->keyboard_timer_flag = 0;
    glb_flag->keyboard_long_press_flag = 0;
    glb_flag->yaw_flag = 0;
    glb_flag->start_flag = 0;
    glb_flag->breaking_flag = 0;
}

//------------------------------------------------------------------------------
// 函数简介     结构体初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void struct_init(void)
{
    ips200_show_string(0, 16*10, "all_parameters");
    // 控制部分结构体
    control_temp_struct_init();
    control_speed_struct_init();
    control_steering_struct_init();
    control_parameters_struct_init();
    control_pwm_out_struct_init();
    // 姿态传感器部分结构体
    inertial_measurement_unit_temp_struct_init();
    inertial_measurement_unit_parameters_struct_init();
    // 矩阵键盘部分结构体
    keyboard_temp_struct_init();
    // 计时器部分结构体
    timer_keyboard_struct_init();
    // 全局标志位部分结构体
    global_flag_struct_init();
    ips200_show_string(16*11, 16*10, "success");
}

//------------------------------------------------------------------------------
// 函数简介     中断初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void isr_init(void)
{
    // 初始化 pit 中断
    pit_ms_init(CCU60_CH0, 10);
    pit_ms_init(CCU60_CH1, 2);
   // pit_ms_init(CCU61_CH0, 10);
}

//------------------------------------------------------------------------------
// 函数简介     外设初始化完成提示
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void init_complete(void)
{
    system_delay_ms(500);
    gpio_set_level(buzzer, GPIO_HIGH);
    system_delay_ms(100);
    gpio_set_level(buzzer, GPIO_LOW);
}
void pitch_init(void)
{



}
//------------------------------------------------------------------------------
// 函数简介     全部设备初始化
// 参数说明     无
// 返回参数     无
// 备注信息     无
//------------------------------------------------------------------------------
void init_alldevice(void)
{
    screen_init();
    servo_init();
    small_driver_uart_init();
    imu_init();
    wireless_init();
    io_init();
    seekfree_assistant_init();
    struct_init();
    isr_init();
    init_complete();
    ips200_clear();
}
