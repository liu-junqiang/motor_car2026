/*
 * imu.h
 *
 *  Created on: 2025Äê8ÔÂ18ÈÕ
 *      Author: Pathetic.
 */

#ifndef CODE_IMU_IMU_H_
#define CODE_IMU_IMU_H_

#include "zf_common_headfile.h"

typedef struct
{
    float gyro_integral_z;
    float sampling_data;
    float yaw_integral;
}inertial_measurement_unit_temp_struct;

typedef struct
{
    uint16 sampling_times;
    uint32 sampling_delay_time;
    float sampling_dt;
    float sampling_data_delta;
}inertial_measurement_unit_parameters_struct;

extern inertial_measurement_unit_temp_struct* imu_temp;
extern inertial_measurement_unit_parameters_struct* imu_parameters;
extern float Pitch;
extern float Gyro_x;
extern float Q_angle;
extern float Q_gyro ;
extern float R_angle;
extern float zhou;


void gesture_systematic_error_calc(void);
void yaw_integral_calc(void);
void yaw_integral_handle_calc(void);
void Euler(void);
void Kalman_Filter_X(float Accel, float Gyro);

#endif /* CODE_IMU_IMU_H_ */
