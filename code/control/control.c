/*
 * control.c
 *
 *  Created on: 2025年8月16日
 *      Author: Pathetic.
 */

#include "control.h"

// 结构体定义
control_temp_struct control_temp;
control_speed_struct control_speed;
control_steering_struct control_steering;
control_parameters_struct control_parameters;
control_pwm_out_struct control_pwm_out;
global_flag_struct global_flag;
// 结构体指针初始化
control_temp_struct* ctrl_temp = &control_temp;
control_speed_struct* ctrl_speed = &control_speed;
control_steering_struct* ctrl_steering = &control_steering;
control_parameters_struct* ctrl_parameters = &control_parameters;
control_pwm_out_struct* ctrl_pwm_out = &control_pwm_out;
global_flag_struct* glb_flag = &global_flag;

float yaw_target=0;

#define PATH_LENTH 50
#define PI 3.1415926


//------------------------------------------------------------------------------
// 函数简介     编码器读数
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0    先用左相
//------------------------------------------------------------------------------
void speed_get(void)
{
    printf("left speed:%d, right speed:%d\r\n", motor_value.receive_left_speed_data, motor_value.receive_right_speed_data);
    // 读取 方向解码 脉冲数
    ctrl_temp->encoder_count = -motor_value.receive_left_speed_data;
}

//------------------------------------------------------------------------------
// 函数简介     路程获取        约49479个脉冲/m
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0    方向编码器
//lucheng_get() 函数的作用是累加编码器脉冲数，计算车辆行驶的总路程。路程信息可用于判断车辆是否到达特定位置（如坡道等），从而实现元素识别和策略切换。
//------------------------------------------------------------------------------
void lucheng_get(void)
{
    ctrl_temp->lucheng += (float)(1.0f * ctrl_temp->encoder_count);
}

//------------------------------------------------------------------------------
// 函数简介     低通滤波（平滑编码器速度）
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void low_pass_filter_calc(void)
{
    ctrl_temp->encoder_speed = ctrl_temp->encoder_count;
    static int16 encoder_speed_last = 0 ;
    ctrl_temp->encoder_speed = (int16)(ctrl_temp->encoder_speed * ctrl_parameters->low_pass_filter_k + (1 - ctrl_parameters->low_pass_filter_k) * encoder_speed_last);
    encoder_speed_last = ctrl_temp->encoder_speed;
}

//------------------------------------------------------------------------------
// 函数简介     舵机低通滤波
// 参数说明     input: 输入值
// 返回参数     滤波后的值
// 备注信息     v1.0
//------------------------------------------------------------------------------
float steering_low_pass_filter(float input)
{
    static float steering_output_last = 0;
    float output = input * ctrl_parameters->steering_low_pass_filter_k + (1 - ctrl_parameters->steering_low_pass_filter_k) * steering_output_last;
    steering_output_last = output;
    return output;
}

//------------------------------------------------------------------------------
// 函数简介     电机增量式pid
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void motor_pid_calc(void)
{
    static int16 speedset = 0;

    ctrl_temp->motor_speed = ctrl_temp->encoder_speed;
    speedset = (int16)(ctrl_parameters->setspeed * 1.0f);


    // 偏差
    ctrl_temp->speed_error_2 = ctrl_temp->speed_error_1;
    ctrl_temp->speed_error_1 = ctrl_temp->speed_error_0;
    ctrl_temp->speed_error_0 = speedset - ctrl_temp->motor_speed;


    // 比例
    ctrl_temp->motor_proportion = ctrl_temp->speed_error_0 - ctrl_temp->speed_error_1;
    // 积分
    ctrl_temp->motor_integral = ctrl_temp->speed_error_0;
    // 微分
    ctrl_temp->motor_derivation = ctrl_temp->speed_error_0 - 2 * ctrl_temp->speed_error_1 + ctrl_temp->speed_error_2;

    //积分限幅
    if(ctrl_temp->motor_integral > ctrl_parameters->motor_integral_lim)
    {
        ctrl_temp->motor_integral = ctrl_parameters->motor_integral_lim;
    }
    else  if(ctrl_temp->motor_integral < -ctrl_parameters->motor_integral_lim)
    {
        ctrl_temp->motor_integral = -ctrl_parameters->motor_integral_lim;
    }
    // 增量式pid仅当速度闭环使能
    if(glb_flag->Close_speed_enable)
    {
        ctrl_pwm_out->motor_pwm_delta = (int16)(ctrl_parameters->motor_kp * ctrl_temp->motor_proportion + ctrl_parameters->motor_ki * ctrl_temp->motor_integral + ctrl_parameters->motor_kd * ctrl_temp->motor_derivation);
        ctrl_pwm_out->motor_pwm_out += ctrl_pwm_out->motor_pwm_delta ;
    }

    // 限幅
    if(ctrl_pwm_out->motor_pwm_out > ctrl_parameters->motor_pwm_out_lim * PWM_DUTY_MAX)
    {
        ctrl_pwm_out->motor_pwm_out = ctrl_parameters->motor_pwm_out_lim * PWM_DUTY_MAX;
    }
    else if(ctrl_pwm_out->motor_pwm_out < -ctrl_parameters->motor_pwm_out_lim * PWM_DUTY_MAX)
    {
        ctrl_pwm_out->motor_pwm_out = -ctrl_parameters->motor_pwm_out_lim * PWM_DUTY_MAX;
    }
}
//------------------------------------------------------------------------------
// 函数简介     舵机角度环（二级）
// 参数说明     Pitch往右倒是正数，舵机此时应当向右打，即增大PWM
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
float Pitch_Loop_error = 0.0f;//倾角误差
float Pitch_Loop_error0 = 0.0f;//上次误差
float Pitch_Loop_Kp = 16.3f;
float Pitch_Loop_Kd = 1.5f;
float Pitch_u = 0.0f;//期望角速度

//float Pitch_Loop_error = 0.0f;//倾角误差
//float Pitch_Loop_error0 = 0.0f;//上次误差
//float Pitch_Loop_Kp = 25.3f;
//float Pitch_Loop_Kd = 4.5f;
//float Pitch_u = 0.0f;//期望角速度
void Pitch_PID_Controller(float Error_Loop_Out)
{
    Pitch_Loop_error = -Pitch + Error_Loop_Out ;
    Pitch_u = (Pitch_Loop_Kp * Pitch_Loop_error + Pitch_Loop_Kd * (Pitch_Loop_error - Pitch_Loop_error0));
    Pitch_Loop_error0 = Pitch_Loop_error;
}

//------------------------------------------------------------------------------
// 函数简介     舵机角速度环（三级）
// 参数说明     Gyro_X往右倒是正数，舵机此时应当向右打，即增大PWM
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
float Gyro_x_Loop_error = 0.0f;
float Gyro_x_Loop_error0 = 0.0f;//上次误差
float Gyro_x_Loop_Kp = 2.0f;
float Gyro_x_Loop_Kd = 0.5f;

//float Gyro_x_Loop_error = 0.0f;
//float Gyro_x_Loop_error0 = 0.0f;//上次误差
//float Gyro_x_Loop_Kp = 3.0f;
//float Gyro_x_Loop_Kd = 0.5f;
void Gyro_x_PID_Controller(float Pitch_Loop_Out)
{
    Gyro_x_Loop_error = Gyro_x - Pitch_Loop_Out ;
    ctrl_pwm_out->steering_pwm_delta =(Gyro_x_Loop_Kp * Gyro_x_Loop_error + Gyro_x_Loop_Kd * (Gyro_x_Loop_error - Gyro_x_Loop_error0));
    ctrl_pwm_out->steering_pwm_out = (steering_middle + ctrl_pwm_out->steering_pwm_delta);
    Gyro_x_Loop_error0 = Gyro_x_Loop_error;
    if(ctrl_pwm_out->steering_pwm_out < steering_left)
        ctrl_pwm_out->steering_pwm_out = steering_left;
    else if(ctrl_pwm_out->steering_pwm_out > steering_right)
        ctrl_pwm_out->steering_pwm_out = steering_right;

}

//------------------------------------------------------------------------------
// 函数简介     舵机pdk（方向环）
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
float Steering_Loop_error=0.0f;
float Steering_Loop_error0=0.0f;
float Steering_Loop_kp=0.3f;
float Steering_Loop_kd=0;
float Steering_u=0.0f;
void Steering_Loop_Controller(float Target_Yaw)
{
    Steering_Loop_error=imu_temp->yaw_integral-Target_Yaw;

    //对误差限幅
    if(Steering_Loop_error>190)
    {
        Steering_Loop_error = 360- Steering_Loop_error;
    }
    if(Steering_Loop_error<-190)
    {
        Steering_Loop_error = 360+ Steering_Loop_error;
    }

    Steering_u=-(Steering_Loop_kp*Steering_Loop_error+
        Steering_Loop_kd*(Steering_Loop_error-Steering_Loop_error0));

    Steering_Loop_error0=Steering_Loop_error;

    if( Steering_u> 20.0f)
       Steering_u=3.0f;

   else if(Steering_u<-20.0f)
       Steering_u=-3.0f;
}

//------------------------------------------------------------------------------
// 函数简介     动态控制
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void dynamic_ctrl(void)
{

     ctrl_parameters->steering_kp = ctrl_steering->steering_kp_straight;
     ctrl_parameters->steering_kd = ctrl_steering->steering_kd_straight;
     ctrl_parameters->steering_kk = ctrl_steering->steering_kk_straight;

    // 闭环
    if(glb_flag->Close_speed_enable == 1)
    {
        ctrl_parameters->setspeed = ctrl_speed->close_speed_straight;
    }
    // 开环
    else
    {
        ctrl_pwm_out->motor_pwm_out = ctrl_speed->open_speed_straight;
    }
}



small_device_value_struct motor_value;      // 定义通讯参数结构体

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无刷驱动 串口接收回调函数
// 参数说明     void
// 返回参数     void
// 使用示例     uart_control_callback(1000, -1000);
// 备注信息     用于解析接收到的速度数据  该函数需要在对应的串口接收中断中调用
//-------------------------------------------------------------------------------------------------------------------
void uart_control_callback(void)
{
    uint8 receive_data;                                                                     // 定义临时变量

    if(uart_query_byte(SMALL_DRIVER_UART, &receive_data))                                   // 接收串口数据
    {
        if(receive_data == 0xA5 && motor_value.receive_data_buffer[0] != 0xA5)              // 判断是否收到帧头 并且 当前接收内容中是否正确包含帧头
        {
            motor_value.receive_data_count = 0;                                             // 未收到帧头或者未正确包含帧头则重新接收
        }

        motor_value.receive_data_buffer[motor_value.receive_data_count ++] = receive_data;  // 保存串口数据

        if(motor_value.receive_data_count >= 7)                                             // 判断是否接收到指定数量的数据
        {
            if(motor_value.receive_data_buffer[0] == 0xA5)                                  // 判断帧头是否正确
            {

                motor_value.sum_check_data = 0;                                             // 清除校验位数据

                for(int i = 0; i < 6; i ++)
                {
                    motor_value.sum_check_data += motor_value.receive_data_buffer[i];       // 重新计算校验位
                }

                if(motor_value.sum_check_data == motor_value.receive_data_buffer[6])        // 校验数据准确性
                {

                    if(motor_value.receive_data_buffer[1] == 0x02)                          // 判断是否正确接收到 速度输出 功能字
                    {
                        motor_value.receive_left_speed_data  = (((int)motor_value.receive_data_buffer[2] << 8) | (int)motor_value.receive_data_buffer[3]);  // 拟合左侧电机转速数据

                        motor_value.receive_right_speed_data = (((int)motor_value.receive_data_buffer[4] << 8) | (int)motor_value.receive_data_buffer[5]);  // 拟合右侧电机转速数据
                    }

                    motor_value.receive_data_count = 0;                                     // 清除缓冲区计数值

                    memset(motor_value.receive_data_buffer, 0, 7);                          // 清除缓冲区数据
                }
                else
                {
                    motor_value.receive_data_count = 0;                                     // 清除缓冲区计数值

                    memset(motor_value.receive_data_buffer, 0, 7);                          // 清除缓冲区数据
                }
            }
            else
            {
                motor_value.receive_data_count = 0;                                         // 清除缓冲区计数值

                memset(motor_value.receive_data_buffer, 0, 7);                              // 清除缓冲区数据
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无刷驱动 设置电机占空比
// 参数说明     left_duty       左侧电机占空比  范围 -10000 ~ 10000  负数为反转
// 参数说明     right_duty      右侧电机占空比  范围 -10000 ~ 10000  负数为反转
// 返回参数     void
// 使用示例     small_driver_set_duty(1000, -1000);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void small_driver_set_duty(int16 left_duty, int16 right_duty)
{
    motor_value.send_data_buffer[0] = 0xA5;                                         // 配置帧头

    motor_value.send_data_buffer[1] = 0X01;                                         // 配置功能字

    motor_value.send_data_buffer[2] = (uint8)((left_duty & 0xFF00) >> 8);           // 拆分 左侧占空比 的高八位

    motor_value.send_data_buffer[3] = (uint8)(left_duty & 0x00FF);                  // 拆分 左侧占空比 的低八位

    motor_value.send_data_buffer[4] = (uint8)((right_duty & 0xFF00) >> 8);          // 拆分 右侧占空比 的高八位

    motor_value.send_data_buffer[5] = (uint8)(right_duty & 0x00FF);                 // 拆分 右侧占空比 的低八位

    motor_value.send_data_buffer[6] = 0;                                            // 和校验清除

    for(int i = 0; i < 6; i ++)
    {
        motor_value.send_data_buffer[6] += motor_value.send_data_buffer[i];         // 计算校验位
    }

    uart_write_buffer(SMALL_DRIVER_UART, motor_value.send_data_buffer, 7);                     // 发送设置占空比的 字节包 数据
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无刷驱动 获取速度信息
// 参数说明     void
// 返回参数     void
// 使用示例     small_driver_get_speed();
// 备注信息     仅需发送一次 驱动将周期发出速度信息(默认10ms)
//-------------------------------------------------------------------------------------------------------------------
void small_driver_get_speed(void)
{
    motor_value.send_data_buffer[0] = 0xA5;                                         // 配置帧头

    motor_value.send_data_buffer[1] = 0X02;                                         // 配置功能字

    motor_value.send_data_buffer[2] = 0x00;                                         // 数据位清空

    motor_value.send_data_buffer[3] = 0x00;                                         // 数据位清空

    motor_value.send_data_buffer[4] = 0x00;                                         // 数据位清空

    motor_value.send_data_buffer[5] = 0x00;                                         // 数据位清空

    motor_value.send_data_buffer[6] = 0xA7;                                         // 配置校验位

    uart_write_buffer(SMALL_DRIVER_UART, motor_value.send_data_buffer, 7);                     // 发送获取转速数据的 字节包 数据
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无刷驱动 参数初始化
// 参数说明     void
// 返回参数     void
// 使用示例     small_driver_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void small_driver_init(void)
{
    memset(motor_value.send_data_buffer, 0, 7);                             // 清除缓冲区数据

    memset(motor_value.receive_data_buffer, 0, 7);                          // 清除缓冲区数据

    motor_value.receive_data_count          = 0;

    motor_value.sum_check_data              = 0;

    motor_value.receive_right_speed_data    = 0;

    motor_value.receive_left_speed_data     = 0;
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无刷驱动 串口通讯初始化
// 参数说明     void
// 返回参数     void
// 使用示例     small_driver_uart_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void small_driver_uart_init(void)
{
    uart_init(SMALL_DRIVER_UART, SMALL_DRIVER_BAUDRATE, SMALL_DRIVER_RX, SMALL_DRIVER_TX);      // 串口初始化

    uart_rx_interrupt(SMALL_DRIVER_UART, 1);                                                    // 使能串口接收中断

    small_driver_init();                                                                        // 结构体参数初始化

    small_driver_set_duty(0, 0);                                                                // 设置0占空比

    small_driver_get_speed();                                                                   // 获取实时速度数据
}
//------------------------------------------------------------------------------
// 函数简介     pwm输出
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void pwm_out(void)
{
    // 舵机输出
    //pwm_set_duty(steering_pwm, steering_middle);
    pwm_set_duty(steering_pwm, ctrl_pwm_out->steering_pwm_out);

    small_driver_set_duty(-ctrl_pwm_out->motor_pwm_out,-ctrl_pwm_out->motor_pwm_out);
    //small_driver_set_duty(0, 0);
    //pwm_set_duty(steering_pwm, 560);
//    //倾倒判断
//    if(Pitch>28||Pitch<-28)
//    {
//        pwm_set_duty(steering_pwm, steering_middle);
//        small_driver_set_duty(0, 0);
//    }

}

////------------------------------------------------------------------------------
//// 函数简介     pwm输出(原地)
//// 参数说明     无
//// 返回参数     无
//// 备注信息     v1.0
////------------------------------------------------------------------------------
//uint8_t time=0;
//void pwm_out_yuan(void)
//{
//    // 舵机输出
//    pwm_set_duty(steering_pwm, ctrl_pwm_out->steering_pwm_out);
//
//    //电机输出
//
//    if(time<=20)
//    {
//        small_driver_set_duty(-ctrl_pwm_out->motor_pwm_out,-ctrl_pwm_out->motor_pwm_out);
//        time++;
//    }
//    if(time>20&&time<=40)
//    {
//        small_driver_set_duty(ctrl_pwm_out->motor_pwm_out,ctrl_pwm_out->motor_pwm_out);
//        time++;
//    }
//    if(time>40)
//    {
//        time =0;
//    }
//
//
//
//    //倾倒判断
//    if(Pitch>28||Pitch<-28)
//    {
//        pwm_set_duty(steering_pwm, steering_middle);
//        small_driver_set_duty(0, 0);
//    }
//
//}

uint8_t  ptemp=0.0f;
void pitch_pid_cnt(void)
{
    ptemp++;
    if(ptemp==4)
    {
        ptemp=0;
        Pitch_PID_Controller(Steering_u);
    }

}
uint8_t  ptemp2=0.0f;
void pdk_pid_cnt(void)
{
    ptemp2++;
    if(ptemp2==10)
    {
        ptemp2=0;
        Steering_Loop_Controller(yaw_target);
        //Steering_Loop_Controller(180);
    }

}

//------------------------------------------------------------------------------
// 函数简介     科目一
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
enum sub1 kemu1=road1;
//uint64 sub1_sim=989580; //跑直线路程的脉冲数 20m
uint64 sub1_sim=300000;
void subject1(void)
{
    if(kemu1==road1)
    {
        yaw_target=0;
    }
    if(kemu1==road1 && ctrl_temp->lucheng >=sub1_sim)  //判断转弯
    {
        kemu1=road_circle;
        gpio_set_level(LED, GPIO_LOW);
        ctrl_temp->lucheng = 0;
    }
    if(kemu1==road_circle)
    {
        yaw_target = 180;
    }
//    if((imu_temp->yaw_integral>175||imu_temp->yaw_integral<-175) && kemu1==road_circle)//判断转弯结束
//    {
//        kemu1= road2;
//        ctrl_temp->lucheng=0;
//    }
//    if(kemu1==road2)
//    {
//        yaw_target = 180;
//    }
//    if(kemu1==road2 && ctrl_temp->lucheng >=sub1_sim) //科目一结束
//    {
//        ctrl_speed->open_speed_straight = 0;
//    }

}

//------------------------------------------------------------------------------
// 函数简介     航位推算
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0 放在计算航向角的中断里
//------------------------------------------------------------------------------
float pos_now[2]={0};//(x,y)
void get_pos(void)
{
    pos_now[0] = pos_now[0] + (float)(1.0f * ctrl_temp->encoder_count)*sin(imu_temp->yaw_integral*(3.14/180));
    pos_now[1] = pos_now[1] + (float)(1.0f * ctrl_temp->encoder_count)*sin(imu_temp->yaw_integral*(3.14/180));
}

//------------------------------------------------------------------------------
// 函数简介     生成预设路径
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
float path[PATH_LENTH][2]={{0,0}};//(x,y)
uint8_t half = PATH_LENTH/2;
float dist1=1;
float dist2=3;
float R_turn=1.5;//路径半径
void get_path(void)
{
    for (int i = 0; i < PATH_LENTH; i++)
    {
        //从左进入
            if (i < half)
            {
                float t = (float)i / half * 2.0f * PI;
                path[i][0] = -R_turn * sinf(t);
                if(t<=PI)
                {
                    path[i][1] = dist1 - R_turn * cosf(t);
                }
                else
                {
                    path[i][1] = dist2 + R_turn * cosf(t);
                }
            }
            else
            {

                float t = (float)(i - half) / half * 2.0f * PI;
                path[i][0] = R_turn * sinf(t);
                if(t<=PI)
                {
                    path[i][1] = dist2 + R_turn * cosf(t);
                }
                else
                {
                    path[i][1] = dist1 - R_turn * cosf(t);
                }

            }
        }
}
