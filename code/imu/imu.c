/*
 * imu.c
 *
 *  Created on: 2025年8月18日
 *      Author: Pathetic.
 */

#include "imu.h"

// 结构体定义
inertial_measurement_unit_temp_struct inertial_measurement_unit_temp;
inertial_measurement_unit_parameters_struct inertial_measurement_unit_parameters;
// 结构体指针初始化
inertial_measurement_unit_temp_struct* imu_temp = &inertial_measurement_unit_temp;
inertial_measurement_unit_parameters_struct* imu_parameters = &inertial_measurement_unit_parameters;

//------------------------------------------------------------------------------
// 函数简介     陀螺仪零漂指示
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void gesture_systematic_error_note(void)
{
    if(glb_flag->Offset_flag == 0)
    {
        gpio_set_level(LED, GPIO_LOW);
        ips200_show_string(0, 16*0, "imu zero error dealing...");
        ips200_show_string(0, 16*1, "Please don't move the car!");
    }
    else
    {
        ips200_clear();
        ips200_show_string(0, 16*0, "imu zero error deal completed.");
        gpio_set_level(LED, GPIO_HIGH);
        system_delay_ms(1000);
        ips200_clear();
    }
}

//------------------------------------------------------------------------------
// 函数简介     陀螺仪零漂
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void gesture_systematic_error_calc(void)
{
    // 标志位清零
    glb_flag->Offset_flag = 0;
    gesture_systematic_error_note();

    // 采样
    for(uint16 i = 0; i < imu_parameters->sampling_times; i++)
    {
        // 陀螺仪读数
        imu660ra_get_gyro();
        // 积分
        imu_temp->gyro_integral_z += (float)imu660ra_gyro_z;
        system_delay_ms(imu_parameters->sampling_delay_time);
    }

    // 平均
    imu_temp->gyro_integral_z /= (1.0f * imu_parameters->sampling_times);

    // 标志位置位
    glb_flag->Offset_flag = 1;
    gesture_systematic_error_note();
}

//------------------------------------------------------------------------------
// 函数简介     积分获取偏航角
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void yaw_integral_calc(void)
{
    if(glb_flag->Offset_flag == 1 && glb_flag->yaw_flag == 1)
    {
        // 获取原始数据
        imu660ra_get_gyro();

        // 去零漂
        imu_temp->sampling_data = imu660ra_gyro_z - imu_temp->gyro_integral_z;
        imu_temp->sampling_data = imu_temp->sampling_data * PI / 180 / 16.4f;

        // 引入死区（滤波）
        if(imu_temp->sampling_data > -imu_parameters->sampling_data_delta && imu_temp->sampling_data < imu_parameters->sampling_data_delta)
            imu_temp->sampling_data = 0;
        else
            imu_temp->yaw_integral += RAD_TO_ANGLE(imu_temp->sampling_data * imu_parameters->sampling_dt);
    }
}

//------------------------------------------------------------------------------
// 函数简介     偏航角限幅
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0
//------------------------------------------------------------------------------
void yaw_integral_handle_calc(void)
{
    if(imu_temp->yaw_integral > 180.0f)
        imu_temp->yaw_integral -= 360;
    else if(imu_temp->yaw_integral < -180.0f)
        imu_temp->yaw_integral += 360;
}

float Pitch;//所需pitch角
float Gyro_x;//所需角速度
float Gyro_z;//所需角速度
float ay, az;
float Angle_x_temp;
float Angle_X_Final;
float Angle_Z_Final;
float zhou;
float Gyro_x_temp;
float Gyro_x_last=0;
float Alpha=0.07f;//0-1,越大对加速度越信任
extern signed short  imu660ra_acc_y, imu660ra_acc_z;            //加速度传感器原始数据
extern signed short imu660ra_gyro_x;         //陀螺仪原始数据

void Euler(void) {
    /*获取X、Y的角速度和加速度*/
    imu660ra_get_acc();
    imu660ra_get_gyro();

    //将 imu660ra 加速度计数据转换为实际物理数据将，单位为 g(m/s^2)
    ay = imu660ra_acc_transition(imu660ra_acc_y);
    az = imu660ra_acc_transition(imu660ra_acc_z);

    Gyro_x_temp = imu660ra_gyro_transition(imu660ra_gyro_x);//把陀螺仪读取到的角速度转换成实际角速度付给Gyro_x_temp
        Gyro_x = Alpha * Gyro_x_temp + (1-Alpha) * Gyro_x_last;
        Gyro_x_last = Gyro_x;

        zhou=imu660ra_gyro_transition(imu660ra_gyro_x);//比较滤波效果
    //将 imu660ra 陀螺仪数据转换为实际物理数据，单位为°/s

    Gyro_z = imu660ra_gyro_transition(imu660ra_gyro_z);

    //用加速度计算三个轴和水平面坐标系之间的夹角
    Angle_x_temp = (atan(ay / az)) * 180 / 3.14;
    Angle_Z_Final += Gyro_z * 0.002;
    Kalman_Filter_X(Angle_x_temp, Gyro_x);  //卡尔曼滤波计算X倾角
}


//卡尔曼参数
float Q_angle = 0.006;//越大对陀螺仪信任度越高
float Q_gyro = 0.002;//越大对加速度信任度越高0.001-0.005
float R_angle = 3.0;//越大对陀螺仪信任度越高
float dt = 0.002;//dt为kalman滤波器采样时间
char C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] = {0, 0, 0, 0};
float PP[2][2] = {{1, 0},
                  {0, 1}};

void Kalman_Filter_X(float Accel, float Gyro) //卡尔曼函数
{
    Angle_X_Final += (Gyro - Q_bias) * dt; //先验估计

    Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

    Pdot[1] = -PP[1][1];
    Pdot[2] = -PP[1][1];
    Pdot[3] = Q_gyro;

    PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
    PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;


    Angle_err = Accel - Angle_X_Final;  //zk-先验估计

    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;
    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0;       //后验估计误差协方差
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;

    Angle_X_Final += K_0 * Angle_err;    //后验估计
    Q_bias += K_1 * Angle_err;    //后验估计
    Gyro_x = Gyro - Q_bias;  //输出值（后验估计）的微分 = 角速度
    Pitch = Angle_X_Final;
}
